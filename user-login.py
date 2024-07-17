import jwt
import secrets
from cryptography.fernet import Fernet
from argon2 import PasswordHasher
from datetime import datetime, timedelta
from dotenv import load_dotenv
from flask import Flask, request, jsonify
from flask_sqlalchemy import SQLAlchemy
from flask_security import Security, SQLAlchemyUserDatastore, UserMixin, RoleMixin, login_required
import os

# Set up logging
import logging
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

# Load environment variables from a .env file
load_dotenv()

# Password policy parameters
MIN_PASSWORD_LENGTH = 12
PASSWORD_COMPLEXITY_REQUIREMENT = {"uppercase": True, "lowercase": True, "digit": True, "special_char": True}

# Session management and JWT settings
active_sessions = {}
SECRET_KEY = os.getenv("APP_SECRET_KEY", 'your_secret_key')  # Default to a value if not set
SECURITY_PASSWORD_SALT = os.getenv("SECURITY_PASSWORD_SALT", 'your_salt')  # Default to a value if not set

# Initialize Flask app
app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///example.db'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
app.config['SECRET_KEY'] = SECRET_KEY
app.config['SECURITY_PASSWORD_SALT'] = SECURITY_PASSWORD_SALT

db = SQLAlchemy(app)

# Define User and Role models for Flask-Security
class RolesUsers(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    user_id = db.Column(db.Integer, db.ForeignKey('user.id'))
    role_id = db.Column(db.Integer, db.ForeignKey('role.id'))

class Role(db.Model, RoleMixin):
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(80), unique=True)

class User(db.Model, UserMixin):
    id = db.Column(db.Integer, primary_key=True)
    username = db.Column(db.String(80), unique=True)
    email = db.Column(db.String(255), unique=True)
    password = db.Column(db.String(255))
    active = db.Column(db.Boolean)
    confirmed_at = db.Column(db.DateTime())
    roles = db.relationship('Role', secondary='roles_users', backref=db.backref('users', lazy='dynamic'))

# Setup Flask-Security
user_datastore = SQLAlchemyUserDatastore(db, User, Role)
security = Security(app, user_datastore)

# Custom User class for enhanced authentication
class CustomUser(User):
    def __init__(self, username, password):
        super().__init__(username=username, password=password)
        self.salt = secrets.token_hex(32)  # Increased salt length for better security
        self.hash_key = self.hash_password(password, self.salt)
        self.folded_hash = self.fold_hash(self.hash_key)
        self.encryption_key = Fernet.generate_key()

    def hash_password(self, password, salt):
        # Use Argon2 for password hashing
        ph = PasswordHasher()
        hashed_password = ph.hash(password + salt)
        return hashed_password

    def fold_hash(self, hash_key):
        folded_hash = hash_key[:len(hash_key)//2] + hash_key[len(hash_key)//2:][::-1]
        return folded_hash

    def encrypt(self, text, key):
        cipher = Fernet(key)
        encrypted_text = cipher.encrypt(text.encode())
        return encrypted_text

    def store_user(self):
        user_database[self.username] = {
            'salt': self.salt,
            'encrypted_hash': self.encrypt(self.folded_hash, self.encryption_key),
            'encryption_key': self.encryption_key
        }
        logging.info(f"User '{self.username}' registered successfully.")

    def authenticate(self, entered_password):
        ph = PasswordHasher()
        try:
            ph.verify(self.hash_password(entered_password, self.salt), self.hash_key)
            logging.info(f"Authentication successful for user '{self.username}'.")
            return True
        except Exception as e:
            logging.warning(f"Authentication failed for user '{self.username}': {e}")
            return False

# Routes
@app.route('/register', methods=['POST'])
def register():
    data = request.get_json()
    username = data.get('username')
    password = data.get('password')
    email = data.get('email')

    if not username or not password or not email:
        return jsonify({'error': 'Incomplete data provided'}), 400

    # Check if the user already exists
    existing_user = user_datastore.find_user(username=username) or user_datastore.find_user(email=email)
    if existing_user:
        return jsonify({'error': 'User already exists'}), 400

    # Create a new user using enhanced authentication
    new_user = CustomUser(username, password)
    new_user.store_user()

    # Create a new user for Flask-Security
    user_datastore.create_user(username=username, email=email, password=security.hash_password(password))
    db.session.commit()

    return jsonify({'message': 'User registered successfully'}), 201

@app.route('/login', methods=['POST'])
def login():
    data = request.get_json()
    username = data.get('username')
    password = data.get('password')

    # Check if the user exists in either enhanced authentication or Flask-Security
    user = user_datastore.find_user(username=username) or user_datastore.find_user(email=username)
    if user:
        custom_user = CustomUser(username, password)
        if custom_user.authenticate(password) or security.verify_password(password, user.password):
            return jsonify({'message': 'Login successful'}), 200
        else:
            return jsonify({'error': 'Invalid credentials'}), 401
    else:
        return jsonify({'error': 'User not found'}), 404

@app.route('/protected', methods=['GET'])
@login_required
def protected():
    return jsonify({'message': 'Access granted to protected route'}), 200

if __name__ == '__main__':
    db.create_all()
    app.run(debug=True)

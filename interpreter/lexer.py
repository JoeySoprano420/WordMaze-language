class Token:
    def __init__(self, type, value):
        self.type = type
        self.value = value

class Lexer:
    def __init__(self, text):
        self.text = text
        self.pos = 0
        self.current_char = self.text[self.pos] if self.pos < len(self.text) else None

    def error(self):
        raise Exception('Invalid character')

    def advance(self):
        self.pos += 1
        if self.pos < len(self.text):
            self.current_char = self.text[self.pos]
        else:
            self.current_char = None

    def peek(self):
        peek_pos = self.pos + 1
        if peek_pos < len(self.text):
            return self.text[peek_pos]
        else:
            return None

    def skip_whitespace(self):
        while self.current_char is not None and self.current_char.isspace():
            self.advance()

    def get_number(self):
        num = ''
        while self.current_char is not None and self.current_char.isdigit():
            num += self.current_char
            self.advance()
        if self.current_char == '.':
            num += '.'
            self.advance()
            while self.current_char is not None and self.current_char.isdigit():
                num += self.current_char
                self.advance()
        return float(num) if '.' in num else int(num)

    def get_word(self):
        word = ''
        while self.current_char is not None and (self.current_char.isalnum() or self.current_char == '_'):
            word += self.current_char
            self.advance()
        return word

    def get_next_token(self):
        while self.current_char is not None:

            if self.current_char.isspace():
                self.skip_whitespace()
                continue

            if self.current_char.isdigit():
                return Token('NUMBER', self.get_number())

            if self.current_char.isalpha() or self.current_char == '_':
                word = self.get_word()
                if word in ['let', 'if', 'else', 'while', 'for', 'try', 'catch', 'function', 'return', 'print']:
                    return Token(word.upper(), word)
                return Token('WORD', word)

            if self.current_char == '+':
                self.advance()
                return Token('PLUS', '+')

            if self.current_char == '-':
                self.advance()
                return Token('MINUS', '-')

            if self.current_char == '*':
                self.advance()
                return Token('MULTIPLY', '*')

            if self.current_char == '/':
                self.advance()
                return Token('DIVIDE', '/')

            if self.current_char == '(':
                self.advance()
                return Token('LPAREN', '(')

            if self.current_char == ')':
                self.advance()
                return Token('RPAREN', ')')

            if self.current_char == '=':
                self.advance()
                if self.current_char == '=':
                    self.advance()
                    return Token('EQUALS_EQUALS', '==')
                return Token('EQUALS', '=')

            if self.current_char == ',':
                self.advance()
                return Token('COMMA', ',')

            if self.current_char == ':':
                self.advance()
                return Token('COLON', ':')

            if self.current_char == '{':
                self.advance()
                return Token('LCURLY', '{')

            if self.current_char == '}':
                self.advance()
                return Token('RCURLY', '}')

            if self.current_char == ';':
                self.advance()
                return Token('SEMICOLON', ';')

            self.error()

        return Token('EOF', None)

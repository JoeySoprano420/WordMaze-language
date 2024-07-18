#include <iostream>
#include <memory>
#include <vector>
#include <stdexcept>

// Custom exception for AST-related errors
class ASTException : public std::exception {
private:
    std::string message;
public:
    ASTException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

// Forward declaration of ASTNode
class ASTNode;

// Abstract base class for AST nodes
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void generateCode(std::ostream& os, int depth = 0) const = 0;
};

// Expression node representing any value or variable
class ExpressionNode : public ASTNode {
private:
    std::string value;
public:
    ExpressionNode(const std::string& val) : value(val) {}
    void generateCode(std::ostream& os, int depth = 0) const override {
        os << value;
    }
};

// AST node for if-else statement
class IfNode : public ASTNode {
private:
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> thenBranch;
    std::unique_ptr<ASTNode> elseBranch;
public:
    IfNode(std::unique_ptr<ASTNode> cond, std::unique_ptr<ASTNode> thenBr, std::unique_ptr<ASTNode> elseBr = nullptr)
        : condition(std::move(cond)), thenBranch(std::move(thenBr)), elseBranch(std::move(elseBr)) {}

    void generateCode(std::ostream& os, int depth = 0) const override {
        if (!condition || !thenBranch) {
            throw ASTException("IfNode: Missing condition or then branch");
        }
        os << std::string(depth, '\t') << "if (";
        condition->generateCode(os);
        os << ") {\n";
        thenBranch->generateCode(os, depth + 1);
        os << "\n" << std::string(depth, '\t') << "}";
        if (elseBranch) {
            os << " else {\n";
            elseBranch->generateCode(os, depth + 1);
            os << "\n" << std::string(depth, '\t') << "}";
        }
    }
};

// AST node for a sequence of statements
class SequenceNode : public ASTNode {
private:
    std::vector<std::unique_ptr<ASTNode>> statements;
public:
    void addStatement(std::unique_ptr<ASTNode> stmt) {
        if (!stmt) {
            throw ASTException("SequenceNode: Cannot add null statement");
        }
        statements.push_back(std::move(stmt));
    }

    void generateCode(std::ostream& os, int depth = 0) const override {
        for (const auto& stmt : statements) {
            stmt->generateCode(os, depth);
            os << ";\n";
        }
    }
};

// AST node for a function call
class FunctionCallNode : public ASTNode {
private:
    std::string functionName;
    std::vector<std::unique_ptr<ASTNode>> arguments;
public:
    FunctionCallNode(const std::string& name, std::vector<std::unique_ptr<ASTNode>> args)
        : functionName(name), arguments(std::move(args)) {}

    void generateCode(std::ostream& os, int depth = 0) const override {
        os << functionName << "(";
        bool first = true;
        for (const auto& arg : arguments) {
            if (!first) os << ", ";
            arg->generateCode(os);
            first = false;
        }
        os << ")";
    }
};

// AST node for variable declaration
class VariableDeclarationNode : public ASTNode {
private:
    std::string type;
    std::string name;
public:
    VariableDeclarationNode(const std::string& t, const std::string& n)
        : type(t), name(n) {}

    void generateCode(std::ostream& os, int depth = 0) const override {
        if (type.empty() || name.empty()) {
            throw ASTException("VariableDeclarationNode: Missing type or name");
        }
        os << std::string(depth, '\t') << type << " " << name;
    }
};

// AST node for assignment operation
class AssignmentNode : public ASTNode {
private:
    std::unique_ptr<ASTNode> lhs;
    std::unique_ptr<ASTNode> rhs;
public:
    AssignmentNode(std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right)
        : lhs(std::move(left)), rhs(std::move(right)) {}

    void generateCode(std::ostream& os, int depth = 0) const override {
        if (!lhs || !rhs) {
            throw ASTException("AssignmentNode: Missing left-hand side or right-hand side");
        }
        lhs->generateCode(os);
        os << " = ";
        rhs->generateCode(os);
    }
};

// AST node for binary operation
class BinaryOperationNode : public ASTNode {
private:
    std::unique_ptr<ASTNode> left;
    std::string op;
    std::unique_ptr<ASTNode> right;
public:
    BinaryOperationNode(std::unique_ptr<ASTNode> l, const std::string& o, std::unique_ptr<ASTNode> r)
        : left(std::move(l)), op(o), right(std::move(r)) {}

    void generateCode(std::ostream& os, int depth = 0) const override {
        if (!left || !right) {
            throw ASTException("BinaryOperationNode: Missing left or right operand");
        }
        os << "(";
        left->generateCode(os);
        os << " " << op << " ";
        right->generateCode(os);
        os << ")";
    }
};

// AST node for unary operation
class UnaryOperationNode : public ASTNode {
private:
    std::string op;
    std::unique_ptr<ASTNode> operand;
public:
    UnaryOperationNode(const std::string& o, std::unique_ptr<ASTNode> opnd)
        : op(o), operand(std::move(opnd)) {}

    void generateCode(std::ostream& os, int depth = 0) const override {
        if (!operand) {
            throw ASTException("UnaryOperationNode: Missing operand");
        }
        os << op;
        operand->generateCode(os);
    }
};

// AST node for while loop
class WhileLoopNode : public ASTNode {
private:
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> body;
public:
    WhileLoopNode(std::unique_ptr<ASTNode> cond, std::unique_ptr<ASTNode> b)
        : condition(std::move(cond)), body(std::move(b)) {}

    void generateCode(std::ostream& os, int depth = 0) const override {
        if (!condition || !body) {
            throw ASTException("WhileLoopNode: Missing condition or body");
        }
        os << std::string(depth, '\t') << "while (";
        condition->generateCode(os);
        os << ") {\n";
        body->generateCode(os, depth + 1);
        os << "\n" << std::string(depth, '\t') << "}";
    }
};

// AST node for for loop
class ForLoopNode : public ASTNode {
private:
    std::unique_ptr<ASTNode> init;
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> iteration;
    std::unique_ptr<ASTNode> body;
public:
    ForLoopNode(std::unique_ptr<ASTNode> i, std::unique_ptr<ASTNode> cond,
                std::unique_ptr<ASTNode> iter, std::unique_ptr<ASTNode> b)
        : init(std::move(i)), condition(std::move(cond)), iteration(std::move(iter)), body(std::move(b)) {}

    void generateCode(std::ostream& os, int depth = 0) const override {
        if (!condition || !body) {
            throw ASTException("ForLoopNode: Missing condition or body");
        }
        os << std::string(depth, '\t') << "for (";
        if (init) init->generateCode(os);
        os << "; ";
        if (condition) condition->generateCode(os);
        os << "; ";
        if (iteration) iteration->generateCode(os);
        os << ") {\n";
        body->generateCode(os, depth + 1);
        os << "\n" << std::string(depth, '\t') << "}";
    }
};

int main() {
    try {
        // Example AST generation
        std::unique_ptr<ASTNode> ifElseNode = std::make_unique<IfNode>(
            std::make_unique<ExpressionNode>("x > 10"),
            std::make_unique<ExpressionNode>("doSomething()"),
            std::make_unique<ExpressionNode>("doSomethingElse()")
        );

        std::unique_ptr<ASTNode> sequenceNode = std::make_unique<SequenceNode>();
        sequenceNode->addStatement(std::make_unique<VariableDeclarationNode>("int", "x"));
        sequenceNode->addStatement(std::move(ifElseNode));
        sequenceNode->addStatement(std::make_unique<FunctionCallNode>("print", std::vector<std::unique_ptr<ASTNode>>{
            std::make_unique<ExpressionNode>("\"Hello, world!\"")
        }));

        // Generate code for the entire AST
        sequenceNode->generateCode(std::cout);

    } catch (const ASTException& e) {
        std::cerr << "AST Exception: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception occurred" << std::endl;
    }

    return 0;
}
#include <iostream>
#include <memory>
#include <stdexcept>

// Custom exception for AST-related errors
class ASTException : public std::exception {
private:
    std::string message;
public:
    ASTException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

// Abstract base class for AST nodes
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void generateCode(std::ostream& os) const = 0;
};

// Expression node representing any value or variable
class ExpressionNode : public ASTNode {
private:
    std::string value;
public:
    ExpressionNode(const std::string& val) : value(val) {}
    void generateCode(std::ostream& os) const override {
        os << value;
    }
};

// AST node for if-else statement
class IfNode : public ASTNode {
private:
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> thenBranch;
    std::unique_ptr<ASTNode> elseBranch;
public:
    IfNode(std::unique_ptr<ASTNode> cond, std::unique_ptr<ASTNode> thenBr, std::unique_ptr<ASTNode> elseBr = nullptr)
        : condition(std::move(cond)), thenBranch(std::move(thenBr)), elseBranch(std::move(elseBr)) {}

    void generateCode(std::ostream& os) const override {
        if (!condition || !thenBranch) {
            throw ASTException("IfNode: Missing condition or then branch");
        }
        os << "if (";
        condition->generateCode(os);
        os << ") {\n";
        thenBranch->generateCode(os);
        os << "\n}";
        if (elseBranch) {
            os << " else {\n";
            elseBranch->generateCode(os);
            os << "\n}";
        }
    }
};

int main() {
    try {
        // Example AST generation
        std::unique_ptr<ASTNode> ifElseNode = std::make_unique<IfNode>(
            std::make_unique<ExpressionNode>("x > 10"),
            std::make_unique<ExpressionNode>("doSomething();"),
            std::make_unique<ExpressionNode>("doSomethingElse();")
        );

        // Generate code for the entire AST
        ifElseNode->generateCode(std::cout);

    } catch (const ASTException& e) {
        std::cerr << "AST Exception: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception occurred" << std::endl;
    }

    return 0;
}
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <stdexcept>

// Custom exception for AST-related errors
class ASTException : public std::exception {
private:
    std::string message;
public:
    ASTException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

// Forward declaration of ASTNode
class ASTNode;

// Abstract base class for AST nodes
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void generateCode(std::ostream& os, int depth = 0) const = 0;
};

// Expression node representing any value or variable
class ExpressionNode : public ASTNode {
private:
    std::string value;
public:
    ExpressionNode(const std::string& val) : value(val) {}
    void generateCode(std::ostream& os, int depth = 0) const override {
        os << value;
    }
};

// AST node for if-else statement
class IfNode : public ASTNode {
private:
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> thenBranch;
    std::unique_ptr<ASTNode> elseBranch;
public:
    IfNode(std::unique_ptr<ASTNode> cond, std::unique_ptr<ASTNode> thenBr, std::unique_ptr<ASTNode> elseBr = nullptr)
        : condition(std::move(cond)), thenBranch(std::move(thenBr)), elseBranch(std::move(elseBr)) {}

    void generateCode(std::ostream& os, int depth = 0) const override {
        if (!condition || !thenBranch) {
            throw ASTException("IfNode: Missing condition or then branch");
        }
        os << std::string(depth, '\t') << "if (";
        condition->generateCode(os);
        os << ") {\n";
        thenBranch->generateCode(os, depth + 1);
        os << "\n" << std::string(depth, '\t') << "}";
        if (elseBranch) {
            os << " else {\n";
            elseBranch->generateCode(os, depth + 1);
            os << "\n" << std::string(depth, '\t') << "}";
        }
    }
};

// AST node for a sequence of statements
class SequenceNode : public ASTNode {
private:
    std::vector<std::unique_ptr<ASTNode>> statements;
public:
    void addStatement(std::unique_ptr<ASTNode> stmt) {
        if (!stmt) {
            throw ASTException("SequenceNode: Cannot add null statement");
        }
        statements.push_back(std::move(stmt));
    }

    void generateCode(std::ostream& os, int depth = 0) const override {
        for (const auto& stmt : statements) {
            stmt->generateCode(os, depth);
            os << ";\n";
        }
    }
};

// AST node for a function call
class FunctionCallNode : public ASTNode {
private:
    std::string functionName;
    std::vector<std::unique_ptr<ASTNode>> arguments;
public:
    FunctionCallNode(const std::string& name, std::vector<std::unique_ptr<ASTNode>> args)
        : functionName(name), arguments(std::move(args)) {}

    void generateCode(std::ostream& os, int depth = 0) const override {
        os << functionName << "(";
        bool first = true;
        for (const auto& arg : arguments) {
            if (!first) os << ", ";
            arg->generateCode(os);
            first = false;
        }
        os << ")";
    }
};

// AST node for variable declaration
class VariableDeclarationNode : public ASTNode {
private:
    std::string type;
    std::string name;
public:
    VariableDeclarationNode(const std::string& t, const std::string& n)
        : type(t), name(n) {}

    void generateCode(std::ostream& os, int depth = 0) const override {
        if (type.empty() || name.empty()) {
            throw ASTException("VariableDeclarationNode: Missing type or name");
        }
        os << std::string(depth, '\t') << type << " " << name;
    }
};

// AST node for assignment operation
class AssignmentNode : public ASTNode {
private:
    std::unique_ptr<ASTNode> lhs;
    std::unique_ptr<ASTNode> rhs;
public:
    AssignmentNode(std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right)
        : lhs(std::move(left)), rhs(std::move(right)) {}

    void generateCode(std::ostream& os, int depth = 0) const override {
        if (!lhs || !rhs) {
            throw ASTException("AssignmentNode: Missing left-hand side or right-hand side");
        }
        lhs->generateCode(os);
        os << " = ";
        rhs->generateCode(os);
    }
};

// AST node for binary operation
class BinaryOperationNode : public ASTNode {
private:
    std::unique_ptr<ASTNode> left;
    std::string op;
    std::unique_ptr<ASTNode> right;
public:
    BinaryOperationNode(std::unique_ptr<ASTNode> l, const std::string& o, std::unique_ptr<ASTNode> r)
        : left(std::move(l)), op(o), right(std::move(r)) {}

    void generateCode(std::ostream& os, int depth = 0) const override {
        if (!left || !right) {
            throw ASTException("BinaryOperationNode: Missing left or right operand");
        }
        os << "(";
        left->generateCode(os);
        os << " " << op << " ";
        right->generateCode(os);
        os << ")";
    }
};

// AST node for unary operation
class UnaryOperationNode : public ASTNode {
private:
    std::string op;
    std::unique_ptr<ASTNode> operand;
public:
    UnaryOperationNode(const std::string& o, std::unique_ptr<ASTNode> opnd)
        : op(o), operand(std::move(opnd)) {}

    void generateCode(std::ostream& os, int depth = 0) const override {
        if (!operand) {
            throw ASTException("UnaryOperationNode: Missing operand");
        }
        os << op;
        operand->generateCode(os);
    }
};

// AST node for while loop
class WhileLoopNode : public ASTNode {
private:
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> body;
public:
    WhileLoopNode(std::unique_ptr<ASTNode> cond, std::unique_ptr<ASTNode> b)
        : condition(std::move(cond)), body(std::move(b)) {}

    void generateCode(std::ostream& os, int depth = 0) const override {
        if (!condition || !body) {
            throw ASTException("WhileLoopNode: Missing condition or body");
        }
        os << std::string(depth, '\t') << "while (";
        condition->generateCode(os);
        os << ") {\n";
        body->generateCode(os, depth + 1);
        os << "\n" << std::string(depth, '\t') << "}";
    }
};

// AST node for for loop
class ForLoopNode : public ASTNode {
private:
    std::unique_ptr<ASTNode> init;
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> iteration;
    std::unique_ptr<ASTNode> body;
public:
    ForLoopNode(std::unique_ptr<ASTNode> i, std::unique_ptr<ASTNode> cond,
                std::unique_ptr<ASTNode> iter, std::unique_ptr<ASTNode> b)
        : init(std::move(i)), condition(std::move(cond)), iteration(std::move(iter)), body(std::move(b)) {}

    void generateCode(std::ostream& os, int depth = 0) const override {
        if (!condition || !body) {
            throw ASTException("ForLoopNode: Missing condition or body");
        }
        os << std::string(depth, '\t') << "for (";
        if (init) init->generateCode(os);
        os << "; ";
        if (condition) condition->generateCode(os);
        os << "; ";
        if (iteration) iteration->generateCode(os);
        os << ") {\n";
        body->generateCode(os, depth + 1);
        os << "\n" << std::string(depth, '\t') << "}";
    }
};

int main() {
    try {
        // Example AST generation
        std::unique_ptr<ASTNode> ifElseNode = std::make_unique<IfNode>(
            std::make_unique<ExpressionNode>("x > 10"),
            std::make_unique<ExpressionNode>("doSomething()"),
            std::make_unique<ExpressionNode>("doSomethingElse()")
        );

        std::unique_ptr<ASTNode> sequenceNode = std::make_unique<SequenceNode>();
        sequenceNode->addStatement(std::make_unique<VariableDeclarationNode>("int", "x"));
        sequenceNode->addStatement(std::move(ifElseNode));
        sequenceNode->addStatement(std::make_unique<FunctionCallNode>("print", std::vector<std::unique_ptr<ASTNode>>{
            std::make_unique<ExpressionNode>("\"Hello, world!\"")
        }));

        // Generate code
        sequenceNode->generateCode(std::cout);
        std::cout << std::endl;
    } catch (const ASTException& e) {
        std::cerr << "AST Error: " << e.what() << std::endl;
    }
    return 0;
}

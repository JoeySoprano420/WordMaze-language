// Enhanced IDE Integration with injector functions and modular architecture

#include <iostream>
#include <string>
#include <memory>  // For std::unique_ptr

// Forward declaration of IDE interfaces
class IDEPlugin;
class ASTNode;

// Abstract base class for AST nodes
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void generateCode() const = 0;
    virtual void accept(IDEPlugin& plugin) const = 0;
};

// Expression Node with dynamic value
class ExpressionNode : public ASTNode {
    std::string value;

public:
    ExpressionNode(std::string val) : value(std::move(val)) {}

    void generateCode() const override {
        std::cout << value;
    }

    void accept(IDEPlugin& plugin) const override {
        plugin.visit(*this);
    }
};

// If Statement Node with condition, then branch, and optional else branch
class IfNode : public ASTNode {
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> thenBranch;
    std::unique_ptr<ASTNode> elseBranch;

public:
    IfNode(std::unique_ptr<ASTNode> cond, std::unique_ptr<ASTNode> thenBr, std::unique_ptr<ASTNode> elseBr = nullptr)
        : condition(std::move(cond)), thenBranch(std::move(thenBr)), elseBranch(std::move(elseBr)) {}

    void generateCode() const override {
        std::cout << "if (";
        condition->generateCode();
        std::cout << ") ";
        thenBranch->generateCode();
        if (elseBranch) {
            std::cout << " else ";
            elseBranch->generateCode();
        }
    }

    void accept(IDEPlugin& plugin) const override {
        plugin.visit(*this);
    }
};

// IDE Plugin interface for interacting with AST nodes
class IDEPlugin {
public:
    virtual ~IDEPlugin() = default;
    virtual void visit(const ExpressionNode& node) = 0;
    virtual void visit(const IfNode& node) = 0;
};

// Example IDE Plugin with injector functions
class MyIDEPlugin : public IDEPlugin {
public:
    void visit(const ExpressionNode& node) override {
        // Example: Implement syntax highlighting and code completion
        std::cout << "Highlighting expression: " << node.generateCode() << std::endl;
    }

    void visit(const IfNode& node) override {
        // Example: Implement error checking and suggestion
        std::cout << "Checking if statement: ";
        node.generateCode();
        std::cout << std::endl;
    }
};

// Example usage
int main() {
    std::unique_ptr<ASTNode> ifStatement = std::make_unique<IfNode>(
        std::make_unique<ExpressionNode>("a + b"),
        std::make_unique<ExpressionNode>("printHelloWorld"),
        std::make_unique<ExpressionNode>("printGoodbyeWorld")
    );

    // Use the IDE plugin to interact with AST nodes
    MyIDEPlugin idePlugin;
    ifStatement->accept(idePlugin);

    return 0;
}

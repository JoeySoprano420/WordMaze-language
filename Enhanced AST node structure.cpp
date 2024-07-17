#include <iostream>
#include <string>
#include <memory>  // For std::unique_ptr

// Abstract base class for AST nodes
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void generateCode() const = 0;
    virtual void optimize() {}
    virtual void accept(Visitor& visitor) const = 0;  // Visitor pattern for future expansions
};

// Expression Node with dynamic value
class ExpressionNode : public ASTNode {
    std::string value;

public:
    ExpressionNode(std::string val) : value(std::move(val)) {}

    void generateCode() const override {
        std::cout << value;
    }

    void accept(Visitor& visitor) const override {
        visitor.visit(*this);
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

    void optimize() override {
        // Perform optimization on the condition and branches if needed
        condition->optimize();
        thenBranch->optimize();
        if (elseBranch) {
            elseBranch->optimize();
        }
    }

    void accept(Visitor& visitor) const override {
        visitor.visit(*this);
    }
};

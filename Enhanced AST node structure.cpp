#include <iostream>
#include <string>
#include <memory>  // For std::unique_ptr

// Forward declaration of Visitor interface
class Visitor;

// Abstract base class for AST nodes
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void generateCode() const = 0;
    virtual void optimize() {}
    virtual void accept(Visitor& visitor) const = 0;
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
        // Example: Constant folding optimization
        // This is just a basic example, real optimization would be more complex
        if (auto* exprNode = dynamic_cast<ExpressionNode*>(condition.get())) {
            // Example: Replace constant expressions with their evaluated values
            if (exprNode->value == "true") {
                condition.reset(new ExpressionNode("1"));
            } else if (exprNode->value == "false") {
                condition.reset(new ExpressionNode("0"));
            }
        }
    }

    void accept(Visitor& visitor) const override {
        visitor.visit(*this);
    }
};

// Visitor interface for visiting AST nodes
class Visitor {
public:
    virtual ~Visitor() = default;
    virtual void visit(const ExpressionNode& node) = 0;
    virtual void visit(const IfNode& node) = 0;
};

// Example of a Concrete Visitor implementing optimization
class OptimizingVisitor : public Visitor {
public:
    void visit(const ExpressionNode& node) override {
        // No optimization for ExpressionNode in this example
    }

    void visit(const IfNode& node) override {
        // Optimize the IfNode and its branches
        node.optimize();
    }
};

// Example usage
int main() {
    std::unique_ptr<ASTNode> ifStatement = std::make_unique<IfNode>(
        std::make_unique<ExpressionNode>("true"),
        std::make_unique<ExpressionNode>("cout << \"Hello World\";"),
        std::make_unique<ExpressionNode>("cout << \"Goodbye World\";")
    );

    // Use the optimizing visitor to perform optimizations
    OptimizingVisitor optimizer;
    ifStatement->accept(optimizer);

    // Generate optimized code
    ifStatement->generateCode();

    return 0;
}

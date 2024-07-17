// Example: C++ Code Generation for a Simple AST Node
#include <iostream>
#include <string>

class ASTNode {
public:
    virtual void generateCode() = 0;
};

class ExpressionNode : public ASTNode {
    std::string value;
public:
    ExpressionNode(std::string val) : value(val) {}
    void generateCode() override {
        std::cout << value;
    }
};

class IfNode : public ASTNode {
    ASTNode* condition;
    ASTNode* thenBranch;
    ASTNode* elseBranch;
public:
    IfNode(ASTNode* cond, ASTNode* thenBr, ASTNode* elseBr = nullptr)
        : condition(cond), thenBranch(thenBr), elseBranch(elseBr) {}
    void generateCode() override {
        std::cout << "if (";
        condition->generateCode();
        std::cout << ") ";
        thenBranch->generateCode();
        if (elseBranch) {
            std::cout << " else ";
            elseBranch->generateCode();
        }
    }
};

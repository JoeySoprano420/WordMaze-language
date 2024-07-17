class ASTNode:
    def accept(self, visitor):
        raise NotImplementedError("Accept method not implemented in base class")

class ExpressionNode(ASTNode):
    def __init__(self, value):
        self.value = value

    def accept(self, visitor):
        return visitor.visit_expression_node(self)

class BinaryOpNode(ExpressionNode):
    def __init__(self, left, operator, right):
        self.left = left
        self.operator = operator
        self.right = right

    def accept(self, visitor):
        return visitor.visit_binary_op_node(self)

class UnaryOpNode(ExpressionNode):
    def __init__(self, operator, operand):
        self.operator = operator
        self.operand = operand

    def accept(self, visitor):
        return visitor.visit_unary_op_node(self)

class IfNode(ASTNode):
    def __init__(self, condition, then_branch, else_branch=None):
        self.condition = condition
        self.then_branch = then_branch
        self.else_branch = else_branch

    def accept(self, visitor):
        return visitor.visit_if_node(self)

class WhileNode(ASTNode):
    def __init__(self, condition, body):
        self.condition = condition
        self.body = body

    def accept(self, visitor):
        return visitor.visit_while_node(self)

class ForNode(ASTNode):
    def __init__(self, init, condition, increment, body):
        self.init = init
        self.condition = condition
        self.increment = increment
        self.body = body

    def accept(self, visitor):
        return visitor.visit_for_node(self)

class ReturnNode(ASTNode):
    def __init__(self, value=None):
        self.value = value

    def accept(self, visitor):
        return visitor.visit_return_node(self)

class VariableDeclarationNode(ASTNode):
    def __init__(self, var_type, identifier, initializer=None):
        self.var_type = var_type
        self.identifier = identifier
        self.initializer = initializer

    def accept(self, visitor):
        return visitor.visit_variable_declaration_node(self)

class FunctionDeclarationNode(ASTNode):
    def __init__(self, return_type, name, parameters, body):
        self.return_type = return_type
        self.name = name
        self.parameters = parameters
        self.body = body

    def accept(self, visitor):
        return visitor.visit_function_declaration_node(self)

class ParameterNode(ASTNode):
    def __init__(self, var_type, name):
        self.var_type = var_type
        self.name = name

    def accept(self, visitor):
        return visitor.visit_parameter_node(self)

class FunctionCallNode(ASTNode):
    def __init__(self, callee, arguments):
        self.callee = callee
        self.arguments = arguments

    def accept(self, visitor):
        return visitor.visit_function_call_node(self)

class ClassDeclarationNode(ASTNode):
    def __init__(self, name, members):
        self.name = name
        self.members = members

    def accept(self, visitor):
        return visitor.visit_class_declaration_node(self)

class MethodDeclarationNode(ASTNode):
    def __init__(self, return_type, name, parameters, body):
        self.return_type = return_type
        self.name = name
        self.parameters = parameters
        self.body = body

    def accept(self, visitor):
        return visitor.visit_method_declaration_node(self)

class FieldDeclarationNode(ASTNode):
    def __init__(self, var_type, name, initializer=None):
        self.var_type = var_type
        self.name = name
        self.initializer = initializer

    def accept(self, visitor):
        return visitor.visit_field_declaration_node(self)

class AssignmentNode(ASTNode):
    def __init__(self, target, value):
        self.target = target
        self.value = value

    def accept(self, visitor):
        return visitor.visit_assignment_node(self)

class BlockNode(ASTNode):
    def __init__(self, statements):
        self.statements = statements

    def accept(self, visitor):
        return visitor.visit_block_node(self)

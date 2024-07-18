class IfNode:
    def __init__(self, condition, then_body, else_body):
        self.condition = condition
        self.then_body = then_body
        self.else_body = else_body

class WhileNode:
    def __init__(self, condition, body):
        self.condition = condition
        self.body = body

class FunctionDefinitionNode:
    def __init__(self, function_name, parameters, body):
        self.function_name = function_name
        self.parameters = parameters
        self.body = body

class PrintNode:
    def __init__(self, expression):
        self.expression = expression

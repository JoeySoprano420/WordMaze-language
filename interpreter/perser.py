class Parser:
    def __init__(self, lexer):
        self.lexer = lexer
        self.current_token = self.lexer.get_next_token()

    def error(self):
        raise Exception(f'Invalid syntax with token {self.current_token.type} ({self.current_token.value})')

    def eat(self, token_type):
        if self.current_token.type == token_type:
            self.current_token = self.lexer.get_next_token()
        else:
            self.error()

    def parse(self):
        return self.parse_statements()

    def parse_statements(self):
        statements = []
        while self.current_token.type != 'EOF':
            statements.append(self.parse_statement())
        return statements

    def parse_statement(self):
        token = self.current_token

        if token.type == 'LET':
            return self.parse_assignment()

        elif token.type == 'IF':
            return self.parse_if()

        elif token.type == 'WHILE':
            return self.parse_while()

        elif token.type == 'FOR':
            return self.parse_for()

        elif token.type == 'PRINT':
            return self.parse_print()

        elif token.type == 'TRY':
            return self.parse_try_catch()

        elif token.type == 'FUNCTION':
            return self.parse_function_definition()

        elif token.type == 'RETURN':
            return self.parse_return()

        else:
            self.error()

    def parse_assignment(self):
        self.eat('LET')
        variable_name = self.current_token.value
        self.eat('WORD')
        self.eat('EQUALS')
        expression = self.parse_expression()
        self.eat('SEMICOLON')
        return BinaryOpNode(VariableNode(variable_name), '=', expression)

    def parse_expression(self):
        return self.parse_logical_or()

    def parse_logical_or(self):
        node = self.parse_logical_and()

        while self.current_token.type == 'OR':
            token = self.current_token
            self.eat('OR')
            node = BinaryOpNode(node, token.value, self.parse_logical_and())

        return node

    def parse_logical_and(self):
        node = self.parse_equality()

        while self.current_token.type == 'AND':
            token = self.current_token
            self.eat('AND')
            node = BinaryOpNode(node, token.value, self.parse_equality())

        return node

    def parse_equality(self):
        node = self.parse_comparison()

        while self.current_token.type in ['EQUALS_EQUALS', 'NOT_EQUALS']:
            token = self.current_token
            if token.type == 'EQUALS_EQUALS':
                self.eat('EQUALS_EQUALS')
            elif token.type == 'NOT_EQUALS':
                self.eat('NOT_EQUALS')
            node = BinaryOpNode(node, token.value, self.parse_comparison())

        return node

    def parse_comparison(self):
        node = self.parse_term()

        while self.current_token.type in ['LESS_THAN', 'GREATER_THAN']:
            token = self.current_token
            if token.type == 'LESS_THAN':
                self.eat('LESS_THAN')
            elif token.type == 'GREATER_THAN':
                self.eat('GREATER_THAN')
            node = BinaryOpNode(node, token.value, self.parse_term())

        return node

    def parse_term(self):
        node = self.parse_factor()

        while self.current_token.type in ['PLUS', 'MINUS']:
            token = self.current_token
            if token.type == 'PLUS':
                self.eat('PLUS')
            elif token.type == 'MINUS':
                self.eat('MINUS')
            node = BinaryOpNode(node, token.value, self.parse_factor())

        return node

    def parse_factor(self):
        node = self.parse_primary()

        while self.current_token.type in ['MULTIPLY', 'DIVIDE']:
            token = self.current_token
            if token.type == 'MULTIPLY':
                self.eat('MULTIPLY')
            elif token.type == 'DIVIDE':
                self.eat('DIVIDE')
            node = BinaryOpNode(node, token.value, self.parse_primary())

        return node

    def parse_primary(self):
        token = self.current_token

        if token.type == 'NUMBER':
            self.eat('NUMBER')
            return LiteralNode(token.value)

        elif token.type == 'WORD':
            identifier = token.value
            if self.peek_token().type == 'LPAREN':
                return self.parse_function_call(identifier)
            elif self.peek_token().type == 'LBRACKET':
                return self.parse_array_access(identifier)
            else:
                self.eat('WORD')
                return VariableNode(identifier)

        elif token.type == 'MINUS':
            self.eat('MINUS')
            return UnaryOpNode('-', self.parse_primary())

        elif token.type == 'LPAREN':
            self.eat('LPAREN')
            node = self.parse_expression()
            self.eat('RPAREN')
            return node

        else:
            self.error()

    def parse_function_call(self, identifier):
        self.eat('WORD')
        self.eat('LPAREN')
        arguments = []
        if self.current_token.type != 'RPAREN':
            arguments.append(self.parse_expression())
            while self.current_token.type == 'COMMA':
                self.eat('COMMA')
                arguments.append(self.parse_expression())
        self.eat('RPAREN')
        return FunctionCallNode(identifier, arguments)

    def parse_array_access(self, identifier):
        self.eat('WORD')
        self.eat('LBRACKET')
       
    def parse_array_access(self, identifier):
        self.eat('WORD')
        self.eat('LBRACKET')
        index = self.parse_expression()
        self.eat('RBRACKET')
        return ArrayAccessNode(identifier, index)
    def parse_for(self):
        self.eat('FOR')
        self.eat('LPAREN')
        init_statement = self.parse_assignment()  # Optional initialization
        self.eat('SEMICOLON')
        condition = self.parse_expression()  # Loop condition
        self.eat('SEMICOLON')
        increment_statement = self.parse_assignment()  # Increment operation
        self.eat('RPAREN')
        self.eat('LCURLY')
        body = self.parse_statements()
        self.eat('RCURLY')
        return ForNode(init_statement, condition, increment_statement, body)

    def parse_try_catch(self):
        self.eat('TRY')
        self.eat('LCURLY')
        try_statements = self.parse_statements()
        self.eat('RCURLY')
        self.eat('CATCH')
        self.eat('LCURLY')
        catch_statements = self.parse_statements()
        self.eat('RCURLY')
        return TryCatchNode(try_statements, catch_statements)

    def parse_return(self):
        self.eat('RETURN')
        expr = self.parse_expression()
        self.eat('SEMICOLON')
        return ReturnNode(expr)
class Interpreter:
    def __init__(self, parser):
        self.parser = parser
        self.global_scope = {}
        self.local_scope = None

    def interpret(self):
        tree = self.parser.parse()
        return self.visit(tree)

    def visit(self, node):
        method_name = 'visit_' + type(node).__name__
        visitor = getattr(self, method_name, self.generic_visit)
        if callable(visitor):
            return visitor(node)
        else:
            raise Exception(f'No visit_{type(node).__name__} method')

    def generic_visit(self, node):
        raise Exception(f'No visit_{type(node).__name__} method')

    def visit_ProgramNode(self, node):
        for statement in node.statements:
            self.visit(statement)

    def visit_AssignmentNode(self, node):
        variable_name = node.variable_name
        value = self.visit(node.expression)
        if self.local_scope is not None:
            self.local_scope[variable_name] = value
        else:
            self.global_scope[variable_name] = value

    def visit_VariableNode(self, node):
        if self.local_scope is not None and node.variable_name in self.local_scope:
            return self.local_scope[node.variable_name]
        elif node.variable_name in self.global_scope:
            return self.global_scope[node.variable_name]
        else:
            raise Exception(f'Undefined variable {node.variable_name}')

    def visit_BinaryOpNode(self, node):
        left_value = self.visit(node.left)
        right_value = self.visit(node.right)
        if node.op == '+':
            return left_value + right_value
        elif node.op == '-':
            return left_value - right_value
        elif node.op == '*':
            return left_value * right_value
        elif node.op == '/':
            return left_value / right_value

    def visit_UnaryOpNode(self, node):
        value = self.visit(node.operand)
        if node.op == '-':
            return -value

    def visit_LiteralNode(self, node):
        return node.value

    def visit_FunctionCallNode(self, node):
        function_name = node.function_name
        if function_name in self.global_scope:
            function = self.global_scope[function_name]
            arguments = [self.visit(arg) for arg in node.arguments]
            return self.call_function(function, arguments)
        else:
            raise Exception(f'Undefined function {function_name}')

    def call_function(self, function, arguments):
        self.local_scope = {}
        for param, arg in zip(function.parameters, arguments):
            self.local_scope[param] = arg
        result = None
        try:
            for statement in function.body:
                result = self.visit(statement)
                if isinstance(result, ReturnNode):
                    result = self.visit(result.expression)
                    break
        finally:
            self.local_scope = None
        return result

    def visit_ForNode(self, node):
        self.visit(node.init_statement)
        while self.visit(node.condition):
            for statement in node.body:
                self.visit(statement)
            self.visit(node.increment_statement)

    def visit_TryCatchNode(self, node):
        try:
            for statement in node.try_statements:
                self.visit(statement)
        except Exception as e:
            for statement in node.catch_statements:
                self.visit(statement)

    def visit_ReturnNode(self, node):
        return node.expression

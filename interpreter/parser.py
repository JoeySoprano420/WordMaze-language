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
        return ProgramNode(self.parse_statements())

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
        return AssignmentNode(variable_name, expression)

    def parse_if(self):
        self.eat('IF')
        self.eat('LPAREN')
        condition = self.parse_expression()
        self.eat('RPAREN')
        self.eat('LBRACE')
        then_body = self.parse_statements()
        self.eat('RBRACE')

        else_body = []
        if self.current_token.type == 'ELSE':
            self.eat('ELSE')
            self.eat('LBRACE')
            else_body = self.parse_statements()
            self.eat('RBRACE')

        return IfNode(condition, then_body, else_body)

    def parse_while(self):
        self.eat('WHILE')
        self.eat('LPAREN')
        condition = self.parse_expression()
        self.eat('RPAREN')
        self.eat('LBRACE')
        body = self.parse_statements()
        self.eat('RBRACE')
        return WhileNode(condition, body)

    def parse_function_definition(self):
        self.eat('FUNCTION')
        function_name = self.current_token.value
        self.eat('WORD')
        self.eat('LPAREN')
        parameters = []
        if self.current_token.type != 'RPAREN':
            parameters.append(self.current_token.value)
            self.eat('WORD')
            while self.current_token.type == 'COMMA':
                self.eat('COMMA')
                parameters.append(self.current_token.value)
                self.eat('WORD')
        self.eat('RPAREN')
        self.eat('LBRACE')
        body = self.parse_statements()
        self.eat('RBRACE')
        return FunctionDefinitionNode(function_name, parameters, body)

    def parse_print(self):
        self.eat('PRINT')
        expression = self.parse_expression()
        self.eat('SEMICOLON')
        return PrintNode(expression)

    def parse_return(self):
        self.eat('RETURN')
        expression = self.parse_expression()
        self.eat('SEMICOLON')
        return ReturnNode(expression)

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
        index = self.parse_expression()
        self.eat('RBRACKET')
       

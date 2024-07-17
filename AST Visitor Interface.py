class ASTVisitor:
    def visit_expression_node(self, node):
        raise NotImplementedError

    def visit_binary_op_node(self, node):
        raise NotImplementedError

    def visit_unary_op_node(self, node):
        raise NotImplementedError

    def visit_if_node(self, node):
        raise NotImplementedError

    def visit_while_node(self, node):
        raise NotImplementedError

    def visit_for_node(self, node):
        raise NotImplementedError

    def visit_return_node(self, node):
        raise NotImplementedError

    def visit_variable_declaration_node(self, node):
        raise NotImplementedError

    def visit_function_declaration_node(self, node):
        raise NotImplementedError

    def visit_parameter_node(self, node):
        raise NotImplementedError

    def visit_function_call_node(self, node):
        raise NotImplementedError

    def visit_class_declaration_node(self, node):
        raise NotImplementedError

    def visit_method_declaration_node(self, node):
        raise NotImplementedError

    def visit_field_declaration_node(self, node):
        raise NotImplementedError

    def visit_assignment_node(self, node):
        raise NotImplementedError

    def visit_block_node(self, node):
        raise NotImplementedError

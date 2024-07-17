class CodeGeneratorVisitor(ASTVisitor):
    def visit_expression_node(self, node):
        return str(node.value)

    def visit_binary_op_node(self, node):
        left = node.left.accept(self)
        right = node.right.accept(self)
        return f"({left} {node.operator} {right})"

    def visit_unary_op_node(self, node):
        operand = node.operand.accept(self)
        return f"({node.operator}{operand})"

    def visit_if_node(self, node):
        condition = node.condition.accept(self)
        then_branch = node.then_branch.accept(self)
        if node.else_branch:
            else_branch = node.else_branch.accept(self)
            return f"if ({condition}) {{ {then_branch} }} else {{ {else_branch} }}"
        else:
            return f"if ({condition}) {{ {then_branch} }}"

    def visit_while_node(self, node):
        condition = node.condition.accept(self)
        body = node.body.accept(self)
        return f"while ({condition}) {{ {body} }}"

    def visit_for_node(self, node):
        init = node.init.accept(self)
        condition = node.condition.accept(self)
        increment = node.increment.accept(self)
        body = node.body.accept(self)
        return f"for ({init}; {condition}; {increment}) {{ {body} }}"

    def visit_return_node(self, node):
        if node.value:
            value = node.value.accept(self)
            return f"return {value};"
        else:
            return "return;"

    def visit_variable_declaration_node(self, node):
        if node.initializer:
            initializer = node.initializer.accept(self)
            return f"{node.var_type} {node.identifier} = {initializer};"
        else:
            return f"{node.var_type} {node.identifier};"

    def visit_function_declaration_node(self, node):
        parameters = ", ".join([param.accept(self) for param in node.parameters])
        body = node.body.accept(self)
        return f"{node.return_type} {node.name}({parameters}) {{ {body} }}"

    def visit_parameter_node(self, node):
        return f"{node.var_type} {node.name}"

    def visit_function_call_node(self, node):
        arguments = ", ".join([arg.accept(self) for arg in node.arguments])
        return f"{node.callee}({arguments})"

    def visit_class_declaration_node(self, node):
        members = "\n".join([member.accept(self) for member in node.members])
        return f"class {node.name} {{ {members} }}"

    def visit_method_declaration_node(self, node):
        parameters = ", ".join([param.accept(self) for param in node.parameters])
        body = node.body.accept(self)
        return f"{node.return_type} {node.name}({parameters}) {{ {body} }}"

    def visit_field_declaration_node(self, node):
        if node.initializer:
            initializer = node.initializer.accept(self)
            return f"{node.var_type} {node.name} = {initializer};"
        else:
            return f"{node.var_type} {node.name};"

    def visit_assignment_node(self, node):
        value = node.value.accept(self)
        return f"{node.target} = {value};"

    def visit_block_node(self, node):
        statements = "\n".join([stmt.accept(self) for stmt in node.statements])
        return f"{{ {statements} }}"

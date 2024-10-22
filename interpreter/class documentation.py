class Parser:
    """
    The Parser class is responsible for parsing the tokens provided by the Lexer
    and constructing an abstract syntax tree (AST) that represents the structure
    of the WordMaze program.

    Attributes:
        lexer (Lexer): The lexer that provides tokens for parsing.
        current_token (Token): The current token being parsed.

    Methods:
        parse(): Starts the parsing process and returns the AST.
        parse_statements(): Parses a series of statements.
        parse_statement(): Parses a single statement.
        parse_assignment(): Parses an assignment statement.
        parse_expression(): Parses an expression.
        parse_logical_or(): Parses logical OR expressions.
        parse_logical_and(): Parses logical AND expressions.
        parse_equality(): Parses equality expressions.
        parse_comparison(): Parses comparison expressions.
        parse_term(): Parses term expressions (addition and subtraction).
        parse_factor(): Parses factor expressions (multiplication and division).
        parse_primary(): Parses primary expressions (numbers, variables, etc.).
        parse_function_call(identifier): Parses a function call.
        parse_array_access(identifier): Parses an array access.
    """

class Interpreter:
    """
    The Interpreter class is responsible for executing the abstract syntax tree (AST)
    generated by the Parser. It evaluates expressions and executes statements.

    Attributes:
        parser (Parser): The parser that provides the AST.
        global_scope (dict): The global variable scope.
        local_scope (dict or None): The local variable scope for function execution.

    Methods:
        interpret(): Starts the interpretation process.
        visit(node): Visits a node in the AST and executes it.
        generic_visit(node): A generic visit method for unsupported nodes.
        visit_ProgramNode(node): Visits a program node and executes its statements.
        visit_AssignmentNode(node): Visits an assignment node and assigns a value to a variable.
        visit_VariableNode(node): Visits a variable node and retrieves its value.
        visit_BinaryOpNode(node): Visits a binary operation node and evaluates the operation.
        visit_UnaryOpNode(node): Visits a unary operation node and evaluates the operation.
        visit_LiteralNode(node): Visits a literal node and returns its value.
        visit_FunctionCallNode(node): Visits a function call node and executes the function.
        call_function(function, arguments): Calls a function with the given arguments.
        visit_ForNode(node): Visits a for loop node and executes its body.
        visit_TryCatchNode(node): Visits a try-catch node and executes its body with error handling.
        visit_ReturnNode(node): Visits a return node and returns its expression's value.
    """

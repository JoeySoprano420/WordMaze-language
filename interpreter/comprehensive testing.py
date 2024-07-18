import unittest

class TestParser(unittest.TestCase):
    def test_assignment(self):
        lexer = Lexer("let x = 5;")
        parser = Parser(lexer)
        result = parser.parse()
        self.assertEqual(result[0].variable_name, 'x')
        self.assertEqual(result[0].expression.value, 5)

    def test_arithmetic(self):
        lexer = Lexer("let x = 5 + 3 * 2;")
        parser = Parser(lexer)
        result = parser.parse()
        self.assertEqual(result[0].variable_name, 'x')
        self.assertEqual(result[0].expression.op, '+')
        self.assertEqual(result[0].expression.right.op, '*')

    def test_if_statement(self):
        lexer = Lexer("if (x < 5) { print(x); }")
        parser = Parser(lexer)
        result = parser.parse()
        self.assertEqual(result[0].condition.op, '<')
        self.assertEqual(result[0].body[0].value.variable_name, 'x')

if __name__ == '__main__':
    unittest.main()

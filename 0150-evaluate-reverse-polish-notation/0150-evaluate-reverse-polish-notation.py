class Solution(object):
    def evalRPN(self, tokens):
        """
        :type tokens: List[str]
        :rtype: int
        """
        operations = ["+","-","/","*"]
        def doOperation(operation,num1,num2):
            if operation == "+":
                return num1 + num2
            if operation == "-":
                return num1 - num2
            if operation == "/":
                return int(num1 / float(num2))
            if operation == "*":
                return num1 * num2

        stack = []
        for token in tokens:
            if token in operations:
                num2 = stack.pop()
                num1 = stack.pop()
                stack.append(doOperation(token,num1,num2))
            else:
                stack.append(int(token))
        return stack.pop()

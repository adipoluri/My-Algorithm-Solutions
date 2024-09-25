class Solution(object):
    def backspaceCompare(self, s, t):
        """
        :type s: str
        :type t: str
        :rtype: bool
        """
        stack1 = []
        stack2 = []

        for letter in s:
            if letter == "#":
                if len(stack1) != 0:
                    stack1.pop()
            else:
                stack1.append(letter)
        
        for letter in t:
            if letter == "#":
                if len(stack2) != 0:
                    stack2.pop()
            else:
                stack2.append(letter)


        return stack1 == stack2
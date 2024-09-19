class Solution(object):
    def isValid(self, s):
        """
        :type s: str
        :rtype: bool
        """
        stack = []
        options = {"(":")","{":"}","[":"]"}
        close = {"}","]",")"}
        for letter in s:
            if letter in options:
                stack.append(letter)
            if letter in close:
                if len(stack) == 0:
                    return False
                a = stack.pop()
                if options[a] != letter:
                    return False
        if len(stack) != 0:
            return False    
        return True


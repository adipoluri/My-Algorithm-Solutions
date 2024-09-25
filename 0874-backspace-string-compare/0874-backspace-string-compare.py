class Solution(object):
    def backspaceCompare(self, s, t):
        """
        :type s: str
        :type t: str
        :rtype: bool
        """
        return self.answer2(s,t)

    def answer2(self,s,t):
        i,j = len(s)-1,len(t)-1
        iBackspaces, jBackspaces = 0,0

        while i >= 0 or j >= 0:
            # Get index of next valid character
            while i >= 0:
                if s[i] == "#":
                    iBackspaces += 1
                    i -= 1
                elif iBackspaces > 0:
                    i -= 1
                    iBackspaces -= 1
                else:
                    break

            while j >= 0:
                if t[j] == "#":
                    jBackspaces += 1
                    j -= 1
                elif jBackspaces > 0:
                    j -= 1
                    jBackspaces -= 1
                else:
                    break

            print(s[i],t[j])
            if i>= 0 and j>= 0 and s[i] != t[j]:
                return False
            print(i,j)
            if (i >= 0) != (j >= 0):
                return False

            i -= 1
            j -= 1

        return True

    def answer1(self,s,t):
        # Time: O(M+N)
        # Space: O(M+N)
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
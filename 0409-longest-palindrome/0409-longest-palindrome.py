class Solution(object):
    def longestPalindrome(self, s):
        """
        :type s: str
        :rtype: int
        """

        abc = {}
        retVal = 0
        for letter in s:
            abc[letter] = abc.get(letter, 0) + 1

            if abc.get(letter,-1) == 2:
                abc.pop(letter)
                retVal += 2

        return retVal + 1 if 1 in abc.values() else retVal
        
import re
class Solution(object):
    def isPalindrome(self, s):
        """
        :type s: str
        :rtype: bool
        """
        s2 = list(re.sub('[^A-Za-z0-9]+', '', s).lower())
        low = 0
        high = len(s2)-1
        while high > low:
            if s2[high] != s2[low]:
                return False
            high -= 1
            low += 1
            print(low,high)
            
        return True

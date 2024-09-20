class Solution(object):
    def isAnagram(self, s, t):
        """
        :type s: str
        :type t: str
        :rtype: bool
        """
        abc = {}
        if(len(s) != len(t)):
            return False
            
        for letter1,letter2 in zip(s,t):
            abc[letter1] = abc.get(letter1, 0) + 1
            abc[letter2] = abc.get(letter2, 0) - 1

            if abc.get(letter1,-1) == 0:
                abc.pop(letter1)

            if abc.get(letter2,-1) == 0:
                abc.pop(letter2)
            

        return abc == {}
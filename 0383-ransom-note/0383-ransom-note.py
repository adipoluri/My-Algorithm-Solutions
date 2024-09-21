class Solution(object):
    def canConstruct(self, ransomNote, magazine):
        """
        :type ransomNote: str
        :type magazine: str
        :rtype: bool
        """
        abc = {}
        for letter in ransomNote:
            abc[letter] = abc.get(letter, 0) + 1

        for letter in magazine:
            if abc.get(letter, -1) != -1:
                abc[letter] -= 1

            if abc.get(letter,-1) == 0:
                abc.pop(letter)
    
        return abc == {}
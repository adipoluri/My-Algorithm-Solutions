class Solution(object):
    def longestCommonPrefix(self, strs):
        """
        :type strs: List[str]
        :rtype: str
        """
        return self.sol2(strs)

    def sol1(self, strs):
        retVal = ""
        i = 0
    
        while i < 200:
            if i >= len(strs[0]):
                return retVal

            curr = strs[0][i]
            for word in strs:
                if i >= len(word):
                    return retVal
                if word[i] != curr:
                    return retVal
            retVal += curr
            i += 1
        return retVal
                
    def sol2(self, strs):
        if strs == None or len(strs) == 0:
            return ""
        
        for i in range(len(strs[0])):
            curr = strs[0][i]
            for word in strs:
                if i == len(word) or word[i] != curr:
                    return word[0:i]
        return strs[0]
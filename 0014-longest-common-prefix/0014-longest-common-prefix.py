class Solution(object):
    def longestCommonPrefix(self, strs):
        """
        :type strs: List[str]
        :rtype: str
        """
        strs.sort()
        retVal = ""
        i = 0
        
        while i < len(strs[0]):
            curr = strs[0][i]
            for word in strs:
                if i >= len(word):
                    return retVal
                if word[i] != curr:
                    return retVal
            retVal += curr
            i += 1
        return retVal
                
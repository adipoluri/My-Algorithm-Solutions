class Solution(object):
    def countBits(self, n):
        """
        :type n: int
        :rtype: List[int]
        """
        if n == 0:
            return [0]
            
        retVal = [0,1]
        curr = 1
        i = 2
        while i < n+1:
            if (i & (i - 1)) == 0:
                curr = i
            retVal.append(retVal[i-curr]+1)
            i += 1

        return retVal
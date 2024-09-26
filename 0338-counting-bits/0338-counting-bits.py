class Solution(object):
    def countBits(self, n):
        """
        :type n: int
        :rtype: List[int]
        """
        return self.sol2(n)


    def sol1(self,n):
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

    def sol2(self,n):
        retVal = [0]*(n+1)
        for i in range(1,n+1):
            retVal[i]=retVal[i & (i - 1)]+1

        return retVal
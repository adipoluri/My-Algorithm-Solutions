class Solution(object):
    def hammingWeight(self, n):
        """
        :type n: int
        :rtype: int
        """
        count = 0
        retVal = n
        while retVal != 0:
            count += 1
            retVal = retVal & (retVal - 1)

        return count
class Solution(object):
    def climbStairs(self, n):
        """
        :type n: int
        :rtype: int
        """
        if n <= 3:
            return n

        memo = [0, 1, 2, 3]
        for i in range(4,n+1):
            memo.append(memo[i-1] + memo[i-2])
        
        return memo[n]
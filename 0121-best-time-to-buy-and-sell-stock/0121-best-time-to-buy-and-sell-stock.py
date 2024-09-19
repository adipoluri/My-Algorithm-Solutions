class Solution(object):
    def maxProfit(self, prices):
        """
        :type prices: List[int]
        :rtype: int
        """
        min = 10000000
        max = -1
        profit = 0
        for price in prices:
            if price < min:
                min = price
                max = -1
            elif price - min > profit:
                max = price
                profit = max - min

        return profit

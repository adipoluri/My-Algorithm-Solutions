class Solution(object):
    # def maxProfit(self, prices):
    #     """
    #     :type prices: List[int]
    #     :rtype: int
    #     """
    #     buy = prices[0]
    #     profit = 0
    #     for price in prices:
    #         if price < buy:
    #             buy = price
    #         elif price - buy > profit:
    #             profit = price - buy

    #     return profit
    def maxProfit(self, prices):
        buy = prices[0]
        profit = 0
        for i in range(1, len(prices)):
            if prices[i] < buy:
                buy = prices[i]
            elif prices[i] - buy > profit:
                profit = prices[i] - buy
        return profit

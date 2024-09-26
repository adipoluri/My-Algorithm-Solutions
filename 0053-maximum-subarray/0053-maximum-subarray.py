class Solution(object):
    def maxSubArray(self, nums):
        """
        :type nums: List[int]
        :rtype: int
        """

        maxSum = -1000000
        currSum = 0
        for num in nums:
            currSum += num
            if currSum > maxSum:
                maxSum = currSum
            if currSum < 0:
                currSum = 0

        return maxSum

    
    def bruteForce(self, nums):
        if len(nums) == 1:
            return nums[0]

        maxSum = -1000000
        for i in range(0,len(nums)):
            currSum = 0
            for j in range(i,len(nums)):
                currSum += nums[j]
                maxSum = max(currSum,maxSum)
                

        return maxSum
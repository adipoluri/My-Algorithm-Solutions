class Solution(object):
    def twoSum(self, nums, target):
        """
        :type nums: List[int]
        :type target: int
        :rtype: List[int]
        """
        pair = {}
        count = 0
        for num in nums:
            if (target-num) in pair:
                return [pair[target-num],count]
            else:
                pair[num] = count
                count += 1

        return [-1,-1]
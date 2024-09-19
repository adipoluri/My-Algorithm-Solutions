class Solution(object):
    def twoSum(self, nums, target):
        """
        :type nums: List[int]
        :type target: int
        :rtype: List[int]
        """
        diffs = {}
        for i in range(0,len(nums)):
            diff = target - nums[i]            
            if diff in diffs.keys():
                if diffs[diff] != i:
                    return [diffs[diff],i]
            
            diffs[nums[i]] = i

        return[-1,-1]
            
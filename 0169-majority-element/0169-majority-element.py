class Solution(object):
    def majorityElement(self, nums):
        """
        :type nums: List[int]
        :rtype: int
        """
        if nums is None:
            return -1
        
        counter = 0
        majority = 0
        for num in nums:
            if counter == 0:
                majority = num
            
            if num == majority:
                counter += 1
            else:
                counter -= 1

        return majority
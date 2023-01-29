class Solution(object):
    def containsDuplicate(self, nums):
        """
        :type nums: List[int]
        :rtype: bool
        """
        mem = {}
        for num in nums:
            if num in mem:
                return True
            else:
                mem[num] = 1
            
        return False
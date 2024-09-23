class Solution(object):
    def containsDuplicate(self, nums):
        """
        :type nums: List[int]
        :rtype: bool
        """
        counts = {}
        for num in nums:
            if counts.get(num,-1) != -1:
                return True
            else:
                counts[num] = 1

        return False
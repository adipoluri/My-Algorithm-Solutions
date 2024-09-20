class Solution(object):
    def search(self, nums, target):
        """
        :type nums: List[int]
        :type target: int
        :rtype: int
        """
        high = len(nums)-1
        low = 0
        center = low+int((low+high)/2)

        while low <= high:
            center = int(floor((low+high)/2))
 
            if target == nums[center]:
                return center
                
            if target > nums[center]:
                low = center + 1
            else:
                high = center - 1

        return -1 if nums[center] != target else center


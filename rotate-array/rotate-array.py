class Solution(object):
    def rev(self,nums,s,e):
        start = s
        end = e
        while(start<end):
            nums[start], nums[end] = nums[end],nums[start]
            start += 1
            end -= 1
            
        
    def rotate(self, nums, k):
        """
        :type nums: List[int]
        :type k: int
        :rtype: None Do not return anything, modify nums in-place instead.
        """
        n = len(nums)
        k %= n
        self.rev(nums,0,n-1)
        self.rev(nums,0,k-1)
        self.rev(nums,k,n-1)

class Solution(object):
    def productExceptSelf(self, nums):
        """
        :type nums: List[int]
        :rtype: List[int]
        """
        L = [1]
        R = [1]
        for i in range(1,len(nums)):
            L.append(nums[i-1] * L[i-1])
            dist = len(nums)-1-i
            R.insert(0,nums[dist+1] * R[0])
        
        retVal = []
        for i in range(0,len(nums)):
            retVal.append(L[i]*R[i])

        return retVal
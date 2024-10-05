class Solution(object):
    def productExceptSelf(self, nums):
        """
        :type nums: List[int]
        :rtype: List[int]
        """
        length = len(nums)
        L,R = [0]*length,[0]*length

        L[0] = 1
        for i in range(1,length):
            L[i] = (nums[i-1] * L[i-1])

        R[length-1] = 1
        for i in reversed(range(0,length-1)):
            R[i] = nums[i+1] * R[i+1]
            print(i)

        res = []
        for i in range(0,length):
            res.append(L[i]*R[i])

        return res
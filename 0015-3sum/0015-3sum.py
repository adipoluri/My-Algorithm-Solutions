class Solution(object):
    def threeSum(self, nums):
        """
        :type nums: List[int]
        :rtype: List[List[int]]
        """
        retVal = []
        nums.sort()
        print(nums)
        
        prev = None
        for i in range(0,len(nums)):
            if nums[i] > 0:
                break
            
            if nums[i] != prev:
                low = i+1
                high = len(nums)-1
                while low < high:
                    sum = nums[i] + nums[low] + nums[high]
                    if sum < 0:
                        low += 1
                    elif sum > 0:
                        high -= 1
                    else:
                        retVal.append([nums[i],nums[low],nums[high]])

                        high -= 1
                        low += 1
                        prev = nums[low]
                        while low < high and nums[low-1] == nums[low]:
                            low += 1


            prev = nums[i]
        return retVal
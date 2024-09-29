class Solution(object):
    def twoSum(self, numbers, target):
        """
        :type numbers: List[int]
        :type target: int
        :rtype: List[int]
        """
        
        low = 0
        high = len(numbers)-1
        while low < high:
            if numbers[low] + numbers[high] == target:
                return [low+1,high+1]

            if numbers[low] + numbers[high] > target:
                high -= 1
            elif numbers[low] + numbers[high] < target:
                low += 1

        return [-1,-1]
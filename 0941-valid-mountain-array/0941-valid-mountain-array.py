class Solution(object):
    def validMountainArray(self, arr):
        if len(arr) < 3:
            return False
        
        maxNum = max(arr)
        maxNumIndex = arr.index(maxNum, 0, len(arr))
        
        if (maxNumIndex == len(arr) - 1) or (maxNumIndex == 0): 
            return False
        
        for x in range(len(arr)):
            if x != 0:
                if x < maxNumIndex:
                    if arr[x-1] >= arr[x]:
                        return False
                elif x > maxNumIndex:
                    if arr[x - 1] <= arr[x]:
                        return False
        return True
                
                    
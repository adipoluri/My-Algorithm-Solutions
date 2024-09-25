class Solution(object):
    def insert(self, intervals, newInterval):
        """
        :type intervals: List[List[int]]
        :type newInterval: List[int]
        :rtype: List[List[int]]
        """
        
        if len(intervals) == 0:
            return [newInterval]

        high = len(intervals)-1
        low = 0

        while low <= high:
            mid = int((low+high)/2)
            if(intervals[mid][0] < newInterval[0]):
                low = mid + 1
            else:
                high = mid - 1
        
        intervals.insert(int((low+high)/2)+1, newInterval)

        minI = intervals[0][0]
        maxI = intervals[0][1]
        retVal = []
        for interval in intervals:
            if interval[0] <= maxI:
                minI = min(minI,interval[0])
                maxI = max(maxI,interval[1])
            else:
                retVal.append([minI,maxI])
                minI = interval[0]
                maxI = interval[1]
    
        retVal.append([minI,maxI])
        return retVal
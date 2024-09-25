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

        res = []
        for interval in intervals:
            if not res or interval[0] > res[-1][1]:
                res.append(interval)
            else:
                res[-1][1] = max(res[-1][1], interval[1])
    
        return res
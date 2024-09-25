class Solution(object):
    def canAttendMeetings(self, intervals):
        """
        :type intervals: List[List[int]]
        :rtype: bool
        """
        intervals.sort()

        for i in range(0,len(intervals)-1):
            if intervals[i][1] > intervals[i+1][0]:
                return False
        return True
        
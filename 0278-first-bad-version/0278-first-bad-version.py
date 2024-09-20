# The isBadVersion API is already defined for you.
# @param version, an integer
# @return a bool
# def isBadVersion(version):

class Solution(object):
    def firstBadVersion(self, n):
        """
        :type n: int
        :rtype: int
        """
        low, high = 1, n
        
        version = int((low+high)/2)
        while low <= high:
            version = int((low+high)/2)
            if isBadVersion(version):
                high = version - 1
            else:
                low = version + 1
        
        print(version)
        return version if isBadVersion(version) else version + 1


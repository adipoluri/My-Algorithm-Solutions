class Solution(object):
    def lengthOfLongestSubstring(self, s):
        """
        :type s: str
        :rtype: int
        """
        maxSubstring = 0
        currSubstring = 0
        curr = 0
        count = {}
        
        while curr < len(s):
            if count.get(s[curr],-1) == -1:
                count[s[curr]] = curr + 1
                currSubstring += 1
                curr += 1
            else:
                currSubstring = 0
                curr = count[s[curr]]
                count = {}
            maxSubstring = max(maxSubstring, currSubstring)

        return maxSubstring
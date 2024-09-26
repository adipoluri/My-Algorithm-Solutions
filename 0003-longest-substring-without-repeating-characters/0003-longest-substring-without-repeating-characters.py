class Solution(object):
    def lengthOfLongestSubstring(self, s):
        """
        :type s: str
        :rtype: int
        """
        maxSubstring = 0
        count = {}
        curr = 0
        
        for i in range(len(s)):
            if s[i] in count:
                curr = max(count[s[i]],curr)
      
            maxSubstring = max(maxSubstring, i - curr + 1)
            count[s[i]] = i + 1

        return maxSubstring

    def sol1(self,s):
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
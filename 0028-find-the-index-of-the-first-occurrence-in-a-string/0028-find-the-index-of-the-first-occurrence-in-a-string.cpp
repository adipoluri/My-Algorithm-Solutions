class Solution {
public:
    int strStr(string haystack, string needle) {
        for(int i = 0; i < haystack.length(); i++){
            if(needle[0] == haystack[i]) {
                bool correct = true;
                for(int len = 1; len < needle.length(); len++) {
                    if(len+i > haystack.length()) {
                        correct = false;
                        break;
                    }
                    if(needle[len] != haystack[i+len]) {
                        correct = false;
                        break;
                    }
                }
                if(correct) return i;
            }
        }
        return -1;
    }
};
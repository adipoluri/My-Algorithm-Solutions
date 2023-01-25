class Solution {
public:
    bool isPalindrome(int x) {
        if(x<0) return false;
        long y = x;
        long reversed = 0;
        while(y != 0) {
            int digit = y % 10;
            reversed = reversed*10 + digit;
            y /= 10;
        }
        return x == reversed;
    }
};
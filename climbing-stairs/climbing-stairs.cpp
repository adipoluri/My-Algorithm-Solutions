class Solution {
public:
    int climbStairs(int n) {
        if (n==1 || n==2) return n;
        
        vector<int> memo;
        memo.push_back(0); //base cases 0,1,2
        memo.push_back(1);
        memo.push_back(2);
        for(int i = 3; i <= n; i++) {
            memo.push_back(memo[i-1]+memo[i-2]);
        }
        return memo.at(memo.size()-1);
    }
};
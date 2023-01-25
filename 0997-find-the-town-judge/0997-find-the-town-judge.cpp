class Solution {
public:
    int findJudge(int n, vector<vector<int>>& trust) {
        std::vector<int> pop(n+1);
        for(auto person : trust){
            pop[person[1]]++;
            pop[person[0]]--;
        }
        for(int i = 1; i <= n; i++) {
            if(pop[i] == n - 1) return i;
        }
        return -1;
    }
};
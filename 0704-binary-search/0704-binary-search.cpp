class Solution {
public:
    int search(vector<int>& nums, int target) {
        int searchInd = (nums.size()-1)/2;
        int low = 0;
        int high = nums.size()-1;
        while(low <= high) {
            
            searchInd = low + (high - low)/2;
            if (target == nums[searchInd]) return searchInd;
            if(target > nums[searchInd]) low = searchInd + 1;
            else high = searchInd - 1;
            
        }
        return -1;
    }
};
class Solution {
public:
    int findLucky(vector<int>& arr) {
        map<int,int> nums;
        for(auto num : arr) {
            nums[num]++;
        }
        int retval = -1;
        map<int,int>::iterator it;
        for (it = nums.begin(); it != nums.end(); it++)
           {
                if(it->first == it->second) retval = it->first; 
           }
        
        return retval;
        
    }
};
class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        int n = nums.size();
        int index = 0;
        int j = 0;
        while(j<n){
            if(nums[j]!=0){
                nums[index] = nums[j];
                index++;
            }
            j++;
        }
        while(index<n){
            nums[index] = 0;
            index++;
        }
    }
};
class Solution {
    public int[] twoSum(int[] nums, int target) {
        int[] retval = new int[2];
        for(int i = 0; i < nums.length; i++){
            int answer = target - nums[i];
            for(int j = i+1; j<nums.length; j++){
                if(nums[j] == answer){
                    retval[0] = i;
                    retval[1] = j;
                    return retval;
                }
            }
        }
        return retval;
    }
}
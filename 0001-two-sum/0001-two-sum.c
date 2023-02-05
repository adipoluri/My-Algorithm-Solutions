/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* twoSum(int* nums, int numsSize, int target, int* returnSize){
    *returnSize = 2;
    int * retVal = (int*)malloc(2*sizeof(int));
    for(int i = 0; i < numsSize-1; i++) {
        for(int j = i+1; j < numsSize; j++) {
            if(target-nums[i]==nums[j]){
                retVal[0] = i;
                retVal[1] = j;
                return retVal;
            }
        }
    }
    return retVal;
}
class Solution {
public:
    void duplicateZeros(vector<int>& arr) {
        int endSize = arr.size();
        for(int i =0; i < endSize;i++){
            if(arr[i] == 0){
                arr.insert(arr.begin() + i, 0);
                i++;
            }
        }
        arr.resize(endSize, 0);
    }
};
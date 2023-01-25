class Solution {
    public int findJudge(int N, int[][] trust) {
        int[] trustList = new int[N + 1];
        
        for (int[] person : trust) {
            trustList[person[1]]++;
            trustList[person[0]]--;
        }
        
        for(int i = 1; i <= N; i++) {
            if (trustList[i] == N - 1) {
                return i;
            }
        }
        return -1;
    }
}
#include <stdlib.h>     /* abs */

class Solution {
public:
    int reverse(int x) {
        int res = 0;
        int max = 2147483647;
        int min = -max;
        bool negative = x < 0;
        while(x != 0){
            if(res!=0&&max/abs(res) < 10) return 0;
            res = res*10 + x % 10;
            x /= 10;
        }
        return res;
    }
};
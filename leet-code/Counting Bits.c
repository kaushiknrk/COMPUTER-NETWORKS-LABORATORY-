#include <stdlib.h>

int* countBits(int n, int* returnSize) {
    
    *returnSize = n + 1;
    int* ans = (int*) malloc((*returnSize) * sizeof(int));
    
    for (int i = 0; i <= n; i++) {
        int temp = i;
        int count = 0;
        
       
        while (temp > 0) {
            int remainder = temp % 2; 
            if (remainder == 1) {
                count++; 
            }
            temp = temp / 2;
        }
        
        ans[i] = count;
    }
    
    return ans;
}

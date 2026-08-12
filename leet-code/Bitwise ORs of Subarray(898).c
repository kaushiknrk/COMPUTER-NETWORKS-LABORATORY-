#include <stdio.h>
#include <stdlib.h>

int compare(const void* a, const void* b) {
    int int_a = *(const int*)a;
    int int_b = *(const int*)b;
    if (int_a < int_b) return -1;
    if (int_a > int_b) return 1;
    return 0;
}

int subarrayBitwiseORs(int* arr, int arrSize) {
    int prev_set[33];
    int prev_size = 0;
    
    int curr_set[33];
    int curr_size = 0;
    
    int* global_results = (int*)malloc(arrSize * 33 * sizeof(int));
    int global_size = 0;
    
    for (int i = 0; i < arrSize; i++) {
        curr_size = 0;
        
        curr_set[curr_size++] = arr[i];
        
        for (int j = 0; j < prev_size; j++) {
            int new_or = prev_set[j] | arr[i];
            
            if (curr_set[curr_size - 1] != new_or) {
                curr_set[curr_size++] = new_or;
            }
        }
        
        qsort(curr_set, curr_size, sizeof(int), compare);
        int unique_curr_size = 0;
        for (int j = 0; j < curr_size; j++) {
            if (unique_curr_size == 0 || curr_set[j] != curr_set[unique_curr_size - 1]) {
                curr_set[unique_curr_size++] = curr_set[j];
            }
        }
        curr_size = unique_curr_size;
        
        for (int j = 0; j < curr_size; j++) {
            global_results[global_size++] = curr_set[j];
        }
        
        prev_size = curr_size;
        for (int j = 0; j < curr_size; j++) {
            prev_set[j] = curr_set[j];
        }
    }
    
    qsort(global_results, global_size, sizeof(int), compare);
    
    int distinct_count = 0;
    if (global_size > 0) {
        distinct_count = 1;
        for (int i = 1; i < global_size; i++) {
            if (global_results[i] != global_results[i - 1]) {
                distinct_count++;
            }
        }
    }
    
    free(global_results);
    return distinct_count;
}

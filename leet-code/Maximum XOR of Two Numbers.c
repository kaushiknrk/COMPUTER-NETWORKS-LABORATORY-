#include <stdlib.h>
#include <string.h>

struct TrieNode {
    int children[2];
};

struct TrieNode node_pool[6200000];
int pool_ptr;

int create_node() {
    node_pool[pool_ptr].children[0] = 0;
    node_pool[pool_ptr].children[1] = 0;
    return pool_ptr++;
}

void insert(int num) {
    int curr = 0;
    for (int i = 30; i >= 0; i--) {
        int bit = (num >> i) & 1;
        if (!node_pool[curr].children[bit]) {
            node_pool[curr].children[bit] = create_node();
        }
        curr = node_pool[curr].children[bit];
    }
}

int findMaxXorForNum(int num) {
    int curr = 0;
    int max_xor = 0;
    for (int i = 30; i >= 0; i--) {
        int bit = (num >> i) & 1;
        int toggled_bit = 1 - bit;
        if (node_pool[curr].children[toggled_bit]) {
            max_xor |= (1 << i);
            curr = node_pool[curr].children[toggled_bit];
        } else {
            curr = node_pool[curr].children[bit];
        }
    }
    return max_xor;
}

int findMaximumXOR(int* nums, int numsSize) {
    if (numsSize < 2) return 0;
    
    pool_ptr = 0;
    create_node(); 
    
    int global_max = 0;
    
    for (int i = 0; i < numsSize; i++) {
        insert(nums[i]);
    }
    
    for (int i = 0; i < numsSize; i++) {
        int current_max = findMaxXorForNum(nums[i]);
        if (current_max > global_max) {
            global_max = current_max;
        }
    }
    
    return global_max;
}

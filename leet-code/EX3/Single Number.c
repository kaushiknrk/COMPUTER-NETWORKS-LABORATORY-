int singleNumber(int* nums, int numsSize) {
    int unique_element = 0;
    
    for (int i = 0; i < numsSize; i++) {
        unique_element ^= nums[i];
    }
    
    return unique_element;
}

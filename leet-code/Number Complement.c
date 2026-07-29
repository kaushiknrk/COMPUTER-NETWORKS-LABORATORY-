int findComplement(int num) {

    int complement=0;
    unsigned int value=1;
    while(num>0){
        int bin=num%2;
        int change=1-bin;
        complement = complement +(change * value);
        value = value * 2;
        num=num/2;
    }
    return complement;
}

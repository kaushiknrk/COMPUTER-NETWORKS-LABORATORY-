#include <stdio.h>
#include <string.h>

int main() {
    char inputBits[50];
    int msgLen, checkBitsCount, totalBits, idx, step, offset, bitPos;
    int txCodeword[50], rxCodeword[50];
    int userChoice, injectErrorPos;

    printf("Enter data bits as a binary string: ");
    scanf("%s", inputBits);

    msgLen = strlen(inputBits);

    checkBitsCount = 0;
    while ((1 << checkBitsCount) < msgLen + checkBitsCount + 1) {
        checkBitsCount = checkBitsCount + 1;
    }

    totalBits = msgLen + checkBitsCount;

    step = 0;
    for (idx = 1; idx <= totalBits; idx++) {
        if ((idx & (idx - 1)) == 0) {
            txCodeword[idx] = -1;
        } else {
            txCodeword[idx] = inputBits[step] - '0';
            step = step + 1;
        }
    }

    for (idx = 0; idx < checkBitsCount; idx++) {
        bitPos = 1 << idx;
        int activeBitsCount = 0;
        for (step = bitPos; step <= totalBits; step = step + 1) {
            if (((step / bitPos) % 2) == 1) {
                if (txCodeword[step] == 1) {
                    activeBitsCount = activeBitsCount + 1;
                }
            }
        }
        if (activeBitsCount % 2 == 0) {
            txCodeword[bitPos] = 0;
        } else {
            txCodeword[bitPos] = 1;
        }
    }

    printf("\n--- Sender Side ---\n");
    printf("Data bits: %s\n", inputBits);
    printf("Number of parity bits: %d\n", checkBitsCount);
    printf("Transmitted codeword: ");
    for (idx = 1; idx <= totalBits; idx++) {
        printf("%d", txCodeword[idx]);
    }
    printf("\n");

    for (idx = 1; idx <= totalBits; idx++) {
        rxCodeword[idx] = txCodeword[idx];
    }

    printf("\nDo you want to introduce an error during transmission? (1 for yes, 0 for no): ");
    scanf("%d", &userChoice);

    if (userChoice == 1) {
        printf("Enter bit position to flip (1 to %d): ", totalBits);
        scanf("%d", &injectErrorPos);
        if (rxCodeword[injectErrorPos] == 0) {
            rxCodeword[injectErrorPos] = 1;
        } else {
            rxCodeword[injectErrorPos] = 0;
        }
    }

    printf("\n--- Receiver Side ---\n");
    printf("Received codeword: ");
    for (idx = 1; idx <= totalBits; idx++) {
        printf("%d", rxCodeword[idx]);
    }
    printf("\n");

    int errorLocator = 0;
    for (idx = 0; idx < checkBitsCount; idx++) {
        bitPos = 1 << idx;
        int activeBitsCount = 0;
        for (step = bitPos; step <= totalBits; step = step + 1) {
            if (((step / bitPos) % 2) == 1) {
                if (rxCodeword[step] == 1) {
                    activeBitsCount = activeBitsCount + 1;
                }
            }
        }
        if (activeBitsCount % 2 != 0) {
            errorLocator = errorLocator + bitPos;
        }
    }

    printf("Syndrome (error position): %d\n", errorLocator);

    if (errorLocator == 0) {
        printf("Result: No error detected\n");
    } else {
        printf("Result: Error detected at position %d\n", errorLocator);
        if (rxCodeword[errorLocator] == 0) {
            rxCodeword[errorLocator] = 1;
        } else {
            rxCodeword[errorLocator] = 0;
        }
        printf("Corrected codeword: ");
        for (idx = 1; idx <= totalBits; idx++) {
            printf("%d", rxCodeword[idx]);
        }
        printf("\n");
    }

    step = 0;
    char decodedData[50];
    for (idx = 1; idx <= totalBits; idx++) {
        if ((idx & (idx - 1)) != 0) {
            decodedData[step] = rxCodeword[idx] + '0';
            step = step + 1;
        }
    }
    decodedData[step] = '\0';

    printf("Extracted data bits: %s\n", decodedData);

    return 0;
}


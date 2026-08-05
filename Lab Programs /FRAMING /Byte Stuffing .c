#include <stdio.h>
#include <string.h>

#define MARKER "01111110"
#define BYPASS "00101111"

char raw_bits[100][9];
char tx_stream[300][9];
char rx_bits[100][9];

void encode_byte(char character, char stream[]) {
    int idx;
    for(idx = 7; idx >= 0; idx--) {
        stream[7 - idx] = ((character >> idx) & 1) + '0';
    }
    stream[8] = '\0';
}

char decode_byte(char stream[]) {
    int idx, numeric = 0;
    for(idx = 0; idx < 8; idx++) {
        numeric = numeric * 2 + (stream[idx] - '0');
    }
    return (char)numeric;
}

void execute_framing_process(char *text) {
    char final_out[100];
    int idx, tx_ptr = 0, rx_ptr = 0, length;

    length = strlen(text);
    printf("\n>>> CURRENT INPUT DATA: '%s' <<<\n", text);

    printf("\n[Step 1] Bit Conversion Map:\n");
    for(idx = 0; idx < length; idx++) {
        encode_byte(text[idx], raw_bits[idx]);
        printf("  Char [%c] -> Bits [%s]\n", text[idx], raw_bits[idx]);
    }

    strcpy(tx_stream[tx_ptr++], MARKER);
    for(idx = 0; idx < length; idx++) {
        if(strcmp(raw_bits[idx], MARKER) == 0 || strcmp(raw_bits[idx], BYPASS) == 0) {
            strcpy(tx_stream[tx_ptr++], BYPASS);
        }
        strcpy(tx_stream[tx_ptr++], raw_bits[idx]);
    }
    strcpy(tx_stream[tx_ptr++], MARKER);

    printf("\n[Step 2] Transmitted Bitstream:\n  ");
    for(idx = 0; idx < tx_ptr; idx++) {
        printf("%s ", tx_stream[idx]);
    }
    printf("\n");

    printf("\n[Step 3] Extracted Bitstream:\n  ");
    for(idx = 1; idx < tx_ptr - 1; idx++) {
        if(strcmp(tx_stream[idx], BYPASS) == 0) {
            idx++;
            strcpy(rx_bits[rx_ptr], tx_stream[idx]);
            printf("%s ", rx_bits[rx_ptr]);
            rx_ptr++;
        } else {
            strcpy(rx_bits[rx_ptr], tx_stream[idx]);
            printf("%s ", rx_bits[rx_ptr]);
            rx_ptr++;
        }
    }
    printf("\n");

    for(idx = 0; idx < rx_ptr; idx++) {
        final_out[idx] = decode_byte(rx_bits[idx]);
    }
    final_out[rx_ptr] = '\0';

    printf("\n>>> RECOVERED TEXT: '%s' <<<\n\n", final_out);
}

int main() {
    int user_action;
    char text_input[100];

    while(1) {
        printf("+++++++++++++++++++++++++++++++++++\n");
        printf("     DATA LINK LAYER SIMULATOR     \n");
        printf("+++++++++++++++++++++++++++++++++++\n");
        printf(" [1] Process Custom Text\n");
        printf(" [2] Terminate Program\n");
        printf("Select option: ");
        if (scanf("%d", &user_action) != 1) break;
        getchar();

        if(user_action == 2) {
            printf("Shutting down simulator...\n");
            break;
        }

        switch(user_action) {
            case 1:
                printf("\nInsert raw text payload (~ = FLAG, / = ESC): ");
                scanf("%[^\n]", text_input);
                getchar();
                execute_framing_process(text_input);
                break;

            default:
                printf("Error: Option not recognized.\n\n");
        }
    }

    return 0;
}

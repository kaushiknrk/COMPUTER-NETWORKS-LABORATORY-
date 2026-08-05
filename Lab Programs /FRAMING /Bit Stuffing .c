#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define max 100
#define max_bit (max * 8)
#define max_stuff (max_bit * 2)

int data[max_bit], stuff[max_stuff], destuff[max_bit];
int sender_framed[max_stuff];
int sender_framelen = 0;

char str[max];
char output_str[max];
int flag[] = {0, 1, 1, 1, 1, 1, 1, 0};
int flaglen = 8;

void print(char *label, int arr[], int n) {
   int i;
   printf("%s ", label);
   for(i = 0; i < n; i++) {
      printf("%d", arr[i]);
   }
   printf("\n");
}

int bit_stuffing(int input_bits[], int input_len, int output_bits[]) {
   int i, ones = 0, j = 0;
   for(i = 0; i < input_len; i++) {
      output_bits[j++] = input_bits[i];
      ones = (input_bits[i] == 1) ? ones + 1 : 0;
      if(ones == 5) {
         output_bits[j++] = 0;
         ones = 0;
      }
   }
   return j;
}

int bit_destuffing(int input_framed[], int total_len, int output_bits[]) {
   int i, ones = 0, j = 0;
   if (total_len < (flaglen * 2)) {
      printf("\n[FAILURE]: Bitstream length too short for standard delimiters!\n");
      return -1;
   }
   for(i = 0; i < flaglen; i++) {
      if(input_framed[i] != flag[i]) {
         printf("\n[FAILURE]: Starting delimiter mismatch detected!\n");
         return -1;
      }
   }
   for(i = 0; i < flaglen; i++) {
      if(input_framed[total_len - flaglen + i] != flag[i]) {
         printf("\n[FAILURE]: Ending delimiter mismatch detected!\n");
         return -1;
      }
   }
   for(i = flaglen; i < total_len - flaglen; i++) {
      output_bits[j++] = input_framed[i];
      ones = (input_framed[i] == 1) ? ones + 1 : 0;
      if(ones == 5) {
         if(input_framed[i + 1] != 0) {
            printf("\n[FAILURE]: Malformed frame bit sequence pattern at sequence validation point!\n");
            return -1;
         }
         i++; // Skip the stuffed 0
         ones = 0;
      }
   }
   return j;
}

void decode_binary_to_string(int input_bits[], int input_len, char target_str[]) {
   int i, b, out_idx = 0;
   // Safety constraint: ensure we only decode full completed bytes
   int structural_len = (input_len / 8) * 8;
   for(i = 0; i < structural_len; i += 8) {
      char ch = 0;
      for(b = 0; b < 8; b++) {
         ch = (ch << 1) | input_bits[i + b];
      }
      target_str[out_idx++] = ch;
   }
   target_str[out_idx] = '\0';
}

int main() {
   int choice;
   srand(time(NULL));
   while(1) {
      printf("\n----------------------------------\n");
      printf("    DATA LINK LAYER EMULATOR     \n");
      printf("----------------------------------\n");
      printf(" [1] Run Transmitter Module \n");
      printf(" [2] Run Receiver Module\n");
      printf(" [3] Terminate Program\n");
      printf("Select operation mode (1-3): ");
      if (scanf("%d", &choice) != 1) break;
      getchar();

      if(choice == 3) {
         printf("Shutting down core system modules...\n");
         break;
      }

      switch(choice) {
         case 1: {
            int i, b, bit = 0;
            printf("\nProvide message text to encode: ");
            scanf("%[^\n]", str);
            getchar();

            int str_len = strlen(str);
            for(i = 0; i < str_len; i++) {
               char ch = str[i];
               for(b = 7; b >= 0; b--) {
                  data[bit++] = (ch >> b) & 1;
               }
            }
            print("\nBinary Matrix:   ", data, bit);

            int stufflen = bit_stuffing(data, bit, stuff);
            print("Stuffed Payload: ", stuff, stufflen);

            sender_framelen = 0;
            for(i = 0; i < flaglen; i++) sender_framed[sender_framelen++] = flag[i];
            for(i = 0; i < stufflen; i++) sender_framed[sender_framelen++] = stuff[i];
            for(i = 0; i < flaglen; i++) sender_framed[sender_framelen++] = flag[i];
            print("Completed Frame: ", sender_framed, sender_framelen);
            int error_choice;
            printf("\n>>> Noise Generator System <<<\n");
            printf(" 1. Perfect Transmission (0%% attenuation)\n");
            printf(" 2. Induce 1-Bit Mutation\n");
            printf(" 3. Induce Multi-Bit Mutations\n");
            printf("Selection option: ");
            scanf("%d", &error_choice);
            getchar();

            if (error_choice == 2) {
               int error_pos = rand() % sender_framelen;
               sender_framed[error_pos] ^= 1;
               printf("\n[NOISE] Single-bit flip applied at array position: %d.\n", error_pos + 1);
               print("Mutated Payload:", sender_framed, sender_framelen);
            }
            else if (error_choice == 3) {
               int num_errors;
               printf("Quantity of bit mutations to apply: ");
               scanf("%d", &num_errors);
               getchar();

               if(num_errors > sender_framelen) {
                  num_errors = sender_framelen;
               }

               for(i = 0; i < num_errors; i++) {
                  int error_pos = rand() % sender_framelen;
                  sender_framed[error_pos] ^= 1;
               }
               printf("\n[NOISE] Multi-bit mutation completed across %d nodes!\n", num_errors);
               print("Mutated Payload:", sender_framed, sender_framelen);
            } else {
               printf("\nPayload buffer forwarded cleanly to virtual buffer.\n");
            }
            break;
         }

         case 2: {
            int i, rx_choice;
            int active_framed[max_stuff];
            int active_framelen = 0;

            printf("\n>>> Inbound Source Selection <<<\n");
            printf(" 1. Process Data from Virtual Buffer\n");
            printf(" 2. Key-in Raw Binary Stream Manually\n");
            printf("Selection option: ");
            scanf("%d", &rx_choice);
            getchar();

            if(rx_choice == 1) {
               if(sender_framelen == 0) {
                  printf("\n[ABORT]: Virtual buffer empty. Execute Transmitter Module first.\n");
                  break;
               }
               active_framelen = sender_framelen;
               for(i = 0; i < sender_framelen; i++) {
                  active_framed[i] = sender_framed[i];
               }
               print("\nReading Stream:  ", active_framed, active_framelen);
            }
            else if(rx_choice == 2) {
               char bit_string[max_stuff];
               printf("\nInput custom packet stream (Example: 01111110...): \n");
               scanf("%s", bit_string);
               getchar();

               active_framelen = strlen(bit_string);
               for(i = 0; i < active_framelen; i++) {
                  active_framed[i] = bit_string[i] - '0';
               }
            }
            else {
               printf("Unknown processing command\n");
               break;
            }

            int destufflen = bit_destuffing(active_framed, active_framelen, destuff);

            if(destufflen == -1) {
               printf("\n[PACKET DROPPED]: Invalid payload geometry or synchronization fail.\n");
            } else {
               print("\nRestored Stream: ", destuff, destufflen);
               decode_binary_to_string(destuff, destufflen, output_str);
               printf("Parsed Message:  %s\n", output_str);
            }
            break;
         }

         default:
            printf("Unknown system command\n");
      }
   }
   return 0;
}

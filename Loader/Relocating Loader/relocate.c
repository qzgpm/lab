#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void convertHexToBinary(const char *hex, char *binary);

int main(void) {
    char add[16], length[16], input[16], bitmask[64], bit[256], pn[32];
    char relocbit, ch;
    int start = 0, address = 0, opcode = 0, addr = 0, actualadd = 0, tlen = 0;
    int i, num_inst, bits_len, r;
    FILE *fp1 = NULL, *fp2 = NULL;

    printf("\nEnter the actual starting address (hex): ");
    if (scanf("%x", &start) != 1) {
        fprintf(stderr, "Invalid start address input.\n");
        return 1;
    }

    fp1 = fopen("RLIN.txt", "r");
    if (!fp1) {
        perror("Cannot open RLIN.txt");
        return 1;
    }
    fp2 = fopen("RLOUT.txt", "w");
    if (!fp2) {
        perror("Cannot open RLOUT.txt for writing");
        fclose(fp1);
        return 1;
    }

    fprintf(fp2, "----------------------------\n");
    fprintf(fp2, "ADDRESS\t\tCONTENT\n");
    fprintf(fp2, "----------------------------\n");

    /* Read first token (should be H if file is correct) */
    r = fscanf(fp1, "%15s", input);
    if (r != 1) {
        fprintf(stderr, "RLIN.txt appears empty or malformed.\n");
        fclose(fp1);
        fclose(fp2);
        return 1;
    }

    while (strcmp(input, "E") != 0) {
        if (strcmp(input, "H") == 0) {
            /* Header: H <progname> <start> <length> */
            if (fscanf(fp1, "%31s%15s%15s", pn, add, length) != 3) {
                fprintf(stderr, "Malformed header record.\n");
                break;
            }
            /* read next record type token */
            if (fscanf(fp1, "%15s", input) != 1) break;
            continue;
        }

        if (strcmp(input, "T") == 0) {
            /* Text record: T <address> <tlen> <bitmask> <obj...> */
            if (fscanf(fp1, "%x%x%63s", &address, &tlen, bitmask) != 3) {
                fprintf(stderr, "Malformed text record header.\n");
                break;
            }
            address += start;
            convertHexToBinary(bitmask, bit);
            bits_len = (int)strlen(bit);

            /* number of 3-byte objects in this text record */
            num_inst = tlen / 3;
            if (num_inst < 0) num_inst = 0;

            /* ensure we don't read more instructions than there are bits (safety) */
            if (bits_len < num_inst) {
                /* If fewer bits than instructions, limit by bits_len */
                num_inst = bits_len;
            }

            for (i = 0; i < num_inst; ++i) {
                if (fscanf(fp1, "%x%x", &opcode, &addr) != 2) {
                    fprintf(stderr, "Unexpected EOF or malformed object code.\n");
                    break;
                }
                relocbit = bit[i];
                if (relocbit == '1')
                    actualadd = addr + start;
                else
                    actualadd = addr;

                /* ADDRESS (6 hex)  CONTENT: opcode (2 hex) + operand (4 hex) */
                fprintf(fp2, "%06X\t\t%02X%04X\n", address, opcode, actualadd);
                address += 3;
            }

            /* read next record token (H, T, or E) */
            if (fscanf(fp1, "%15s", input) != 1) break;
            continue;
        }

        /* If we reach here, the token isn't H, T or E — try to read next token to avoid infinite loop */
        if (fscanf(fp1, "%15s", input) != 1) break;
    }

    fprintf(fp2, "----------------------------\n");
    fclose(fp1);
    fclose(fp2);

    /* Print output file to stdout */
    printf("\n\nThe contents of output file (RLOUT.TXT)\n\n");
    fp2 = fopen("RLOUT.txt", "r");
    if (!fp2) {
        perror("Cannot reopen RLOUT.txt");
        return 1;
    }
    while ((ch = fgetc(fp2)) != EOF) putchar(ch);
    fclose(fp2);

    return 0;
}

/* convert each hex digit to 4-bit binary (uppercase or lowercase allowed) */
void convertHexToBinary(const char *hex, char *binary) {
    static const char *map[16] = {
        "0000","0001","0010","0011",
        "0100","0101","0110","0111",
        "1000","1001","1010","1011",
        "1100","1101","1110","1111"
    };
    binary[0] = '\0';
    for (size_t i = 0; i < strlen(hex); ++i) {
        char c = hex[i];
        int idx = -1;
        if (c >= '0' && c <= '9') idx = c - '0';
        else if (c >= 'A' && c <= 'F') idx = c - 'A' + 10;
        else if (c >= 'a' && c <= 'f') idx = c - 'a' + 10;
        if (idx >= 0) strcat(binary, map[idx]);
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void passTwo() {
    FILE *fp1, *fp2, *fp3, *fp4, *fp5;
    char label[10], opcode[10], operand[10];
    char code[10], mnemonic[10], symLabel[10];
    char progName[10] = "NONAME";

    int loc, startAddr = 0, progLen = 0;

    fp1 = fopen("intermediate.txt", "r");  
    fp2 = fopen("optab.txt", "r");         
    fp3 = fopen("symtab.txt", "r");        
    fp4 = fopen("length.txt", "r");        
    fp5 = fopen("output.txt", "w");        

    if (!fp1 || !fp2 || !fp3 || !fp4 || !fp5) {
        perror("Error opening files");
        exit(1);
    }

    fscanf(fp4, "%X", &progLen);
    fclose(fp4);

    fscanf(fp1, "%s%s%s", label, opcode, operand);
    if (strcmp(opcode, "START") == 0) {
        startAddr = (int)strtol(operand, NULL, 16);
        if (strcmp(label, "**") != 0) {
            strcpy(progName, label);
        }
        fprintf(fp5, "H^%-6s^%06X^%06X\n", progName, startAddr, progLen);
        fscanf(fp1, "%X%s%s%s", &loc, label, opcode, operand);
    } else {
        fprintf(fp5, "H^%-6s^%06X^%06X\n", progName, startAddr, progLen);
        loc = 0;
    }

    char textRec[100] = "";
    int textStart = loc;
    int textLen = 0;

    while (strcmp(opcode, "END") != 0) {
        int found = 0;
        rewind(fp2);

        while (fscanf(fp2, "%s%s", code, mnemonic) != EOF) {
            if (strcmp(opcode, code) == 0) {
                int addr = 0, symFound = 0;
                rewind(fp3);
                while (fscanf(fp3, "%s%X", symLabel, &addr) != EOF) {
                    if (strcmp(operand, symLabel) == 0) {
                        symFound = 1;
                        break;
                    }
                }
                if (!symFound) addr = 0;

                char objCode[20];
                sprintf(objCode, "%s%04X", mnemonic, addr);

                if (textLen + 3 > 30) {
                    fprintf(fp5, "T^%06X^%02X%s\n", textStart, textLen, textRec);
                    textRec[0] = '\0';
                    textStart = loc;
                    textLen = 0;
                }

                strcat(textRec, "^");
                strcat(textRec, objCode);
                textLen += 3;

                found = 1;
                break;
            }
        }

        if (!found) {
            if (strcmp(opcode, "WORD") == 0) {
                char objCode[20];
                sprintf(objCode, "%06X", atoi(operand));

                if (textLen + 3 > 30) {
                    fprintf(fp5, "T^%06X^%02X%s\n", textStart, textLen, textRec);
                    textRec[0] = '\0';
                    textStart = loc;
                    textLen = 0;
                }

                strcat(textRec, "^");
                strcat(textRec, objCode);
                textLen += 3;
            }
            else if (strcmp(opcode, "BYTE") == 0) {
                char objCode[40] = "";
                if (operand[0] == 'C') {
                    for (int i = 2; i < (int)strlen(operand) - 1; i++) {
                        char hex[5];
                        sprintf(hex, "%02X", operand[i]);
                        strcat(objCode, hex);
                    }
                } else if (operand[0] == 'X') {
                    strncpy(objCode, operand + 2, strlen(operand) - 3);
                    objCode[strlen(operand) - 3] = '\0';
                }

                int byteLen = strlen(objCode) / 2;

                if (textLen + byteLen > 30) {
                    fprintf(fp5, "T^%06X^%02X%s\n", textStart, textLen, textRec);
                    textRec[0] = '\0';
                    textStart = loc;
                    textLen = 0;
                }

                strcat(textRec, "^");
                strcat(textRec, objCode);
                textLen += byteLen;
            }
            else if (strcmp(opcode, "RESW") == 0 || strcmp(opcode, "RESB") == 0) {
                if (textLen > 0) {
                    fprintf(fp5, "T^%06X^%02X%s\n", textStart, textLen, textRec);
                    textRec[0] = '\0';
                    textLen = 0;
                }
            }
        }

        fscanf(fp1, "%X%s%s%s", &loc, label, opcode, operand);
    }

    if (textLen > 0) {
        fprintf(fp5, "T^%06X^%02X%s\n", textStart, textLen, textRec);
    }

    fprintf(fp5, "E^%06X\n", startAddr);

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp5);

    printf("\nPass Two completed. Object program written to output.txt\n");
}

int main() {
    passTwo();
    return 0;
}

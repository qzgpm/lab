#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void display() {
    int ch;
    FILE *fp1, *fp2, *fp3;

    printf("\nThe contents of Input Table:\n\n");
    fp1 = fopen("input.txt", "r");
    if (fp1 == NULL) { perror("input.txt"); exit(1); }
    while ((ch = fgetc(fp1)) != EOF) {
        putchar(ch);
    }
    fclose(fp1);

    printf("\n\nThe contents of Output (Intermediate) Table:\n\n");
    fp2 = fopen("intermediate.txt", "r");
    if (fp2 == NULL) { perror("intermediate.txt"); exit(1); }
    while ((ch = fgetc(fp2)) != EOF) {
        putchar(ch);
    }
    fclose(fp2);

    printf("\n\nThe contents of Symbol Table:\n\n");
    fp3 = fopen("symtab.txt", "r");
    if (fp3 == NULL) { perror("symtab.txt"); exit(1); }
    while ((ch = fgetc(fp3)) != EOF) {
        putchar(ch);
    }
    fclose(fp3);
}

void passOne(char label[10], char opcode[10], char operand[10], char code[10], char nemonic[10]) {
    int locctr, start, length;
    FILE *fp1, *fp2, *fp3, *fp4, *fp5;

    fp1 = fopen("input.txt", "r");
    fp2 = fopen("optab.txt", "r");
    fp3 = fopen("symtab.txt", "w");
    fp4 = fopen("intermediate.txt", "w");
    fp5 = fopen("length.txt", "w");

    if (!fp1 || !fp2 || !fp3 || !fp4 || !fp5) {
        perror("Error opening file");
        exit(1);
    }

    if (fscanf(fp1, "%s\t%s\t%s", label, opcode, operand) != 3) {
        printf("Invalid input file\n");
        exit(1);
    }

    if (strcmp(opcode, "START") == 0) {
        start = (int)strtol(operand, NULL, 16);
        locctr = start;
        fprintf(fp4, "\t%s\t%s\t%s\n", label, opcode, operand);
        fscanf(fp1, "%s\t%s\t%s", label, opcode, operand);
    } else {
        locctr = 0;
        start = 0;
    }

    while (strcmp(opcode, "END") != 0) {
        fprintf(fp4, "%04X\t%s\t%s\t%s\n", locctr, label, opcode, operand);

        if (strcmp(label, "**") != 0) {
            fprintf(fp3, "%s\t%04X\n", label, locctr);
        }

        rewind(fp2);
        int found = 0;
        while (fscanf(fp2, "%s\t%s", code, nemonic) != EOF) {
            if (strcmp(opcode, code) == 0) {
                locctr += 3;
                found = 1;
                break;
            }
        }

        if (!found) {
            if (strcmp(opcode, "WORD") == 0) {
                locctr += 3;
            }
            else if (strcmp(opcode, "RESW") == 0) {
                locctr += 3 * atoi(operand);
            }
            else if (strcmp(opcode, "BYTE") == 0) {
                if (operand[0] == 'C')
                    locctr += (int)strlen(operand) - 3;
                else if (operand[0] == 'X')
                    locctr += ((int)strlen(operand) - 3) / 2;
                else
                    locctr++;
            }
            else if (strcmp(opcode, "RESB") == 0) {
                locctr += atoi(operand);
            }
        }

        if (fscanf(fp1, "%s\t%s\t%s", label, opcode, operand) != 3)
            break;
    }

    fprintf(fp4, "%04X\t%s\t%s\t%s\n", locctr, label, opcode, operand);

    fclose(fp4);
    fclose(fp3);
    fclose(fp2);
    fclose(fp1);

    display();

    length = locctr - start;
    fprintf(fp5, "%X\n", length);
    fclose(fp5);

    printf("\nThe length of the code : %d (decimal) = %X (hex)\n", length, length);
}

int main() {
    char label[10], opcode[10], operand[10];
    char code[10], nemonic[10];
    passOne(label, opcode, operand, code, nemonic);
    return 0;
}

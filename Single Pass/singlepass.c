#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *my_itoa(int num, char *str)
{
    if (str == NULL)
        return NULL;
    sprintf(str, "%d", num);
    return str;
}

int main()
{
    FILE *f1, *f2, *f3, *f4, *f5;
    int lc, sa, i = 0, j = 0, m[10], pgmlen, len, k, len1, l = 0;
    int textlen = 0;
    char name[10], opnd[10], la[10], mne[10], s1[10], mne1[10], opnd1[10];
    char lcs[10], ms[10];
    char sym[10], symaddr[10], obj1[10], obj2[10], s2[10], q[10], s3[10];

    f1 = fopen("input.txt", "r");
    f2 = fopen("optab.txt", "r");
    f3 = fopen("symtab.txt", "w+");
    f4 = fopen("symtab1.txt", "w+");
    f5 = fopen("output.txt", "w+");

    if (!f1 || !f2 || !f3 || !f4 || !f5)
    {
        printf("Error opening files.\n");
        return 1;
    }

    fscanf(f1, "%s%s%s", la, mne, opnd);

    if (strcmp(mne, "START") == 0)
    {
        sa = atoi(opnd);
        strcpy(name, la);
        lc = sa;
    }

    strcpy(s1, "*");
    fscanf(f1, "%s%s%s", la, mne, opnd);

    while (strcmp(mne, "END") != 0)
    {
        if (strcmp(la, "-") == 0)
        {
            fseek(f2, 0, SEEK_SET);
            fscanf(f2, "%s%s", mne1, opnd1);
            while (!feof(f2))
            {
                if (strcmp(mne1, mne) == 0)
                {
                    m[i] = lc + 1;
                    fprintf(f3, "%s\t%s\n", opnd, s1);
                    fprintf(f5, "%s\t0000\n", opnd1);
                    lc += 3;
                    textlen += 3;
                    i++;
                    break;
                }
                fscanf(f2, "%s%s", mne1, opnd1);
            }
        }
        else
        {
            fseek(f3, 0, SEEK_SET);
            fscanf(f3, "%s%s", sym, symaddr);
            while (!feof(f3))
            {
                if (strcmp(sym, la) == 0)
                {
                    my_itoa(lc, lcs);
                    fprintf(f4, "%s\t%s\n", la, lcs);
                    my_itoa(m[j], ms);
                    j++;
                    fprintf(f5, "%s\t%s\n", ms, lcs);
                    i++;
                    break;
                }
                fscanf(f3, "%s%s", sym, symaddr);
            }

            if (strcmp(mne, "RESW") == 0)
                lc += 3 * atoi(opnd);
            else if (strcmp(mne, "RESB") == 0)
                lc += atoi(opnd);
            else if (strcmp(mne, "BYTE") == 0)
            {
                strcpy(s2, "-");
                len = strlen(opnd);
                lc += len - 3;
                textlen += len - 3;
                for (k = 2; k < len - 1; k++)
                {
                    q[l++] = opnd[k];
                }
                q[l] = '\0';
                fprintf(f5, "%s\t%s\n", q, s2);
            }
            else if (strcmp(mne, "WORD") == 0)
            {
                strcpy(s3, "#");
                lc += 3;
                textlen += 3;
                fprintf(f5, "%s\t%s\n", opnd, s3);
            }
        }

        fseek(f2, 0, SEEK_SET);
        fscanf(f1, "%s%s%s", la, mne, opnd);
    }

    fseek(f5, 0, SEEK_SET);
    pgmlen = lc - sa;

    printf("H^%s^%d^0%x\n", name, sa, pgmlen);
    printf("T^");
    printf("00%d^0%x", sa, textlen);

    fscanf(f5, "%s%s", obj1, obj2);
    while (!feof(f5))
    {
        if (strcmp(obj2, "0000") == 0)
            printf("^%s%s", obj1, obj2);
        else if (strcmp(obj2, "-") == 0)
        {
            printf("^");
            len1 = strlen(obj1);
            for (k = 0; k < len1; k++)
                printf("%X", obj1[k]);
        }
        else if (strcmp(obj2, "#") == 0)
        {
            printf("^%s", obj1);
        }
        fscanf(f5, "%s%s", obj1, obj2);
    }

    fseek(f5, 0, SEEK_SET);
    fscanf(f5, "%s%s", obj1, obj2);
    while (!feof(f5))
    {
        if (strcmp(obj2, "0000") != 0)
        {
            if (strcmp(obj2, "-") != 0)
            {
                if (strcmp(obj2, "#") != 0)
                {
                    printf("\n");
                    printf("T^%s^02^%s", obj1, obj2);
                }
            }
        }
        fscanf(f5, "%s%s", obj1, obj2);
    }

    printf("\nE^00%d\n", sa);

    fclose(f1);
    fclose(f2);
    fclose(f3);
    fclose(f4);
    fclose(f5);

    return 0;
}

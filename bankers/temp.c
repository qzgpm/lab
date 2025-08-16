#include <stdio.h>

#define MAX_PROCESSES 20
#define MAX_RESOURCES 100

int avail[MAX_RESOURCES];
int total_res[MAX_RESOURCES];
int total_alloc[MAX_RESOURCES];

struct process {
    int pid;
    int max[MAX_RESOURCES];
    int alloc[MAX_RESOURCES];
    int need[MAX_RESOURCES];
    int finish;
} p[MAX_PROCESSES];

int main() {
    int i, j, m, n;
    int sequence[MAX_PROCESSES], x = 0;
    int work[MAX_RESOURCES];
    int systemSafe = 1;

    printf("ENTER THE NUMBER OF RESOURCES: ");
    scanf("%d", &m);

    printf("MAXIMUM RESOURCE COUNT FOR EACH RESOURCE:\n");
    for (j = 0; j < m; j++) {
        printf("Resource %d: ", j);
        scanf("%d", &total_res[j]);
    }

    printf("\nENTER THE NUMBER OF PROCESSES: ");
    scanf("%d", &n);

    for (j = 0; j < m; j++) {
        total_alloc[j] = 0;
    }

    for (i = 0; i < n; i++) {
        p[i].pid = i;

        printf("\nALLOCATED RESOURCES FOR PROCESS P%d:\n", i);
        for (j = 0; j < m; j++) {
            printf("Resource %d: ", j);
            scanf("%d", &p[i].alloc[j]);
        }

        printf("MAXIMUM DEMAND FOR PROCESS P%d:\n", i);
        for (j = 0; j < m; j++) {
            printf("Resource %d: ", j);
            scanf("%d", &p[i].max[j]);
            p[i].need[j] = p[i].max[j] - p[i].alloc[j];
        }

        for (j = 0; j < m; j++) {
            total_alloc[j] += p[i].alloc[j];
        }

        p[i].finish = 0;
    }

    // Calculate available resources
    printf("\nAVAILABLE RESOURCES:\n");
    for (j = 0; j < m; j++) {
        avail[j] = total_res[j] - total_alloc[j];
        work[j] = avail[j];
        printf("Resource %d: %d\n", j, avail[j]);
    }

    // Banker's Algorithm to check system safety
    int done = 0;
    while (done < n) {
        int flag = 0;

        for (i = 0; i < n; i++) {
            if (!p[i].finish) {
                int canExecute = 1;

                for (j = 0; j < m; j++) {
                    if (p[i].need[j] > work[j]) {
                        canExecute = 0;
                        break;
                    }
                }

                if (canExecute) {
                    for (j = 0; j < m; j++) {
                        work[j] += p[i].alloc[j];
                    }
                    sequence[x++] = p[i].pid;
                    p[i].finish = 1;
                    flag = 1;
                    done++;
                }
            }
        }

        if (!flag) {
            systemSafe = 0;
            break;
        }
    }

    // Display table
    printf("\nPROCESS\t\tMAX\t\tALLOC\t\tNEED\n");
    for (i = 0; i < n; i++) {
        printf("P%d\t\t", p[i].pid);
        for (j = 0; j < m; j++)
            printf("%d ", p[i].max[j]);
        printf("\t");

        for (j = 0; j < m; j++)
            printf("%d ", p[i].alloc[j]);
        printf("\t");

        for (j = 0; j < m; j++)
            printf("%d ", p[i].need[j]);
        printf("\n");
    }

    // Print result
    if (systemSafe) {
        printf("\nSAFE SEQUENCE: ");
        for (i = 0; i < x; i++) {
            printf("P%d", sequence[i]);
            if (i < x - 1) printf(" -> ");
        }
        printf("\nSystem is in a SAFE STATE.\n");
    } else {
        printf("\nSystem is in an UNSAFE STATE (Deadlock may occur).\n");
    }

    return 0;
}

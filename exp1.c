#include <stdio.h>
#include <stdlib.h>

struct process {
    int pid, at, bt, ct, tat, wt, priority, btcopy;
};

void getProcessDetails(struct process p[], int *n, int needPriority) {
    printf("Enter number of processes: ");
    scanf("%d", n);
    for (int i = 0; i < *n; i++) {
        printf("Enter Arrival Time and Burst Time for P%d: ", i + 1);
        scanf("%d %d", &p[i].at, &p[i].bt);
        p[i].pid = i + 1;
        if (needPriority) {
            printf("Enter Priority for P%d: ", i + 1);
            scanf("%d", &p[i].priority);
        }
    }
}

void fcfs(struct process p[], int n) {
    int i;
    float avgtat = 0, avgwt = 0;

    // Sort by Arrival Time
    for (i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (p[i].at > p[j].at) {
                struct process temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }

    p[0].ct = p[0].at + p[0].bt;
    for (i = 1; i < n; i++) {
        if (p[i].at > p[i - 1].ct)
            p[i].ct = p[i].at + p[i].bt;
        else
            p[i].ct = p[i - 1].ct + p[i].bt;
    }

    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for (i = 0; i < n; i++) {
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        avgtat += p[i].tat;
        avgwt += p[i].wt;
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }

    printf("Average TAT = %.2f\n", avgtat / n);
    printf("Average WT = %.2f\n", avgwt / n);
}

void sjf(struct process p[], int n) {
    int completed = 0, current_time = 0;
    float total_tat = 0, total_wt = 0;

    while (completed != n) {
        int idx = -1, min_bt = 9999;

        for (int i = 0; i < n; i++)
            if (p[i].at <= current_time && p[i].ct == 0 && p[i].bt < min_bt) {
                min_bt = p[i].bt;
                idx = i;
            }

        if (idx != -1) {
            p[idx].ct = current_time + p[idx].bt;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            total_tat += p[idx].tat;
            total_wt += p[idx].wt;
            current_time = p[idx].ct;
            completed++;
        } else {
            current_time++;
        }
    }

    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++)
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);

    printf("Average TAT = %.2f\n", total_tat / n);
    printf("Average WT = %.2f\n", total_wt / n);
}

void rr(struct process p[], int n, int ts) {
    int i, ct = 0, completed = 0, q[100], front = 0, rear = -1, in_q[20] = {0};
    float total_tat = 0, total_wt = 0;

    for (i = 0; i < n; i++)
        p[i].btcopy = p[i].bt;

    printf("\nExecution Order: ");

    while (completed < n) {
        for (i = 0; i < n; i++) {
            if (p[i].at <= ct && p[i].bt > 0 && in_q[i] == 0) {
                q[++rear] = i;
                in_q[i] = 1;
            }
        }

        if (front > rear) {
            ct++;
            continue;
        }

        int id = q[front++];
        printf("P%d -> ", p[id].pid);

        if (p[id].bt > ts) {
            p[id].bt -= ts;
            ct += ts;
        } else {
            ct += p[id].bt;
            p[id].tat = ct - p[id].at;
            p[id].wt = p[id].tat - p[id].btcopy;
            p[id].bt = 0;
            completed++;
        }

        for (i = 0; i < n; i++) {
            if (p[i].at <= ct && p[i].bt > 0 && in_q[i] == 0) {
                q[++rear] = i;
                in_q[i] = 1;
            }
        }

        if (p[id].bt > 0)
            q[++rear] = id;
    }

    printf("\n\nPID\tAT\tBT\tTAT\tWT\n");
    for (i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].btcopy, p[i].tat, p[i].wt);
        total_tat += p[i].tat;
        total_wt += p[i].wt;
    }

    printf("Average TAT = %.2f\n", total_tat / n);
    printf("Average WT = %.2f\n", total_wt / n);
}

void priority(struct process p[], int n) {
    int i, j;
    float avgtat = 0, avgwt = 0;

    for (i = 0; i < n - 1; i++)
        for (j = 0; j < n - i - 1; j++)
            if (p[j].priority > p[j + 1].priority) {
                struct process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }

    p[0].ct = p[0].at + p[0].bt;
    for (i = 1; i < n; i++) {
        if (p[i].at > p[i - 1].ct)
            p[i].ct = p[i].at + p[i].bt;
        else
            p[i].ct = p[i - 1].ct + p[i].bt;
    }

    for (i = 0; i < n; i++) {
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        avgtat += p[i].tat;
        avgwt += p[i].wt;
    }

    printf("\nPID\tAT\tPR\tBT\tCT\tTAT\tWT\n");
    for (i = 0; i < n; i++)
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].priority, p[i].bt, p[i].ct, p[i].tat, p[i].wt);

    printf("Average TAT = %.2f\n", avgtat / n);
    printf("Average WT = %.2f\n", avgwt / n);
}

int main() {
    int choice, n, i, ts;
    struct process p[20]; // Allocate enough space

    printf("\n========= CPU Scheduling Menu =========\n");
    printf("1. FCFS\n");
    printf("2. SJF (Non-Preemptive)\n");
    printf("3. Round Robin\n");
    printf("4. Priority (Non-Preemptive)\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice >= 1 && choice <= 4) {
        printf("Enter the number of processes: ");
        scanf("%d", &n);

        for (i = 0; i < n; i++) {
            p[i].pid = i + 1;
            printf("Enter Arrival Time and Burst Time for Process %d: ", p[i].pid);
            scanf("%d %d", &p[i].at, &p[i].bt);

            // For priority scheduling
            if (choice == 4) {
                printf("Enter Priority for Process %d: ", p[i].pid);
                scanf("%d", &p[i].priority);
            }

            // Clear any previous data
            p[i].ct = p[i].tat = p[i].wt = 0;
        }

        if (choice == 3) {
            printf("Enter Time Slice for Round Robin: ");
            scanf("%d", &ts);
        }
    }

    switch(choice) {
        case 1:
            fcfs(p, n);
            break;
        case 2:
            sjf(p, n);
            break;
        case 3:
            rr(p, n, ts);
            break;
        case 4:
            priority(p, n);
            break;
        case 5:
            printf("Exiting program...\n");
            exit(0);
        default:
            printf("Invalid choice!\n");
    }

    return 0;
}


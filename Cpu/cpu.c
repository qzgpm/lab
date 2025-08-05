#include <stdio.h>

typedef struct process {
    int pid, at, bt, ct, tat, wt, prio, rem_bt;
} pro;

void input(pro p[], int n, int with_priority) {
    for (int i = 0; i < n; i++) {
        p[i].pid = i;
        printf("Enter Arrival Time and Burst Time for P%d: ", i);
        scanf("%d %d", &p[i].at, &p[i].bt);
        p[i].rem_bt = p[i].bt;
        if (with_priority) {
            printf("Enter Priority for P%d: ", i);
            scanf("%d", &p[i].prio);
        }
    }
}

void print_table(pro p[], int n) {
    float sum_tat = 0, sum_wt = 0;
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        sum_tat += p[i].tat;
        sum_wt += p[i].wt;
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }
    printf("Avg TAT = %.2f\n", sum_tat / n);
    printf("Avg WT  = %.2f\n", sum_wt / n);
}

void fcfs(pro p[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (p[j].at > p[j + 1].at) {
                pro tmp = p[j]; p[j] = p[j + 1]; p[j + 1] = tmp;
            }

    int time = 0;
    for (int i = 0; i < n; i++) {
        if (time < p[i].at) time = p[i].at;
        time += p[i].bt;
        p[i].ct = time;
    }
    print_table(p, n);
}

void sjf(pro p[], int n) {
    int time = 0, completed = 0;
    while (completed < n) {
        int idx = -1, min_bt = 1e9;
        for (int i = 0; i < n; i++)
            if (p[i].rem_bt > 0 && p[i].at <= time && p[i].bt < min_bt) {
                min_bt = p[i].bt;
                idx = i;
            }

        if (idx == -1) { time++; continue; }

        time += p[idx].bt;
        p[idx].ct = time;
        p[idx].rem_bt = 0;
        completed++;
    }
    print_table(p, n);
}

void priority(pro p[], int n) {
    int time = 0, completed = 0;
    while (completed < n) {
        int idx = -1, min_pr = 1e9;
        for (int i = 0; i < n; i++)
            if (p[i].rem_bt > 0 && p[i].at <= time && p[i].prio < min_pr) {
                min_pr = p[i].prio;
                idx = i;
            }

        if (idx == -1) { time++; continue; }

        time += p[idx].bt;
        p[idx].ct = time;
        p[idx].rem_bt = 0;
        completed++;
    }
    print_table(p, n);
}

void rr(pro p[], int n, int q) {
    int time = 0, completed = 0;
    while (completed < n) {
        int idle = 1;
        for (int i = 0; i < n; i++) {
            if (p[i].rem_bt > 0 && p[i].at <= time) {
                idle = 0;
                int t = (p[i].rem_bt > q) ? q : p[i].rem_bt;
                time += t;
                p[i].rem_bt -= t;
		printf("P%d|",p[i].pid);
                if (p[i].rem_bt == 0) {
                    p[i].ct = time;
                    completed++;
                }
            }
        }
        if (idle) time++;
    }
    print_table(p, n);
}

int main() {
    int n, ch, quantum;
    pro p[20];

    printf("Enter number of processes: ");
    scanf("%d", &n);

    do {
        printf("\n1.FCFS\n2.SJF\n3.Priority\n4.Round Robin\n5.Exit\nChoose: ");
        scanf("%d", &ch);
        switch (ch) {
            case 1:
                input(p, n, 0);
                fcfs(p, n);
                break;
            case 2:
                input(p, n, 0);
                sjf(p, n);
                break;
            case 3:
                input(p, n, 1);
                priority(p, n);
                break;
            case 4:
                input(p, n, 0);
                printf("Enter Time Quantum: ");
                scanf("%d", &quantum);
                rr(p, n, quantum);
                break;
            case 5:
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (ch != 5);

    return 0;
}

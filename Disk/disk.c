#include <stdio.h>
#include <stdlib.h>

void fcfs(int req[], int n, int head) {
    int seek = 0;
    printf("\nFCFS Order:\n");
    for (int i = 0; i < n; i++) {
        printf("%d -> ", req[i]);
        seek += abs(req[i] - head);
        head = req[i];
    }
    printf("\nTotal Seek Time: %d\n", seek);
    printf("Average Seek Time: %.2f\n", (float)seek / n);
}

int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

void scan(int req[], int n, int head, int size, int prev) {
    int seek = 0;
    int dir = head > prev ? 1 : 0;
    req[n++] = head;
    qsort(req, n, sizeof(int), compare);

    int idx;
    for (idx = 0; idx < n; idx++)
        if (req[idx] == head) break;

    printf("\nSCAN Order:\n");
    if (dir) { // right then left
        for (int i = idx; i < n; i++) {
            printf("%d -> ", req[i]);
            seek += abs(head - req[i]);
            head = req[i];
        }
        seek += abs(head - (size - 1));
        head = size - 1;
        printf("%d -> ", head);
        for (int i = idx - 1; i >= 0; i--) {
            printf("%d -> ", req[i]);
            seek += abs(head - req[i]);
            head = req[i];
        }
    } else { // left then right
        for (int i = idx; i >= 0; i--) {
            printf("%d -> ", req[i]);
            seek += abs(head - req[i]);
            head = req[i];
        }
        seek += head;
        head = 0;
        printf("%d -> ", head);
        for (int i = idx + 1; i < n; i++) {
            printf("%d -> ", req[i]);
            seek += abs(head - req[i]);
            head = req[i];
        }
    }
    printf("\nTotal Seek Time: %d\n", seek);
    printf("Average Seek Time: %.2f\n", (float)seek / (n - 1));
}

void cscan(int req[], int n, int head, int size, int prev) {
    int seek = 0;
    int dir = head > prev ? 1 : 0;
    req[n++] = head;
    qsort(req, n, sizeof(int), compare);

    int idx;
    for (idx = 0; idx < n; idx++)
        if (req[idx] == head) break;

    printf("\nC-SCAN Order:\n");
    if (dir) {
        for (int i = idx; i < n; i++) {
            printf("%d -> ", req[i]);
            seek += abs(head - req[i]);
            head = req[i];
        }
        seek += abs(head - (size - 1)) + (size - 1);
        head = 0;
        printf("%d -> ", head);
        for (int i = 0; i < idx; i++) {
            printf("%d -> ", req[i]);
            seek += abs(head - req[i]);
            head = req[i];
        }
    } else {
        for (int i = idx; i >= 0; i--) {
            printf("%d -> ", req[i]);
            seek += abs(head - req[i]);
            head = req[i];
        }
        seek += head + (size - 1);
        head = size - 1;
        printf("%d -> ", head);
        for (int i = n - 1; i > idx; i--) {
            printf("%d -> ", req[i]);
            seek += abs(head - req[i]);
            head = req[i];
        }
    }
    printf("\nTotal Seek Time: %d\n", seek);
    printf("Average Seek Time: %.2f\n", (float)seek / (n - 1));
}

int main() {
    int req[100], n, head, prev, size, ch;

    printf("Enter Disk Size: ");
    scanf("%d", &size);
    printf("Enter Number of Requests: ");
    scanf("%d", &n);
    printf("Enter Disk Requests:\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &req[i]);
    printf("Enter Current Head Position: ");
    scanf("%d", &head);
    printf("Enter Previous Head Position: ");
    scanf("%d", &prev);

    do{
    printf("\nDisk Scheduling Algorithms:\n1. FCFS\n2. SCAN\n3. C-SCAN\nChoose: ");
    scanf("%d", &ch);

    switch (ch) {
        case 1: fcfs(req, n, head); break;
        case 2: scan(req, n, head, size, prev); break;
        case 3: cscan(req, n, head, size, prev); break;
        default: printf("Invalid Choice\n");
    }
    }while(ch != 4);

    return 0;
}

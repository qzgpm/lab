#include <stdio.h>
#include <stdlib.h>

int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

void print_stats(int seek, int count) {
    printf("\nTotal Seek Time: %d\n", seek);
    printf("Average Seek Time: %.2f\n", (float)seek / count);
}

void fcfs(int req[], int n, int head) {
    int seek = 0;
    printf("\nFCFS Order:\n%d", head);
    for (int i = 0; i < n; i++) {
        seek += abs(req[i] - head);
        head = req[i];
        printf(" -> %d", head);
    }
    print_stats(seek, n);
}

void scan(int req[], int n, int head, int size, int prev) {
    int seek = 0;
    int dir = head > prev ? 1 : 0;

    qsort(req, n, sizeof(int), compare);

    int idx = 0;
    while (idx < n && req[idx] < head) idx++;

    printf("\nSCAN Order:\n%d", head);

    if (dir) {  // moving right
        for (int i = idx; i < n; i++) {
            seek += abs(head - req[i]);
            head = req[i];
            printf(" -> %d", head);
        }
        if (head != size - 1) {
            seek += (size - 1) - head;
            head = size - 1;
            printf(" -> %d", head);
        }
        for (int i = idx - 1; i >= 0; i--) {
            seek += abs(head - req[i]);
            head = req[i];
            printf(" -> %d", head);
        }
    } else {   // moving left
        for (int i = idx - 1; i >= 0; i--) {
            seek += abs(head - req[i]);
            head = req[i];
            printf(" -> %d", head);
        }
        if (head != 0) {
            seek += head;
            head = 0;
            printf(" -> %d", head);
        }
        for (int i = idx; i < n; i++) {
            seek += abs(head - req[i]);
            head = req[i];
            printf(" -> %d", head);
        }
    }
    print_stats(seek, n);
}

void cscan(int req[], int n, int head, int size, int prev) {
    int seek = 0;
    int dir = head > prev ? 1 : 0;

    qsort(req, n, sizeof(int), compare);

    int idx = 0;
    while (idx < n && req[idx] < head) idx++;

    printf("\nC-SCAN Order:\n%d", head);

    if (dir) {  // moving right
        for (int i = idx; i < n; i++) {
            seek += abs(head - req[i]);
            head = req[i];
            printf(" -> %d", head);
        }
        if (head != size - 1) {
            seek += (size - 1) - head;
            head = size - 1;
            printf(" -> %d", head);
        }
        seek += size - 1; // jump from end to start
        head = 0;
        printf(" -> %d", head);

        for (int i = 0; i < idx; i++) {
            seek += abs(head - req[i]);
            head = req[i];
            printf(" -> %d", head);
        }
    } else {  // moving left
        for (int i = idx - 1; i >= 0; i--) {
            seek += abs(head - req[i]);
            head = req[i];
            printf(" -> %d", head);
        }
        if (head != 0) {
            seek += head;
            head = 0;
            printf(" -> %d", head);
        }
        seek += size - 1; // jump from start to end
        head = size - 1;
        printf(" -> %d", head);

        for (int i = n - 1; i >= idx; i--) {
            seek += abs(head - req[i]);
            head = req[i];
            printf(" -> %d", head);
        }
    }
    print_stats(seek, n);
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

    do {
        printf("\nDisk Scheduling Algorithms:\n");
        printf("1. FCFS\n2. SCAN\n3. C-SCAN\n4. Exit\nChoose: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1: fcfs(req, n, head); break;
            case 2: scan(req, n, head, size, prev); break;
            case 3: cscan(req, n, head, size, prev); break;
            case 4: printf("Exiting...\n"); break;
            default: printf("Invalid Choice\n");
        }
    } while (ch != 4);

    return 0;
}

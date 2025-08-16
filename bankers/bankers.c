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
}p[MAX_RESOURCES];

int main(){
  int i, j, m, n;
  int sequence[MAX_PROCESSES], x = 0;
  int work[MAX_RESOURCES];
  int systemSafe = 1;

  printf("Enter The No. of Resources: ");
  scanf("%d",&m);

  printf("Enter The MAX Resources for:-\n");
  for (i = 0; i < m; i++) {
    printf("Resources %d: ",i+1);
    scanf("%d",&total_res[i]);
  }

  printf("Enter The No of Processes: ");
  scanf("%d",&n);

  for ( i = 0; i < m; i++){
    total_alloc[i] = 0;
  }

  for ( i = 0; i < n; i++){
    p[i].pid = i;

    printf("The Allocation Resources of Process P0%d:-\n",i+1);
    for( j = 0; j < m; j++){
      printf("Resources %d: ",j+1);
      scanf("%d",&p[i].alloc[j]);
    }

    printf("The MAX Resources of Process P0%d:-\n",i+1);
    for( j = 0; j < m; j++){
      printf("Resources %d: ",j+1);
      scanf("%d",&p[i].max[j]);
      p[i].need[j] = p[i].max[j] - p[i].alloc[j];
    }

    for ( j = 0; j < m; j++){
      total_alloc[j] += p[i].alloc[j];
    }

    p[i].finish = 0;
  }

  printf("\nAVAILABLE RESOURCES:\n");
  for (j = 0; j < m; j++) {
      avail[j] = total_res[j] - total_alloc[j];
      work[j] = avail[j];
      printf("Resource %d: %d\n", j+1, avail[j]);
  }

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

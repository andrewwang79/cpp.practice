#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void hi_crash() {
  int a = 0;
  double dd = 10 / a;
}

char* command = "./out/crash";
char* argument_list[] = {NULL};
void execvp_crash() {
  int status_code = execvp(command, argument_list);
  printf("execvp has taken control of this child process. This won't execute unless it terminates abnormally!\n");

  if (status_code == -1) {
    printf("Terminated Incorrectly\n");
  }
}

int main(int argc, char** argv) {
  int flag = 0;
  pid_t pId = fork();
  if (pId == -1) {
    perror("fork error");
    exit(EXIT_FAILURE);
  } else if (pId == 0) {
    int myPid = getpid();
    int parentPid = getppid();

    printf("Child:SelfID=%d ParentID=%d \n", myPid, parentPid);
    flag = 123;
    printf("Child:flag=%d %p \n", flag, &flag);

    int count = 0;
    do {
      count++;
      sleep(1);
      printf("Child count=%d \n", count);
      if (count >= 2) {
        break;
      }
    } while (1);
    execvp_crash();
    return EXIT_SUCCESS;
  } else {
    printf("Parent:SelfID=%d MyChildPID=%d \n", getpid(), pId);
    flag = 456;
    printf("Parent:flag=%d %p \n", flag,
           &flag);  // 连地址都一样,说明是真的完全拷贝,但值已经是不同的了..
    int count = 0;
    do {
      count++;
      sleep(1);
      printf("Parent count=%d \n", count);
      if (count >= 5) {
        break;
      }
    } while (1);
  }

  return EXIT_SUCCESS;
}

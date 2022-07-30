#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int main()
{
  int pipefd[2];
  char buffer[10];

  /* Clear the buffer */
  memset(buffer, 0, sizeof(buffer));

  /* Open the pipe */
  if (pipe(pipefd) < 0)
  {
    printf("[%d:%d] ERROR: Failed to open pipe\n", getppid(), getpid());
    exit(1);
  }

  /* Create a child process */
  pid_t child_pid = fork();
  assert(child_pid >= 0);
  if (child_pid == 0)
  {
    /* Child closes write-end, then reads from the pipe */
    close(pipefd[1]);
    ssize_t bytes_read = read(pipefd[0], buffer, 10);
    if (bytes_read <= 0)
      exit(0);

    printf("[%d:%d] Child received: '%s'\n", getppid(), getpid(), buffer);
    exit(0);
  }
  /* Parent closes the unused reading end */
  close(pipefd[0]);

  /* Parent sends 'hello' and waits */
  strncpy(buffer, "hello", sizeof(buffer));
  printf("[%d:%d] Parent is sending '%s'\n", getppid(), getpid(), buffer);
  write(pipefd[1], buffer, sizeof(buffer));
  wait(NULL);
  printf("[%d:%d] Child should have printed the message\n", getppid(), getpid());
  
  return 0;
}

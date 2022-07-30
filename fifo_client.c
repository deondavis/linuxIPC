#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>   // included for assert call
#include <fcntl.h>    // included for open call
#include <sys/stat.h> // included for O_RDONLY
#include <unistd.h>   // included for read close unlink calls

int main()
{
const char *FIFO = "/tmp/MY_FIFO";
/* Use the file name to open the FIFO for writing */
int fifo = open (FIFO, O_WRONLY);
assert (fifo != -1);

printf ("[%d:%d] fifo_client\n",getppid(),getpid());

/* Open the FIFO 6 times, writing an int each time */
for (int index = 5; index >= 0; index--)
  {
    /* Write 5, 4, 3, 2, 1, 0 into the FIFO */
    int msg = index;
    write (fifo, &msg, sizeof (int));

    /* Add a slight delay each time */
    sleep (1);
  }

/* Close the FIFO */
close (fifo);

return 0;
}






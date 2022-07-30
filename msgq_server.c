#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>   // included for assert call
#include <fcntl.h>    // included for open call
#include <sys/stat.h> // included for O_RDONLY
#include <unistd.h>   // included for read close unlink calls
#include <mqueue.h>

#define MSG_QUE_NAME "/MY_QUE"
#ifdef BUG_DEMO
#define MSG_QUE_NAME "/tmp/MY_QUE"
/* reason detailed here:
   https://stackoverflow.com/questions/35093655/mq-open-err-no-13-permission-denied */
#endif

int main()
{

   struct message
   {
      int priority;
      long data;
   };

   struct message my_message[10] = {0};
   /* Create and open a message queue for writing */
   mqd_t mqd = mq_open(MSG_QUE_NAME, O_CREAT | O_EXCL | O_WRONLY, 0600, NULL);

   /* Ensure the creation was successful */
   if (mqd == -1)
   {
      perror("mq_open");
      exit(1);
   }

   for (int index = 5, priority = 10; index >= 0; index--, priority++)
   {
      my_message->data = index;
      my_message->priority = priority;
      mq_send(mqd, (const char *)&my_message, sizeof(struct message), priority);
   }

   mq_close(mqd);
  
   return 0;
}
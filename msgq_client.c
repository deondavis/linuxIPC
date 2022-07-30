#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>   // included for assert call
#include <fcntl.h>    // included for open call
#include <sys/stat.h> // included for O_RDONLY
#include <unistd.h>   // included for read close unlink calls
#include <mqueue.h>

int main()
{
   struct message
   {
      int priority;
      long data;
   };

   /* Open the message queue for reading */
   mqd_t mqd = mq_open("/MY_QUE", O_RDONLY);
   assert(mqd != -1);

   /* Get the message queue attributes */
   struct mq_attr attr;
   assert(mq_getattr(mqd, &attr) != -1);

   char *buffer = calloc(attr.mq_msgsize, 1);
   assert(buffer != NULL);

   /* Retrieve message from the queue and get its priority level */
   unsigned int priority = 0;
   for (int index = 5; index >= 0; index--)
   {
      if ((mq_receive(mqd, buffer, attr.mq_msgsize, &priority)) == -1)
      {
         printf("Failed to receive message\n");
      }
      else
      {
         struct message *msg = (struct message *)buffer;
         printf("Received [priority %u]: %ld'\n", priority, msg->data);
      }
      priority = 0;
   }
   /* Clean up the allocated memory and message queue */
   free(buffer);
   buffer = NULL;
   mq_close(mqd);
   mq_unlink("/MY_QUE");
}
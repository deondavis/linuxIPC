#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <assert.h>
#include <unistd.h>

#define SHM_OBJ_SIZE 4096
#define SHM_FILE_NAME "/tmp/MY_SHM"

int main()
{
   /* Open the file to get the file descriptor */
   int fd = open(SHM_FILE_NAME, O_RDONLY);
   assert(fd != -1);

   /* Map the object into memory so file operations aren't needed */
   void *ptr = mmap(NULL, SHM_OBJ_SIZE, PROT_READ, MAP_SHARED, fd, 0);

   /* read from the shared memory object */
   printf("%s", (char *)ptr);

   /* unmap from the virtual address space of this proceess */
   munmap(ptr, SHM_OBJ_SIZE);

   /* close the shared memory object */
   close(fd);

   /* unlinking the shared memory object */
   shm_unlink(SHM_FILE_NAME);

   return 0;
}
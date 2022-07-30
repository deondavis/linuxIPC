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
#define TEST_DATA1 "hello-"
#define TEST_DATA2 "veoneer"

int main()
{
   /* Open the file to get the file descriptor */
int fd = open (SHM_FILE_NAME, O_CREAT | O_RDWR);
assert (fd != -1);


   /* configure the size of the shared memory object */
   ftruncate(fd, SHM_OBJ_SIZE);

   /* Map the object into memory so file operations aren't needed */
   void *ptr = mmap(NULL, SHM_OBJ_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

   assert(ptr != MAP_FAILED);

   /* write to the shared memory object */
   /* memcpy can be also used */
   sprintf(ptr, "%s", TEST_DATA1);

   /* do normal pointer arithmetic to update the memory contents directly */
   ptr += strlen(TEST_DATA1);

   sprintf(ptr, "%s", TEST_DATA2);
   ptr += strlen(TEST_DATA2);

   /* unmap from the virtual address space of this proceess */
   munmap(ptr, SHM_OBJ_SIZE);

   /* close the shared memory object */
   close(fd);

   return 0;
}
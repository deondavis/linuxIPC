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
#define SHM_OBJ_NAME "/MY_SHM"

int main()
{
   /* Create unsized shared memory object;
   generally created under /dev/shm/ by linux and
   return value is a file descriptor */
   int shm_fd = shm_open(SHM_OBJ_NAME, O_RDONLY, S_IRUSR | S_IWUSR);
   assert(shm_fd != -1);

   /* Map the object into memory so file operations aren't needed */
   void *ptr = mmap(NULL, SHM_OBJ_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

   /* read from the shared memory object */
   printf("%s", (char *)ptr);

   /* unmap from the virtual address space of this proceess */
   munmap(ptr, SHM_OBJ_SIZE);

   /* close the shared memory object */
   close(shm_fd);

   /* unlinking the shared memory object */
   shm_unlink(SHM_OBJ_NAME);

   return 0;
}
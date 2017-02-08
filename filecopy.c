// int main(int argc, const char * argv[]) {
//      // insert code here...
//      ifstream file1(argv[1]);
//      ifstream file2(argv[2]);
//      ifstream file3(argv[3]);
// }
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
int delete_file(const char *name){
  int status = remove(name);
  return status;
}


int perform_copy(const char *from, const char *to){
    int fd_to, fd_from;
    char buf[4096];
    char delete[10];
    ssize_t nread;
    int saved_errno;

    fd_from = open(from, O_RDONLY);
    if (fd_from < 0){
        printf("Failed to open %s.\n", from);
        return -1;

    }
    fd_to = open(to, O_WRONLY | O_CREAT | O_EXCL, 0666);
    if (fd_to < 0){
        printf("%s already exists. Do you wish to overwrite? (y/n)\n", to);
        scanf ("%s", delete);
        if (strcmp(delete, "y") == 0){
          int deleted_result = delete_file(to);
          if (deleted_result != 0 ){
            goto out_error;
            return -1;
          }
        }else{
          goto out_error;
          return -1;
        }

    }
    fd_to = open(to, O_WRONLY | O_CREAT | O_EXCL, 0666);


    while (nread = read(fd_from, buf, sizeof buf), nread > 0)
    {
        char *out_ptr = buf;
        ssize_t nwritten;

        do {
            nwritten = write(fd_to, out_ptr, nread);

            if (nwritten >= 0)
            {
                nread -= nwritten;
                out_ptr += nwritten;
            }
            else if (errno != EINTR)
            {
                printf("error2\n");
                goto out_error;
            }
        } while (nread > 0);
    }

    if (nread == 0)
    {
        if (close(fd_to) < 0)
        {
            fd_to = -1;
            goto out_error;
        }
        close(fd_from);
        printf("success!\n");
        /* Success! */
        return 0;
    }

  out_error:
    saved_errno = errno;
    printf("error\n");
    close(fd_from);
    if (fd_to >= 0)
        close(fd_to);

    errno = saved_errno;
    return -1;
}

int main(int argc, const char *argv[]){
  char from_file[128];

  printf("Enter the name of the file to copy from: \n");
  scanf ("%s", from_file);

  char to_file [128];

  printf("Enter the name of the file to copy to: \n");
  scanf ("%s", to_file);

  printf("Now copying file from %s to %s...", from_file, to_file);


  int result = perform_copy(from_file, to_file);
  return result;

}

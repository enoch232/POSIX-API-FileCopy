// Enoch Ko
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
//this method is to delete a file.
int delete_file(const char *name){
  int status = remove(name);
  return status;
}

//this method is to copy a file.
int perform_copy(const char *from, const char *to){
  int to_fd, from_fd; //create two fds, one for 'to' and other one for 'from'
  char buff[4096]; //create a buffer with enough space.
  char delete[10]; //create a variable to store the answer for whether user wants to overwrite
  ssize_t read_byte;  //create ssize_t to hold the result from reading.

  from_fd = open(from, O_RDONLY); //open 'from' file, with Read only.
  if (from_fd < 0){ //if error,
      printf("Failed to open %s.\n", from); //display error message
      close(from_fd); //close from since we opened it.
      return -1; // return -1 to show there was an error.
  }
  to_fd = open(to, O_CREAT| O_WRONLY, 0666); //when 'from' file is successfully opened, open 'to' with three (Create a file if it doesnt exist, error if exists , and open for writing only to overwrite. Also give 0666 permission to be able to read and write to it.)
  // printf("TO_FD: %d", to_fd);
  if (to_fd < 0){ // if error
      printf("\n%s already exists. Do you wish to overwrite? (y/n)\n", to); //display message
      scanf ("%s", delete); //ask for user's input and store it to delete var.
      if (strcmp(delete, "y") == 0){ //if user said yes to overwrite
        int deleted_result = delete_file(to); //delete the one that exists, and give back to result if it delete successfully
        if (deleted_result != 0 ){ // if it didnt delete successfully,
          printf("Failed to overwrite."); //show message
          close(from_fd); //close from file
          if (to_fd >= 0) //if to file is open,
              close(to_fd); //close to file too.
          return -1; // return -1 to show there was an error.
          // to_fd = open(to, O_CREAT | O_EXCL | O_WRONLY, 0666); //when 'from' file is successfully opened, open 'to' with three (Create a file if it doesnt exist, error if exists , and open for writing only to overwrite. Also give 0666 permission to be able to read and write to it.)
        }
      }else if (strcmp(delete, "n") == 0 ){ // if no to overwrite
        close(from_fd); // close from file
        if (to_fd >= 0) // if to file is open,
            close(to_fd); //close to file too.
        return -1; //return -1 to show there was an error.
      }else{
        close(from_fd); // close from file
        if (to_fd >= 0) // if to file is open,
            close(to_fd); //close to file too.
        printf("Invalid answer, program terminated."); //display error message.
        return -1; //return -1 to show there was an error.
      }
  }

  read_byte = read(from_fd, buff, sizeof buff); //read bytes from read to the buffer.
  while (read_byte > 0){ //if the size its copying over is more than 0,
      ssize_t write_byte; // a variable to store
      char *buff_replica = buff; //a temp place so that it can keep copying forward.
      do {
          write_byte = write(to_fd, buff_replica, read_byte); //first it will write at buff location, but buff replica will increase as much as it reads.

          if (write_byte >= 0){ //how much it wrote forward,
              read_byte -= write_byte; //now subtract how much it wrote
              buff_replica += write_byte; //and change the pointer so that it can later write to correct spot.
          }
      } while (read_byte > 0); //keep doing until it cant read.
      read_byte = read(from_fd, buff, sizeof buff); //update the read_byte
  }
  if (read_byte == 0) // if finished,
  {
      close(to_fd); //close to file
      close(from_fd); //close from file.
      return 0; //return success.
  }
  //
  //This is commented out because this following code is for one at a time only.
  // to_fd = open(to, O_WRONLY | O_CREAT, 0666);
  // read_byte = read(from_fd, buff, sizeof buff);
  // // printf("read byte %zd\n", read_byte);
  // write(to_fd, buff, read_byte);
  // return 1;
  // close(from_fd);
  // if (to_fd >= 0)
  //     close(to_fd);

}

int main(int argc, const char *argv[]){
  char from_file[128]; //variable to store name of from file
  char to_file [128]; //variable to store name of to file

  printf("Enter the name of the file to copy from: \n"); //show message
  scanf ("%s", from_file); //take input
  printf("Enter the name of the file to copy to: \n"); //show message
  scanf ("%s", to_file); //take input

  printf("Now copying file from %s to %s...\n", from_file, to_file); //show message


  int result = perform_copy(from_file, to_file); //perform actual copy
  return result;//show the result

}

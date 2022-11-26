#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define INT_SIZE sizeof(int)
#define PRIME_END 35

void sieve(int *);

int main(int argc, char *argv[]) {
  int i;
  int pid;
  int pipeline[2];

  pipe(pipeline);

  pid = fork();
  if (pid == 0) {
    sieve(pipeline); 
  } else {
    for (i = 2; i <= PRIME_END; i++) {
      write(pipeline[1], &i, INT_SIZE); // write  
    }
    close(pipeline[1]);
    wait((int *)0);
  }

  exit(0);
}

void sieve(int *from_pipeline) {
  int p, n, k;
  int pid;
  int to_pipeline[2];

  /**
   * IMPORTANT!!!
   *
   * Quota from book-riscv-rev2, page 16:
   * > After fork, both parent and child have file descriptors referring to the pipe.
   *
   * > If no data is available, a read on a pipe waits for either data to be written
   * > or for all file descriptors referring to the write end to be closed;
   * > in the latter case, read will return 0, just as if the end of a data file had been reached.
   * > The fact that read blocks until it is impossible for new data to arrive is one reason
   * > that it’s important for the child to close the write end of the pipe before executing wc above:
   * > if one of wc ’s file descriptors referred to the write end of the pipe, wc would never see end-of-file.
   */

  // anytime to read, close write first
  close(from_pipeline[1]);
  // get a number from left neighbor, wait for it, if close then return 0
  if ((k = read(from_pipeline[0], &p, INT_SIZE)) == 0) { 
    // it's a timing to exit
    // since from pipeline has been closed
    // we do not expect any more number from this
    close(from_pipeline[0]);
    exit(0);
  }

  // what we read is a prime cause 
  // cause first we get 2, it's a prime
  // next in a child process 
  // we wait for the next number above
  // in parent process, 4 has been drop
  //3 is most left and has been written to from_pipeline, and we read it
  // and print 3
  // in third process, since 3 has been printed, 4 has been drop
  // we wait until 5 % 3 != 0
  // in fourth thread
  // left is 5
  // 6 has been drop in second process (pid=4)
  // 7 % 5 != 0
  // print 7 
  fprintf(1, "%d: prime %d\n", getpid(), p);

  pipe(to_pipeline);
  pid = fork(); // create a proc
  if (pid == 0) { // child never a prime
    sieve(to_pipeline);
  } else {
    while ((k = read(from_pipeline[0], &n, INT_SIZE)) != 0) { // get a number from left neighbor
      if (n % p) { // a  prime
        write(to_pipeline[1], &n, INT_SIZE); // send to right neighbor
      }
      // else we drop that number 
      // eg 4 % 2 == 0 so we drop 4, 3, 9, .. remain
      // cause no more in the left neighbor(3 itself is most left)
      //print(3)
      //next we drop 9 cause 9 % 3
      // ... 
    }
    // it's a timing to close from_pipeline read and to_pipeline write
    // while loop tells us no more reason to read somthing from "from_pipeline"
    // we have write all potential primes to child no more reason to write
    close(from_pipeline[0]);
    close(to_pipeline[1]);
    // wait all child dead
    wait((int *)0);
  }
  exit(0);
}

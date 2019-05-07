/* 
    implements the 'classic' solution to the readers writers problem which
    prioritizes readers
    - uses processes and System V semaphores
    - the shared resource is a file RESSFILE
    - instead of a set containing two semaphores, _two sets_ with one 
      semaphore each are used
    - the shared counter 'rc' is implemented as a shared variable in a 
      shared memory segment

    Literature:
    [1] P.J. Courtois, F. Heymans, D.L. Parnas: Concurrent Control with 
      ``Readers'' and ``Writers'', CACM 10(14)1971, pp. 667-668
    [2] Andrew S. Tanenbaum, Modern Operating Systems, 3rd ed., Pearson, 
        2009, pp.165-166
    [3] William Stallings, Operating Systems, 6th ed., Prentice Hall, 2007, 
        pp. 245ff 
    and many more
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <limits.h>

#define MAXPROCESSES 20000

#define RESSFILE "zahl.dat"
#define ITERATIONS 100
#define INITVALUE 0UL

#define SEMKEY_MUTEX 0xDEADBEEF
#define SEMKEY_WRI   0xDEAFBABE


/* some prototypes */
void fatal(char*);
void write_file(void);
void read_file(void);

/*
   global resources 
*/
int id_mutex, id_wri;     /* semids of semaphores */
int *rc;                  /* reader counter in shm segment */
unsigned long readers, writers;
/*
    a fatal error has occured -> exit 
*/
void fatal(char *msg)
{
  assert (msg != NULL);

  perror(msg);
  exit(EXIT_FAILURE);
}

/*
    a non-fatal error has occured -> warn, but continue
*/
void nonfatal(char *msg)
{
  assert (msg != NULL);

  perror(msg);
  return;
}

void P(int semid)
{
  struct sembuf mysemop;

  mysemop.sem_num = 0;
  mysemop.sem_op = -1; 
  mysemop.sem_flg = 0;
	
  if ( semop(semid, &mysemop, 1) == -1 ) {
    fatal("P()");
  }
}

void V(int semid)
{
  struct sembuf mysemop;

  mysemop.sem_num = 0;
  mysemop.sem_op = 1;
  mysemop.sem_flg = 0;
  
  if ( semop(semid, &mysemop, 1) == -1 ) {
    fatal("V()");
  }
}

void writer(void)
{
  int c;

  printf("Writer, PID %d, starts\n", getpid());

  for (c=0; c < ITERATIONS; c++) {
    P(id_wri);
    write_file();
    V(id_wri);
  }
  printf("Writer, PID %d, finished\n", getpid());
  exit(EXIT_SUCCESS);
}

void reader(void)
{
  int c;

  printf("Reader, PID %d, starts\n", getpid());

  for (c=0; c < ITERATIONS; c++) {

    P(id_mutex);
    *rc += 1;
    if (*rc == 1) {
      P(id_wri);
    } 
    printf("rc = %d\n", *rc);
    V(id_mutex);

    read_file();

    P(id_mutex);
    *rc -= 1;
    if (*rc == 0) {
      V(id_wri);
    } 
    V(id_mutex);
  }

  printf("Reader, PID %d, finished\n", getpid());
  exit(EXIT_SUCCESS);
}

/*
    must be called atomically by writers
*/
void write_file(void)
{
  FILE *fin;
  int ret;
  unsigned long x;

  fin = fopen(RESSFILE, "r+");
  if (fin == NULL) {
    fatal("fopen for write");
  }

  ret = fscanf(fin, "%lu", &x); /* returns # of converted items */
  if (ret != 1) {
    fatal("fscanf during write access");
  }

  x+=1;
  rewind(fin);
  ret = fprintf(fin, "%ld\n", x);
  if (ret == -1){
    fatal("fprintf");
  }

  printf("Writer, PID %i, wrote %ld\n", getpid(), x);

  ret = fclose(fin);
  if (ret == EOF) {
    fatal("fclose");
  }
}

void read_file(void)
{
  FILE *fin;
  int ret;
  unsigned long x;

  fin = fopen(RESSFILE, "r");
  if (fin == NULL) {
    fatal ("fopen for reading");
  }

  ret = fscanf(fin, "%lu", &x); /* returns # of converted items */
  if (ret != 1) {
    fatal ("fscanf");
  }

  ret = fclose(fin);
  if (ret == EOF) {
    fatal ("fclose after reading");
  }
}

int main(int argc, char* argv[])
{
  pid_t son[MAXPROCESSES], tmppid;
  int c, ret, id_shm;

  FILE *fin;
  unsigned long x;

  /* parse command line parameters */
  if (argc != 3) {
    printf("Usage: %s <readers> <writers>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  readers = strtoul(argv[1], NULL, 10);
  writers = strtoul(argv[2], NULL, 10);
  if ((writers == ULONG_MAX) || (readers == ULONG_MAX)) {
    printf("Could not convert number of readers of writers.\n");
    exit(EXIT_FAILURE);
  }
  if (readers+writers > MAXPROCESSES) {
    printf("Maximum Number of processes (%i) exceeded.\n", MAXPROCESSES);
    exit(EXIT_FAILURE);
  }

  /* create and reset shared file */
  fin = fopen(RESSFILE, "w");
  if (fin == NULL) {
    fatal("main, fopen for write");
  }
  fprintf(fin, "%lu\n", INITVALUE);
  fclose(fin);
  
  /* create and attach an unnamed shared memory segment */
  id_shm = shmget(IPC_PRIVATE, 4096, IPC_CREAT|0600);
  if (id_shm == -1) {
    fatal("shmget");
  }
  rc = (int*) shmat(id_shm, NULL, 0);
  if (rc == (void*) -1) {
    fatal("shmat");
  }

  /* create named semaphore set 'mutex' */
  id_mutex = semget(SEMKEY_MUTEX, 1, IPC_CREAT|S_IRUSR|S_IWUSR);
  if (id_mutex == -1) {
    fatal("semget mutex");
  }
  /* create named semaphore set 'wri' */
  id_wri = semget(SEMKEY_WRI, 1, IPC_CREAT|S_IRUSR|S_IWUSR);
  if (id_wri == -1) {
    fatal("semget wri");
  }

  /* init mutex and wri as "open" */
  mysem.val = 1;
  ret = semctl(id_mutex, 0, SETVAL, mysem);
  if (ret == -1) {
    fatal("init mutex");
    /* XXX remove semaphore set */
  }
  ret = semctl(id_wri, 0, SETVAL, mysem);
  if (ret == -1) {
    fatal("init wri");
    /* XXX remove semaphore set */
  }

  /* create processes, readers first */
  for (c=0; c<readers; c++) {
    tmppid = fork();
    if (tmppid == -1) {
      fatal("fork a reader");
    }
    if (tmppid != 0) {
      /* Father */
      son[c] = tmppid;
    }  
    else {
      reader(); 
      assert(0); /* may not be reached, because reader() terminates */
    }
  }
  /* only father reaches this */

  /* create writers processes */
  for ( ; c<readers+writers; c++) {
    tmppid = fork();
    if (tmppid == -1) {
      fatal("fork a reader");
    }
    if (tmppid != 0) {
      /* Father */
      son[c] = tmppid;
    }  
    else {
      writer(); 
      assert(0); /* may not be reached, because writer() terminates */
    }
  }
    
  /* only father reaches this */

  /* wait for children to terminate */
  for (c=0; c<readers+writers; c++) {
    tmppid = wait(&ret);
    printf("Son with PID %ld returned %d.\n", (long) tmppid, ret);
  }

  /* detach and delete shared memory segment */
  ret = shmdt((void*) rc);
  if (ret == -1) {
    fatal("shmdt");
  }
  ret = shmctl(id_shm, IPC_RMID, NULL);
  if (ret == -1) {
    fatal("delete shared memory segment");
  }

  /* destroy the semaphores */
  ret = semctl(id_wri, 0, IPC_RMID);
  if (ret == -1) {
    fatal("delete semaphore wri");
  }
  ret = semctl(id_mutex, 0, IPC_RMID);
  if (ret == -1) {
    fatal("delete semaphore mutex");
  }

  /* open shared file and check its final value */
  fin = fopen(RESSFILE, "r");
  if (fin == NULL) {
    nonfatal("main, fopen for check");
  }

  ret = fscanf(fin, "%lu", &x); /* returns # of converted items */
  if (ret != 1) {
    nonfatal ("main, fscanf while checking");
  }

  fclose(fin);

  if (x == writers*ITERATIONS) {
    printf("Correct value %ld in %s reached.\nDeleting.", x, RESSFILE);
    ret = unlink(RESSFILE);
    if (ret == -1) {
      fatal("unlink");
    }
  }
  else {
    printf("Incorrect value %ld in %s .\n", x, RESSFILE);
    printf("Leaving file for inspection.\n");
  }

  exit(EXIT_SUCCESS);
}

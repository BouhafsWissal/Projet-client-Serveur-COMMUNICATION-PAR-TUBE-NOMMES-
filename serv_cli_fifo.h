#ifndef SERV_CLI_FIFO_H_INCLUDED
#define SERV_CLI_FIFO_H_INCLUDED


#define NMIN 1
#define NMAX 10
#define FIFO1 "fifo1"
#define FIFO2 "fifo2"


typedef struct Question {
   int n;
   int pid;
} Question;

typedef struct Reponse {
   int pid;
   int * tab;
} Reponse;


#endif


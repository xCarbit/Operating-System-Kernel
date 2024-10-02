// Created by os on 7/23/23.
//

#ifndef PROJECT_BASE_SYSCALL_C_H
#define PROJECT_BASE_SYSCALL_C_H

#include "../lib/hw.h"



int time_sleep(time_t t);



void* mem_alloc (size_t size);


int mem_free (void*);


class TCB;
typedef TCB* thread_t;
int thread_create (
        thread_t* handle,
        void(*start_routine)(void*),
        void* arg
);

int thread_createCPP (thread_t* handle, void(*start_routine)(void*),void* arg);

int thread_exit ();


void thread_dispatch ();

void thread_join (thread_t handle);

void changeMode();
class Sem;
typedef Sem* sem_t;
int sem_open (sem_t* handle, unsigned init);

int sem_close (sem_t handle);
int sem_wait (sem_t id);
int sem_signal (sem_t id);


int sem_trywait (sem_t id);


void putc(char);
char getc();


#endif //PROJECT_BASE_SYSCALL_C_H

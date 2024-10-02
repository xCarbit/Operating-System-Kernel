//
// Created by os on 8/16/23.
//

#ifndef PROJECT_BASE_V1_1_SYSCALL_CPP_H
#define PROJECT_BASE_V1_1_SYSCALL_CPP_H

#include "syscall_c.h"
#include "Scheduler.h"

void* operator new (size_t);
void operator delete (void*);
class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();
    int start ();
    void join();
    static void dispatch ();


    static int sleep (time_t time){ return time_sleep(time); }//POPRAVITI
protected:
    Thread ();
    virtual void run () {}
private:
    thread_t myHandle; // dodao =0
    void (*body)(void*); void* arg;

    static void wrapper(void* _thread);
};



//class Semaphore

class Semaphore {
public:

    Semaphore (unsigned init);
    Semaphore();
    virtual ~Semaphore ();

    int wait ();
    int signal ();
    int tryWait();

private:
    sem_t myHandle;

};

class Console {
public:
    static char getc ();
    static void putc (char c);
};

#endif //PROJECT_BASE_V1_1_SYSCALL_CPP_H

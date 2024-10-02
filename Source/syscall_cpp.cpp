//
// Created by os on 8/16/23.
//

#include "../h/syscall_cpp.h"
#include "../lib/console.h"
#include "../h/printing.hpp"
void* operator new(size_t size){
    return mem_alloc(size);

}

void operator delete(void* adr){

    mem_free(adr);
}

Thread::Thread(void (*body)(void *), void *arg) {

    thread_createCPP(&myHandle, body, arg);

}

Thread::~Thread(){

    mem_free(myHandle);

}


int Thread::start(){
    Scheduler::put(myHandle);
    return 0;
}

void Thread::dispatch() {
    thread_dispatch();
}

void Thread::join(){
    thread_join(myHandle);
}

void Thread::wrapper(void* _thread){

    Thread* thread=(Thread*)_thread;
    thread->run();
}

Thread::Thread(){

    thread_createCPP(&myHandle, &wrapper, this);

}



Semaphore::Semaphore (unsigned init){

    sem_open(&myHandle, init);

}

Semaphore::Semaphore (){

    sem_open(&myHandle, 1);

}

Semaphore::~Semaphore (){

    sem_close(myHandle);

}


int Semaphore::signal() {
    return sem_signal(myHandle);

}

int Semaphore::wait() {

    return sem_wait(myHandle);
}

int Semaphore::tryWait() {
    return sem_trywait(myHandle);
}


//class Console

char Console::getc(){
    return __getc();

}
void Console::putc(char c) {
    __putc(c);
}
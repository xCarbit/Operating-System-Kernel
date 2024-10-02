//
// Created by os on 7/25/23.
//

#ifndef PROJECT_BASE_V1_1_TCB_H
#define PROJECT_BASE_V1_1_TCB_H

#include "../lib/hw.h"
#include "../h/Scheduler.h" //Mozda ne treba

#include "../h/MemoryAllocator.h"
#include "../h/syscall_c.h"


struct Blocked {

    TCB* data;
    Blocked* next;
};
class TCB {

public:
    using Body= void(*)(void *);

private:
   ushort num_blocked=0;

    struct Context {

        uint64 ra;
        uint64 sp; //Vrh steka

    };

    bool blockedThread;
    bool finished;
    int retValue; //Promenljiva koja povezuje semWait i semClose
    Body body;
    uint64 *stack; // Pocetak steka
    Context context;
    void * funArg;

    uint64 TimeSlice;

    Blocked *headBlck, *tailBlck;

public:
   friend class Riscv;

   void putBlocked(TCB* tcb);
   TCB* getBlocked();


    static TCB* createThread(Body body, void *arg);

    static TCB* createThread(Body _body, void* _funArg, uint64* _stack);
    static TCB* createThreadCPP(Body _body, void* _funArg, uint64* _stack);

    static void yield();


    bool isFinished()  {   return finished; }

     void  setBlocked(bool blocked);
     bool isBlocked();
     void setFinished(bool _finished) {finished = _finished;}

     void setTimeSlice(uint64 timeSLice){
         TimeSlice=timeSLice;

     }
     uint64 getTimeSlice(){return TimeSlice;}

     static TCB* running;
     static uint64 TimeSliceCounter;
     static  void dispatch();
     void unblock();

  void setRetValue(int value){
      retValue=value;
  }

int getRetValue(){

    return retValue;
  }
  static void threadJoin(thread_t handle){

      if(handle== nullptr) return;
      if(handle->isFinished()) return;
        running->setBlocked(true);
        handle->putBlocked(running);
        TCB::yield();
    }

    static void ContextSwitch(Context *oldContext, Context* newContext);
private:

    //static void ContextSwitch(Context *oldContext, Context* newContext);

    static TCB*  createTCB(Body _body,  void* _funArg, uint64* _stack);
    static  void wrapper(); //Nije moguce uzeti adresu nestaticke funkcije


  ~TCB(){

     if(stack!=nullptr) MemoryAllocator::Singleton().mem_free((char*)stack);

  }

};


#endif //PROJECT_BASE_V1_1_TCB_H

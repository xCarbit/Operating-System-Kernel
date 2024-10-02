//
// Created by os on 7/25/23.
//

#include "../h/TCB.h"
#include "../h/MemoryAllocator.h"
#include "../lib/mem.h"
#include "../lib/console.h"
#include "../h/riscv.h"

extern "C" void pushRegisters() ;
extern "C" void popRegisters() ;

TCB* TCB::running= nullptr;
uint64 TCB::TimeSliceCounter=0;

void TCB::putBlocked(TCB* tcb){

    Blocked* newElem=(Blocked*)(MemoryAllocator::Singleton().mem_alloc((sizeof(Blocked)+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE));
    newElem->data=tcb;
    newElem->next=nullptr;

    if(tailBlck!= nullptr){

        tailBlck->next=newElem;
        tailBlck=newElem;

    }
    else{

        headBlck=tailBlck=newElem;
    }

    num_blocked++;

}

TCB* TCB::getBlocked(){

    if(!headBlck) return 0;
   Blocked* elem=headBlck;
   headBlck=headBlck->next;
   if(!headBlck)tailBlck=0;
   TCB* res=elem->data;
   MemoryAllocator::Singleton().mem_free(elem);
   num_blocked--;
   return res;


}



void TCB::yield() {

  __asm__ volatile("li a0, 0x19");
  __asm__ volatile("ecall");
}

void TCB::dispatch() {
    TCB* old=running;

    if(!old->finished && !old->isBlocked())  {Scheduler::put(old);}
    running=Scheduler::get();
    TCB::ContextSwitch(&old->context, &running->context);

}
TCB* TCB::createThread(Body _body, void *_funArg){

    void* adr=(MemoryAllocator::Singleton().mem_alloc((sizeof(TCB)+ sizeof(Blocked)+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE));
    if(adr== nullptr) return 0;
    else {

        TCB* newThread=(TCB*)adr;

        uint64 stack = 0;
        void *stackAdr = nullptr;
        if (_body != nullptr) {

            stackAdr = (MemoryAllocator::Singleton().mem_alloc((DEFAULT_STACK_SIZE + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE));

            stack = (uint64) ((char *) stackAdr + DEFAULT_STACK_SIZE - sizeof(FreeMem));
            newThread->stack = (uint64 *) stackAdr; //GREEESKAAAA
        } else {

            newThread->stack = nullptr;
        }
        newThread->body = _body;
        newThread->funArg = _funArg;
        newThread->finished = false;
        newThread->retValue=0; //Povratna vrednost wait() kad se semafor brise dok na njemu ceka neka nit

        newThread->TimeSlice=2;

       newThread->blockedThread=false;
       newThread->headBlck= nullptr;
       newThread->tailBlck= nullptr;
       newThread->num_blocked=0;

        newThread->context = {(uint64) &wrapper, stack};

        if (_body != nullptr) Scheduler::put(newThread); // main ce sam sebe staviti u Scheduler

        return newThread;
    }

}


void TCB::setBlocked(bool blocked) {
  this->blockedThread=blocked;
}
bool TCB::isBlocked() {
    return this->blockedThread;
}


void TCB::unblock() {
    while(num_blocked>0){
        TCB* tcb=running->getBlocked();
        tcb->setBlocked(false);
        Scheduler::put(tcb);

    }
}



void TCB::wrapper(){

    TCB::TimeSliceCounter=0;

    Riscv::popSppSpie();

    running->body(running->funArg);
    running->setFinished(true);// Nit obavezno ugasiti nakon odradjene funkcije

   //Sve niti koje su cekale da se tekuca nit zavrsi treba iz
   // reda blokiranih prebaciti u Scheduler
    running->unblock();

    yield();

}


TCB* TCB::createThread(Body _body, void *_funArg, uint64* _stack) {

    TCB* tcb=createTCB(_body, _funArg, _stack);
    if(_body!= nullptr) Scheduler::put(tcb);

    return tcb;

}

TCB* TCB::createThreadCPP(Body _body, void* _funArg, uint64* _stack){
    TCB* tcb=createTCB(_body, _funArg, _stack);
    return tcb;

}

TCB* TCB::createTCB(Body _body,  void* _funArg, uint64* _stack){

    void* adr=(MemoryAllocator::Singleton().mem_alloc((sizeof(TCB)+sizeof(Blocked)+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE));
    if(adr== nullptr) return 0;
    else {

        TCB* newThread=(TCB*)adr;

        uint64 stack = 0;
        uint64 *stackAdr = _stack;
        if (_body != nullptr) {

            stack = (uint64) ((char *) stackAdr + DEFAULT_STACK_SIZE - sizeof(FreeMem));
            newThread->stack = stackAdr;
        } else {

            newThread->stack = nullptr;
            MemoryAllocator::Singleton().mem_free(stackAdr);
        }
        newThread->body = _body;
        newThread->funArg = _funArg;
        newThread->finished = false;
        newThread->num_blocked=0;
        newThread->context = {(uint64) &wrapper, stack};

        newThread->retValue=0; //Stajalo 1 NOOVOO

        newThread->TimeSlice=2;


        newThread->blockedThread=false;
        newThread->headBlck= nullptr;
        newThread->tailBlck= nullptr;
        newThread->num_blocked=0;



       // if (_body != nullptr) Scheduler::put(newThread); // main ce sam sebe staviti u Scheduler

        return newThread;
    }


}






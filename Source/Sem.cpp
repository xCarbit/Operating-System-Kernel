//
// Created by os on 7/30/23.
//

#include "../h/Sem.h"

void Sem::putBlocked(TCB* tcb){
    Blocked* newElem=(Blocked*)(MemoryAllocator::Singleton().mem_alloc((sizeof(Blocked)+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE));
    newElem->data=tcb;
    newElem->next=nullptr;

    if( tail!= nullptr){

        tail->next=newElem;
        tail=newElem;

    }
    else{
        head=tail=newElem;
    }

    numBlocked++;
}


TCB* Sem::getBlocked(){

    if(!head) return 0;
    Blocked* elem=head;
    head=head->next;
    if(!head)tail=0;
    TCB* res=elem->data;
    MemoryAllocator::Singleton().mem_free(elem);

    numBlocked--;
    return res;


}
int Sem::semOpen(Sem** handle, unsigned init){
    void *adr=MemoryAllocator::Singleton().mem_alloc((sizeof(Sem)+sizeof (Blocked)+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE);
    if(!adr || !handle) return -1;
    else {

        Sem **Handle = handle;
        Sem *newSem = (Sem *) adr;
        newSem->head = nullptr;
        newSem->tail = nullptr;
        newSem->val = init;
        newSem->numBlocked=0;
        newSem->closed=false;// NOVOOOOOOOO
        *Handle = newSem;
        return 0;
    }
}


 int Sem::semSignal(Sem* id){
    if(id== nullptr)  return -1;
   //if(id->isClosed()) return-1;
    id->val++;
    if(id->val<=0){

        TCB* readyTCB=id->getBlocked();
        readyTCB->setBlocked(false);
        Scheduler::put(readyTCB);

    }
    return 0;

}


 int Sem::semWait(Sem* id){


     if(id== nullptr)  return -1;
     //if(id->isClosed()) return-1;

    id->val--;
    if(id->val<0){

        id->putBlocked(TCB::running);
        TCB::running->setBlocked(true); //Da ne bi zavrsili u Scheculeru
        TCB::yield();


        return  TCB::running->getRetValue();
    }
    return 0;

}

 int Sem::semTryWait(Sem* id){

    if(id== nullptr) return -1;

    if(id->val>=1){
        id->val--;
        return 0;
    }
    return 1;

}


int Sem::semClose(Sem* handle){

    if(handle== nullptr)  return -1;
    //if(handle->isClosed()) return-1;

    while(handle->numBlocked>0){

        TCB *tcb=handle->getBlocked();
        tcb->setBlocked(false);
        Scheduler::put(tcb);
        tcb->setRetValue(-2);

       // handle->val++; //NOVOOO ??? Scenario: Jedna nit brise semafor dok su dve blokirane na njemu, A i B
                       // Da nema ovog uslova nit A bi vadila nit B iz reda blokrianih koji je PRAZAN
    }



    MemoryAllocator::Singleton().mem_free(handle);
    handle= nullptr;

    return 0;


}
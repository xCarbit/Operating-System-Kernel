//
// Created by os on 7/24/23.
//

#include "../h/Scheduler.h"
#include "../h/MemoryAllocator.h"

#include "../lib/mem.h"


Elem* Scheduler::head= nullptr;
Elem* Scheduler::tail= nullptr;

TCB* Scheduler::get(){


    if(head== nullptr) return nullptr;
    Elem* res;
    res=head;
    head=head->next;
    //U Slucaju da head pokazuje na jedini element u listi listu treba proglasiti praznom
    if(head== nullptr){
        tail=nullptr;
    }
    TCB* result=res->data;
    MemoryAllocator::Singleton().mem_free(res);
    return result;


}

void Scheduler::put(TCB *ccb) {

    Elem* newElem=(Elem*)(MemoryAllocator::Singleton().mem_alloc((sizeof(FreeMem)+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE));
    newElem->data=ccb;
    newElem->next=0;
    if(tail!= nullptr) {

        tail->next=newElem;
        tail=newElem;

    }

   else{
       head=tail=newElem;
   }

}
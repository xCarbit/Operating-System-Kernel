//
// Created by os on 7/22/23.
//

#include "../h/MemoryAllocator.h"
#include "../lib/hw.h"

#include "../lib/console.h"

void* MemoryAllocator::mem_alloc (size_t _size){

   static bool flag=false;
    void *result=nullptr;
    if(!flag){

        head=(FreeMem*)(HEAP_START_ADDR);
        head->size=(size_t)(((char*)(HEAP_END_ADDR)-(char*)(HEAP_START_ADDR))/MEM_BLOCK_SIZE);  //Velcina prostora
        flag=true;                                                                                                        // U blokovima

        head->next= nullptr;
        head->prev=nullptr;
    }

    for(FreeMem* curr=head;  curr!= nullptr; curr=curr->next){

        if(curr->size>=_size){

            result=curr;


            if(curr->size==_size) {
                if (curr->prev != nullptr) curr->prev->next = curr->next;
                else head = curr->next;
                if (curr->next != nullptr) curr->next->prev = curr->prev;
                curr->size=_size;  // potrebno radi implementacije funkcije mem_free
                 break;// Krademo of korisnika sizeof(FreeMem)) B
            }                                                    // Radi lakse implementacije funkcije mem_free

                FreeMem *newNode = (FreeMem *) ((uint64 *) (curr) + _size * MEM_BLOCK_SIZE);
                if (curr->prev != nullptr) curr->prev->next = newNode;
                else head = newNode;
                if (curr->next != nullptr) curr->next->prev = newNode;
                newNode->size = curr->size - _size;
                newNode->next = curr->next;
                newNode->prev = curr->prev;
                curr->size=_size; // potrebno radi implementacije funkcije mem_free
                break;

        }
    }
   if(result== nullptr) return nullptr;
    return ((uint64*)result+sizeof(FreeMem));

}


int MemoryAllocator::mem_free (void* adr)  {

   if((char*)adr<(char*)HEAP_START_ADDR || (char*)adr>=(char*)HEAP_END_ADDR) return -1;
   uint64* RealAdr=(uint64*)((uint64*)adr - sizeof(FreeMem));

    //Trazi element liste nakon koga treba osloboditi memoriju

    FreeMem* prevNode= nullptr;

    if(!head || (char*)adr<(char*)head) prevNode= nullptr;

    else{
        for(prevNode=head; prevNode->next!= nullptr && (char*)adr>(char*)(prevNode->next); prevNode=prevNode->next);

    }

    FreeMem* newNode=(FreeMem*)RealAdr;

    newNode->prev=prevNode;

    if(prevNode!= nullptr) newNode->next=prevNode->next;
    else newNode->next=head;
    if(newNode->next) newNode->next->prev=newNode;
    if(prevNode!= nullptr) prevNode->next=newNode;
    else head=newNode;

   //Pokusaj da spojis slobodne fragmente

    tryToJoin(newNode);
    tryToJoin(prevNode);

    adr= nullptr; //NOOVOOO
    return 0;




}


void MemoryAllocator::tryToJoin (FreeMem* cur){

    if(cur== nullptr) return;
    if(cur->next && (((char*)cur+cur->size*MEM_BLOCK_SIZE) ==(char*)(cur->next))){

        cur->size+=cur->next->size;
        cur->next=cur->next->next;
        if(cur->next!= nullptr) cur->next->prev=cur;
    }
}


MemoryAllocator& MemoryAllocator::Singleton(){

    static MemoryAllocator singleton;
    return singleton;

}


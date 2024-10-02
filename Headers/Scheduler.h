//
// Created by os on 7/24/23.
//

#ifndef PROJECT_BASE_V1_1_SCHEDULER_H
#define PROJECT_BASE_V1_1_SCHEDULER_H

#include "../h/TCB.h"

class TCB;
struct Elem {

    TCB* data;
    Elem* next;

};
class Scheduler {
private:



   static Elem* head;
   static Elem* tail;


public:

   static bool isEmpty(){
       if(head==nullptr) return true;
       else{
           return false;
       }
   }
   static TCB* get();
   static void put(TCB* ccb);

};


#endif //PROJECT_BASE_V1_1_SCHEDULER_H

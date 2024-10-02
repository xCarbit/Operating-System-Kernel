#include "../lib/hw.h"
#include "../h/syscall_c.h"
#include "../h/syscall_cpp.h"
#include "../h/TCB.h"
#include "../h/userMain.h"
#include "../lib/console.h"

extern "C" void supervisorTrap();

int main(){

    __asm__ volatile ("csrw stvec, %[vector]" : : [vector] "r"(&supervisorTrap));


  TCB* main;
  thread_create(&main, nullptr, nullptr);
  TCB::running=main;


  changeMode();
  userMain();
  while(!Scheduler::isEmpty())thread_dispatch();



  return 0;
}


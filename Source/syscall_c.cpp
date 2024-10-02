//
// Created by os on 7/23/23.
//

#include "../h/syscall_c.h"
#include "../lib/hw.h"
#include "../h/MemoryAllocator.h"
#include "../lib/console.h"
void* mem_alloc (size_t size)
{


    size_t RealSize; // Velicina zaokruzena na blokove i pretvorena u blokove
    uint64 code=0x01;
    uint64  result;
  /*  if(size% MEM_BLOCK_SIZE==0) RealSize=size/MEM_BLOCK_SIZE;
    else{
        RealSize=size/MEM_BLOCK_SIZE+1;
    }*/
  RealSize=(size+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE;

     //Pripremanje parametara

    __asm__ volatile ("mv a1,%[par1]" : :[par1]"r" (RealSize));
    __asm__ volatile ("mv a0,%[codeOper]" : :[codeOper]"r" (code));

    //Poziv ABI funkcije
    __asm__ volatile ("ecall");

    //Dohvatanje rezultata funkcic(je

    __asm__ volatile("mv %[res], a0" : [res]"=r" (result));

    return (void*)result;
}


int mem_free (void* adr){
    uint64  result;
    uint64 codeOper=0x02;
    void* adresa=adr;

    //Pripremanje parametara

    __asm__ volatile ("mv a1,%[par1]" : :[par1]"r" (adresa));
    __asm__ volatile ("mv a0,%[codeOper]" : :[codeOper]"r" (codeOper));

    //Poziv ABI funkcije
    __asm__ volatile ("ecall");

    //Dohvatanje rezultata funkcije

    __asm__ volatile("mv %[res], a0" : [res]"=r" (result));

    return result;

}


int thread_create (thread_t* handle, void(*start_routine)(void*),void* arg){


     void(* funAdr)(void *)=start_routine;
     void *funArg=arg;
     thread_t* Handle=handle;
     uint64 codeOper=0x11;

     int result;

     //Pripremanje parametara

     uint64* stackAdr= (uint64*) mem_alloc(DEFAULT_STACK_SIZE);
    __asm__ volatile ("mv a4,%[par4]" : :[par4]"r" (stackAdr));

     __asm__ volatile ("mv a3,%[par3]" : :[par3]"r" (funArg));
     __asm__ volatile ("mv a2,%[par2]" : :[par2]"r" (funAdr));
     __asm__ volatile ("mv a1,%[par1]" : :[par1]"r" (Handle));

     __asm__ volatile ("mv a0,%[codeOper]" : :[codeOper]"r" (codeOper));

     __asm__ volatile ("ecall");

     __asm__ volatile("mv %[res], a0" : [res]"=r" (result));

     return result;


}


int thread_createCPP (thread_t* handle, void(*start_routine)(void*),void* arg){


    void(* funAdr)(void *)=start_routine;
    void *funArg=arg;
    thread_t* Handle=handle;
    uint64 codeOper=0x29;

    int result;

    //Pripremanje parametara

    uint64* stackAdr= (uint64*) mem_alloc(DEFAULT_STACK_SIZE);
    __asm__ volatile ("mv a4,%[par4]" : :[par4]"r" (stackAdr));

    __asm__ volatile ("mv a3,%[par3]" : :[par3]"r" (funArg));
    __asm__ volatile ("mv a2,%[par2]" : :[par2]"r" (funAdr));
    __asm__ volatile ("mv a1,%[par1]" : :[par1]"r" (Handle));

    __asm__ volatile ("mv a0,%[codeOper]" : :[codeOper]"r" (codeOper));

    __asm__ volatile ("ecall");

    __asm__ volatile("mv %[res], a0" : [res]"=r" (result));

    return result;


}



int thread_exit (){

    __asm__ volatile("li a0, 0x12");
     __asm__ volatile ("ecall");
     return 0;
}

void thread_dispatch (){

    __asm__ volatile("li a0, 0x13");
    __asm__ volatile ("ecall");
     return ;
}

void thread_join (thread_t handle){


    //Pocetak novog

    //Kraj novog

    thread_t Handle=handle;

    __asm__ volatile ("mv a1,%[par1]" : :[par1]"r" (Handle));
    __asm__ volatile("li a0, 0x14");
    __asm__ volatile("ecall");

    return;

}


int sem_open (sem_t* handle, unsigned int init){

    unsigned int Init=init;
    __asm__ volatile ("mv a2,%[par2]" : :[par2]"r" (Init));
    sem_t* Handle=handle;
    __asm__ volatile ("mv a1,%[par1]" : :[par1]"r" (Handle));
    __asm__ volatile("li a0, 0x21");
    __asm__ volatile("ecall");

    uint64 result;
    __asm__ volatile("mv %[res], a0" : [res]"=r" (result));

     return result;

}

int sem_close (sem_t handle){


    sem_t Handle=handle;
    __asm__ volatile ("mv a1,%[par1]" : :[par1]"r" (Handle));
    __asm__ volatile("li a0, 0x22");
    __asm__ volatile("ecall");


    uint64 result=0;
   __asm__ volatile("mv %[res], a0" : [res]"=r" (result));


    return result;

}

int sem_wait (sem_t id){


    sem_t ID=id;
    __asm__ volatile ("mv a1,%[par1]" : :[par1]"r" (ID));
    __asm__ volatile("li a0, 0x23");
    __asm__ volatile("ecall");


     uint64  ret;
    __asm__ volatile("mv %[ret], a0" : [ret]"=r" (ret));

     return ret;

}

int sem_signal (sem_t id){

    sem_t ID=id;
    __asm__ volatile ("mv a1,%[par1]" : :[par1]"r" (ID));
    __asm__ volatile("li a0, 0x24");
    __asm__ volatile("ecall");

    uint64 result;
    __asm__ volatile("mv %[res], a0" : [res]"=r" (result));

    return result;

}

int sem_trywait (sem_t id){

    sem_t ID=id;
    __asm__ volatile ("mv a1,%[par1]" : :[par1]"r" (ID));
    __asm__ volatile("li a0, 0x26");
    __asm__ volatile("ecall");

    uint64 result;
    __asm__ volatile("mv %[res], a0" : [res]"=r" (result));

    return result;

}

void changeMode(){

    __asm__ volatile("li a0, 0x30");
    __asm__ volatile("ecall");

}


void putc(char c){

    char ch=c;
    __asm__ volatile ("mv a1,%[par1]" : :[par1]"r" (ch));
    __asm__ volatile("li a0,0x42 ");
    __asm__ volatile("ecall");


}

char getc(){
    __asm__ volatile("li a0,0x41 ");
    __asm__ volatile("ecall");

    char ret;
    __asm__ volatile("mv %[arg1], a0" : [arg1]"=r" (ret));
    return ret;

}

int time_sleep(time_t t){
    if(t>100) return 1;
    else return 0;
}
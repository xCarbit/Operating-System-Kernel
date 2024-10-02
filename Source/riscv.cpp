//
// Created by os on 8/1/23.
//

#include "../h/riscv.h"
#include "../lib/hw.h"
#include "../lib/console.h"
#include "../h/MemoryAllocator.h"
#include "../h/TCB.h"

#include "../h/Sem.h"

void Riscv::popSppSpie(){
    __asm__ volatile("csrw sepc, ra");
    mc_sstatus(1<<8);//Korisnicki rezim rada
    __asm__ volatile("sret");
}

void Riscv::handleSupervisorTrap() {

    uint64 volatile register4;
    __asm__ volatile("mv %[arg3], a4" : [arg3]"=r" (register4));

    uint64 CodeOper;
    __asm__ volatile("mv %[code], a0" : [code]"=r" (CodeOper));

    uint64 register1;
    __asm__ volatile("mv %[arg3], a1" : [arg3]"=r" (register1));

    uint64 register3;
    __asm__ volatile("mv %[arg3], a3" : [arg3]"=r" (register3));

    uint64 register2;
    __asm__ volatile("mv %[arg3], a2" : [arg3]"=r" (register2));


    uint64 scauseVar;
    __asm__ volatile("csrr %[ime],scause":[ime] "=r"(scauseVar));

    if(scauseVar==0x0000000000000009 || scauseVar==0x0000000000000008){

        uint64  volatile sepc=r_sepc()+4;
        uint64 volatile sstatus = r_sstatus();


        //mem_alloc ABI
        if(CodeOper==0x01){

            size_t size=register1;


            //Poziv odgovarajuce funkcije iz imlementacije jezgra
            void * res=(MemoryAllocator::Singleton()).mem_alloc(size);

            //Upis u registar rezultata funkcije

            __asm__ volatile ("mv a0,%[resultt]" : :[resultt]"r" (res));


        }
            //mem_free ABI
        else if(CodeOper==0x02){


            void* adr;
            adr=(void*)register1;

            //Poziv odgovarajuce funkcije iz imlementacije jezgra
            int res=(MemoryAllocator::Singleton()).mem_free(adr);

            //Upis u registar rezultata funkcije

            __asm__ volatile ("mv a0,%[resultt]" : :[resultt]"r" (res));

        }
            //thread_create ABI
        else if(CodeOper==0x11) {


            void (*adrFun)(void *);
            void* argFun= nullptr;

            if(register3!=0) argFun=(void*)register3;


            adrFun = (TCB::Body) register2;

            /*uint64 register4;
             __asm__ volatile("mv a4, sp");
             __asm__ volatile("addi a4,a4, 0x40");
             __asm__ volatile("ld a4, 14 * 8 (a4)");
             __asm__ volatile("mv %[arg1], a4" : [arg1]"=r" (register4));*/


            uint64 *_stackAdr = (uint64 *) register4;

            TCB *res = TCB::createThread(adrFun, argFun, _stackAdr);


            TCB **Handle = (TCB **) register1;
            *Handle = res;
            uint64 reg = 0;
            if (!res) reg = -1;
            __asm__ volatile ("mv a0,%[resultt]" : :[resultt]"r"(reg));

        }
            //thread_creatCPP
        else if(CodeOper==0x29){
            void (*adrFun)(void *);
            void* argFun= nullptr;

            if(register3!=0) argFun=(void*)register3;


            adrFun = (TCB::Body) register2;

            /* uint64 register4;
              __asm__ volatile("mv a4, sp");
              __asm__ volatile("addi a4,a4, 0x40");
              __asm__ volatile("ld a4, 14 * 8 (a4)");
              __asm__ volatile("mv %[arg1], a4" : [arg1]"=r" (register4));*/


            uint64 *_stackAdr = (uint64 *) register4;

            TCB *res = TCB::createThreadCPP(adrFun, argFun, _stackAdr);


            TCB **Handle = (TCB **) register1;
            *Handle = res;
            uint64 reg = 0;
            if (!res) reg = -1;
            __asm__ volatile ("mv a0,%[resultt]" : :[resultt]"r"(reg));


        }

            //thread_exit ABI
        else if(CodeOper==0x12){

            TCB::running->setFinished(true);
            TCB::dispatch();
        }
            //thread_dispatch ABI
        else if(CodeOper==0x13){

            TCB::yield();

        }
            //thread_join ABI
        else if(CodeOper==0x14){

            TCB* handle=(TCB*) register1;
            TCB::threadJoin(handle);


        }
            //sem_open ABI
        else if(CodeOper==0x21){


            sem_t* handle=(sem_t*)register1;


            unsigned init=(unsigned )register2;

            int res=Sem::semOpen(handle, init);

            __asm__ volatile ("mv a0,%[resultt]" : :[resultt]"r"(res));
        }
            //sem_close ABI
        else if(CodeOper==0x22){

            sem_t handle=(sem_t)register1;
            int res=Sem::semClose(handle);

            __asm__ volatile ("mv a0,%[retval]" : :[retval]"r"(res));

        }
            //sem_wait ABI
        else if(CodeOper==0x23){

            sem_t id=(sem_t)register1;
            int res=Sem::semWait(id);
            __asm__ volatile ("mv a0,%[resultt]" : :[resultt]"r"(res));
        }
            //sem_signal ABI
        else if(CodeOper==0x24){

            Sem* id=(Sem*)register1;
            int res=Sem::semSignal(id);

            __asm__ volatile ("mv a0,%[resultt]" : :[resultt]"r"(res));

        }
        //semTryWait
        else if (CodeOper==0x26){

            Sem* id= (Sem*)register1;
            int res=Sem::semTryWait(id);
            __asm__ volatile ("mv a0,%[resultt]" : :[resultt]"r"(res));

        }
            //yield ABI
        else if(CodeOper==0x19){

            TCB::dispatch();
            TCB::TimeSliceCounter=0;

        }
            //changeMode
        else if(CodeOper==0x30){
            w_sepc(sepc);
            w_sstatus(sstatus);
            mc_sip(SIP_SSIP);
            mc_sstatus(1<<8);//Korisnicki rezim rada
            return;


        }
            //putc
        else if(CodeOper==0x42){

            char c;
            __asm__ volatile("mv a1, sp");
            __asm__ volatile("addi a1, a1, 0x40");
            __asm__ volatile("ld a1, 11 * 8 (a1)");
            __asm__ volatile("mv %[arg1], a1" : [arg1]"=r" (c));
            __putc(c);
        }
            //getc
        else if(CodeOper==0X41){

            char ret=__getc();
            __asm__ volatile ("mv a0,%[par1]" : :[par1]"r" (ret));
        }
        w_sepc(sepc);
        w_sstatus(sstatus);
        mc_sip(SIP_SSIP);

    }


    else if(scauseVar==0x02){
        __putc('I');
    }

    else if (scauseVar== 0x05){
        __putc('C');
    }

    else if(scauseVar==0x07){

        __putc('U');
    }
    else {

        //Prekid od tajmera
        if (scauseVar == (0x01UL << 63 | 0x01)) {


            /*  TCB::TimeSliceCounter++;
               if(TCB::TimeSliceCounter>=TCB::running->getTimeSlice()){

                    uint64 sepc=r_sepc()+4;
                    uint64 sstatus=r_sstatus();
                    TCB::dispatch();
                    TCB::TimeSliceCounter=0;
                    w_sstatus(sstatus);
                    w_sepc(sepc);
                }
    */
            mc_sip(SIP_SSIP);

        }
        else if (scauseVar == 0x8000000000000009) {
            // __putc('k');  Stvara rekurziju jer u implementaciji __putc() se generise prekid od periferije
            console_handler();
        }
        else {


        }
    }

}
//
// Created by os on 8/1/23.
//

#ifndef PROJECT_BASE_V1_1_RISCV_H
#define PROJECT_BASE_V1_1_RISCV_H


#include "../lib/hw.h"

extern "C" void supervisorTrap();
class Riscv
{
public:
    enum BitMaskSip
    {
        SIP_SSIP = (1 << 1),
        SIP_STIP = (1 << 5),
        SIP_SEIP = (1 << 8),
    };
    // mask clear register sip
    static void mc_sip(uint64 mask);

    // mask clear register sstatus
    static void mc_sstatus(uint64 mask);

    // read register sstatus
    static uint64 r_sstatus();

    static uint64 r_sepc();

    static void w_sepc(uint64 sepc);

    // write register sstatus
    static void w_sstatus(uint64 sstatus);

    static void popSppSpie();

private:

    // supervisor trap handler
    static void handleSupervisorTrap();


};

inline uint64 Riscv::r_sstatus()
{
    uint64 volatile sstatus;
    __asm__ volatile ("csrr %[sstatus], sstatus" : [sstatus] "=r"(sstatus));
    return sstatus;
}
inline uint64  Riscv::r_sepc()
{

    uint64  sepc;
    __asm__ volatile ("csrr %[sepc], sepc" : [sepc] "=r"(sepc));
    return sepc;

}


inline void Riscv::w_sstatus(uint64 sstatus)
{
    __asm__ volatile ("csrw sstatus, %[sstatus]" : : [sstatus] "r"(sstatus));
}

inline void Riscv::w_sepc(uint64 sepc)
{

    __asm__ volatile ("csrw sepc, %[sepc]" : : [sepc] "r"(sepc));

}

inline void Riscv::mc_sip(uint64 mask)
{
    __asm__ volatile ("csrc sip, %[mask]" : : [mask] "r"(mask));
}

inline void Riscv::mc_sstatus(uint64 mask)
{
    __asm__ volatile ("csrc sstatus, %[mask]" : : [mask] "r"(mask));

}


#endif //PROJECT_BASE_V1_1_RISCV_H

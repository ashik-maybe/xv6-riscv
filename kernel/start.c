#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"

void main();
void timerinit();

// entry.S needs one stack per CPU.
__attribute__ ((aligned (16))) char stack0[4096 * NCPU];

// entry.S jumps here in machine mode on stack0.
void
start()
{
  // set M Previous Privilege mode to Supervisor, for mret.
  unsigned long x = r_mstatus();
  x &= ~MSTATUS_MPP_MASK;
  x |= MSTATUS_MPP_S;
  w_mstatus(x);

  // set M Exception Program Counter to main, for mret.
  // requires gcc -mcmodel=medany
  w_mepc((uint64)main);

  // disable paging for now.
  w_satp(0);

  // delegate all interrupts and exceptions to supervisor mode.
  w_medeleg(0xffff);
  w_mideleg(0xffff);
  w_sie(r_sie() | SIE_SEIE | SIE_STIE);

  // configure Physical Memory Protection to give supervisor mode
  // access to all of physical memory.
  w_pmpaddr0(0x3fffffffffffffull);
  w_pmpcfg0(0xf);

  // ask for clock interrupts.
  timerinit();

  // keep each CPU's hartid in its tp register, for cpuid().
  int id = r_mhartid();
  w_tp(id);

  // switch to supervisor mode and jump to main().
  asm volatile("mret");
}

// ask each hart to generate timer interrupts.
void
timerinit()
{
  // enable machine-mode interrupts.
  w_mstatus(r_mstatus() | MSTATUS_MIE);
  // enable machine-mode timer interrupts.
  w_mie(r_mie() | MIE_MTIE);
  
  // set the machine-mode timer's compare register.
  // QEMU's timer frequency is 10 MHz.
  // We set the next interrupt to occur in 0.1 second.
  volatile uint64 *mtime = (volatile uint64 *)0x0200bff8;
  volatile uint64 *mtimecmp = (volatile uint64 *)(0x02004000 + r_mhartid() * 8);
  *mtimecmp = *mtime + 10000000;  // 0.1 sec at 10 MHz
}

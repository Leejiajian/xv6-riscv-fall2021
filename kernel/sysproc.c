#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "date.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
// 添加的关于walk的声明
extern pte_t *walk(pagetable_t pagetable, uint64 va, int alloc);
uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  
  uint64 base; // pointer to base
  int npages;
  uint64 maskaddr, begin;
  pte_t *pte;
  if((argaddr(0, &base) < 0) || argint(1, &npages) < 0 || argaddr(2, &maskaddr) < 0)
    return -1;
  struct proc *p = myproc();
  pagetable_t pagetable = p->pagetable;
  uint64 bitmask = 0;   // 最多64页
  int cnt = 0;
  for(begin = base; begin < base + npages * PGSIZE; begin += PGSIZE) {
    if((pte = walk(pagetable, begin, 1)) == 0)
      return -1;
    if(*pte & PTE_A) {
      bitmask = bitmask | (1 << cnt);// 将对应的位置位
      *pte = *pte & (~PTE_A);   // 将PTE_A还原   
    }
    ++cnt;
    
  }

  // 为啥用数组就会不正确？
  /*
  uint64 base, maskaddr; // pointer to base;
  int npages;
  uint64 begin;
  pte_t * pte;
  uint8 bitmask[8]; 
  if(argaddr(0, &base) < 0 || argint(1, &npages) < 0 || argaddr(2, &maskaddr) < 0)
    return -1;
  struct proc *p = myproc();
  pagetable_t pagetable = p->pagetable;
  int cnt = 0;
  for(begin = base; begin < npages * PGSIZE; begin += PGSIZE) {
    if((pte = walk(pagetable, begin, 1)) == 0)
      return -1;
    if(*pte & PTE_A) {
      bitmask[cnt/8] = bitmask[cnt/8] | (1 << (cnt % 8));
      *pte = *pte & (~PTE_A);
    }
    ++cnt;
  }*/
  
  if(copyout(pagetable, maskaddr, (char *)&bitmask, sizeof(bitmask)) < 0) {
    return -1;
  }
  return 0; 
}
#endif

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

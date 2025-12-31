#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
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

int
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

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_draw(void){
	void* buf;
	uint size;
	argptr(0, (void*)&buf, sizeof(buf));
	argptr(1, (void*)&size, sizeof(size));
	char text[] = 
"         ___ . .  _\n"
" T$$$P    |  |_| |_\n"
" :$$$     |  | | |_\n"
" :$$$                                                      `T$$$$$$$b. \n"
" :$$$     .g$$$$$p.   T$$$$b.    T$$$$$bp.                   BUG    `Tb      T$b      T$P   .g$P^^T$$  ,gP^^T$$ \n"
"  $$$    d^`     `^b   $$  `Tb    $$    `Tb    .s^s. :sssp   $$$     :$; T$$P $^b.     $   dP`     `T :$P    `T \n"
"  :$$   dP         Tb  $$   :$;   $$      Tb  d'   `b $      $$$     :$;  $$  $ `Tp    $  d$           Tbp. \n"
"  :$$  :$;         :$; $$   :$;   $$      :$; T.   .P $^^    $$$    .dP   $$  $   ^b.  $ :$;            `T$$p.   \n"
"  $$$  :$;         :$; $$...dP    $$      :$;  `^s^' .$.     $$$...dP`    $$  $    `Tp $ :$;     `T$$      `T$b  \n"
"  $$$   Tb.       ,dP  $$```Tb    $$      dP ``$``$` `$`$^^  $$$``T$b     $$  $      ^b$  T$       T$ ;      $$; \n"
"  $$$    Tp._   _,gP   $$   `Tb.  $$    ,dP    $  $...$ $..  $$$   T$b    :$  $       `$   Tb.     :$ T.    ,dP  \n"
"  $$$;    `^$$$$$^`   d$$     `T.d$$$$$P^`     $  $```$ $``, $$$    T$b  d$$bd$b      d$b   `^TbsssP` 'T$bgd$P   \n"
"  $$$b.____.dP                                 $ .$. .$.$ss,d$$$b.   T$b.                                        \n"
".d$$$$$$$$$$P                                                         `T$b.                                      \n" ;
	if(sizeof(text) > size) {
		return -1;
	};

	strncpy((char*) buf, text, size);
	return sizeof(text);
}

int sys_thread_create(void) {
  int fcn;
  char* arg;
  char* stack;
  if (argint(0, &fcn) < 0)
    return -1;
  if (argstr(1, &arg) < 0)
    return -1;
  if (argstr(2, &stack) < 0)
    return -1;
  return thread_create((void(*)(void*))fcn, arg, stack);
}

int sys_thread_exit(void)
{
  thread_exit();
  return 0;
}

int sys_thread_join(void)
{
  return thread_join();
}


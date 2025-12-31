#include "types.h"
#include "stat.h"
#include "user.h"

static int x;

int sample_vals[15] = {30, 20, 10, 40, 70, 11, 35, 39, 60, 61, 73, 77, 41, 15, 25};
int get_random(){
  return sample_vals[(x++)%15];
}

int main(){
  // get a list of burst times
  int burst_vals[10];
  x = getpid()%15;
  for(int i=0; i<10; i++){
    burst_vals[i] = get_random();
  }

  printf(1, "PID\t Process type\t  Burst Time\tContext Switches\n");
  printf(1, "---\t ------------\t  ----- ----\t------- --------\n");

  for(int i=0; i<10; i++){
    if(fork() == 0) {
      if(set_burst_time(burst_vals[i]) < 0){
        printf(2, "%d: error setting burst time", getpid());
      }

      // separate newly created processes into 
      // 2 categories: CPU bound and IO bound

      if(i%2==0){
        // CPU bound process
        double temp = 0;
        for(int j=0; j<500000*burst_vals[i]; j++){
          temp += 3.14*2.71;
        }
        int computed_val = ((int)temp)%9000 + 1000;
        printf(1, "%d\t CPU Bound(%d) \t", getpid() ,computed_val);
      } 
      else {
        // IO bound process
				for(int j=0; j<10*burst_vals[i]; j++){
					sleep(1);
				}
				printf(1, "%d\t IO Bound    \t\t", getpid());
      }

			struct processInfo *info = (struct processInfo*) malloc(sizeof(struct processInfo));
			getProcInfo(getpid(), info);
			printf(1, " %d \t  %d\n", get_burst_time(), info->numberContextSwitches);
			exit();

    }
  }
  // wait for all child processes to finish
  while( wait() != -1 ) ; 

  exit();
}
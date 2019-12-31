#include <stdio.h>
#define MAX_NUMBER_OF_PROCESSES 100
#define NUMBER_OF_SCHEDULING_POLICIES 4
#define QUANTUM 1
#define TRUE 1
#define FALSE 0
#define start_time 0

typedef struct{
    int pid;
    int cbt;
    int wt;
    int tt;
    int priority;
    int finished; 
} process;

typedef struct{
    float avg_wt;
    float avg_tt;
} pair;

pair fifo(process* proc, int proc_num);
pair sjf(process* proc, int proc_num);
pair priority_scheduling(process* proc, int proc_num);
pair round_robin(process* proc, int proc_num);


int main(){
    process proc[MAX_NUMBER_OF_PROCESSES];
    pair (*policy_func[NUMBER_OF_SCHEDULING_POLICIES])(process*,int);
    policy_func[0] = fifo;
    policy_func[1] = sjf;
    policy_func[2] = priority_scheduling;
    policy_func[3] = round_robin;
    int proc_num;
    int policy;
    int pid = 0;

    printf("How many process do you have? ");
    scanf("%d", &proc_num);
    for (int i = 0; i < proc_num; i++)
    {
        int proc_cbt;
        int proc_priority;
        printf("--P[%d]--\n", i+1);
        printf("CBT: ");
        scanf("%d", &proc_cbt);
        printf("Priority: ");
        scanf("%d", &proc_priority);
        proc[i].cbt = proc_cbt;
        proc[i].priority = proc_priority;
        proc[i].finished = FALSE;
        proc[i].pid = pid++;
        proc[i].tt = 0;
        proc[i].wt = 0;
    }
    printf("Loop finished\n");
    printf("Run with scheduling: (0:fifo, 1:sjf, 2:priority, 3:RR) ");
    scanf("%d", &policy);
    pair tpl = policy_func[policy](proc,proc_num);
    printf("Average wt: %f, Average tt: %f\n", tpl.avg_wt, tpl.avg_tt);
}

pair fifo(process* proc, int proc_num){
    int time = 0;
    int wt = 0;
    float sum_wt = 0;
    int tt = 0;
    float sum_tt = 0;
    for (int i = 0; i < proc_num; i++){
        if(!proc[i].finished){
            proc[i].wt = time - start_time;
            time += proc[i].cbt;
            proc[i].tt = time - start_time;
            proc[i].finished = TRUE;
        }
    }

    for (int i = 0; i < proc_num; i++){
        sum_wt += proc[i].wt;
        sum_tt += proc[i].tt;
    }
    
    pair return_pair = {.avg_wt = sum_wt/proc_num, .avg_tt = sum_tt/proc_num};
    return return_pair;
}

pair sjf(process* proc, int proc_num){
    int time = 0;
    int wt = 0;
    float sum_wt = 0;
    int tt = 0;
    float sum_tt = 0;
    int min_cbt = 99999;
    int min_cbt_index = 0;
    for (int i = 0; i < proc_num; i++){
        min_cbt = 99999;
        for (int j=0; j<proc_num; j++){
            if (!proc[j].finished){
                if(min_cbt > proc[j].cbt){
                    min_cbt = proc[j].cbt;
                    min_cbt_index = j;
                }
            }
        }
        proc[min_cbt_index].wt=time - start_time;
        time += proc[min_cbt_index].cbt;
        proc[min_cbt_index].tt=time - start_time;
        proc[min_cbt_index].finished=TRUE;
    }

    for (int i = 0; i < proc_num; i++)
    {
        sum_wt += proc[i].wt;
        sum_tt += proc[i].tt;
    }
    
    pair return_pair = {.avg_wt = sum_wt/proc_num, .avg_tt = sum_tt/proc_num};
    return return_pair;
}

pair priority_scheduling(process* proc, int proc_num){
    int time = 0;
    int wt = 0;
    float sum_wt = 0;
    int tt = 0;
    float sum_tt = 0;
    int min_priority = 99999;
    int min_priority_index = 0;
    for (int i = 0; i < proc_num; i++){
        min_priority = 99999;
        for (int j=0; j<proc_num; j++){
            if (!proc[j].finished){
                if(min_priority > proc[j].priority){
                    min_priority = proc[j].priority;
                    min_priority_index = j;
                }
            }
        }
        proc[min_priority_index].wt = time - start_time;
        time += proc[min_priority_index].cbt;
        proc[min_priority_index].tt = time - start_time;
        proc[min_priority_index].finished=TRUE;
    }
    
    for (int i = 0; i < proc_num; i++)
    {
        sum_wt += proc[i].wt;
        sum_tt += proc[i].tt;
    }
    
    pair return_pair = {.avg_wt = sum_wt/proc_num, .avg_tt = sum_tt/proc_num};
    return return_pair;
}

pair round_robin(process* proc, int proc_num){
    int time = 0;
    int wt = 0;
    float sum_wt = 0;
    int tt = 0;
    float sum_tt = 0;
    int min_priority = 0;
    int min_priority_index = 0;
    process proc_backup[MAX_NUMBER_OF_PROCESSES];
    int max_cbt = 0;
    for (int i = 0; i < proc_num; i++){
        if(max_cbt < proc[i].cbt)
            max_cbt = proc[i].cbt;
        proc_backup[i].cbt = proc[i].cbt;
        proc_backup[i].priority = proc[i].priority;
        proc_backup[i].finished = proc[i].finished;
        proc_backup[i].pid = proc[i].pid;
        proc_backup[i].tt = proc[i].tt;
        proc_backup[i].wt = proc[i].wt;
    }
    
    while(max_cbt > 0){
        for (int i = 0; i < proc_num; i++){
            if(!proc[i].finished){
                proc[i].cbt -= QUANTUM;
                time += QUANTUM;
                if(proc[i].cbt == 0){
                    proc[i].finished = TRUE;
                    proc[i].tt = time - start_time;
                    proc[i].wt = time - start_time - proc_backup[i].cbt;
                }
            }
        }
        max_cbt -= QUANTUM;
    }
    
    for (int i = 0; i < proc_num; i++){
        sum_wt += proc[i].wt;
        sum_tt += proc[i].tt;
    }
    
    pair return_pair = {.avg_wt = sum_wt/proc_num, .avg_tt = sum_tt/proc_num};
    return return_pair;
}
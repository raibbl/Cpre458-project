/*******************************************************************************
*
* CprE 308 Scheduling Lab
*
* scheduling.c
*******************************************************************************/
#include <stdio.h>
#include <string.h>
int numoftasks = 0;
struct task
{
    /* Values initialized for each task */
    int period;   /*the interval of the the task*/
    int runtime;  /* Time task requires to complete job */
    int priority; /* Priority of the task */
    int arrivaltime;
    /* Values algorithm may use to track tasks */
    int starttime;
    int endtime;
    int flag;
    int remainingtime;
    int orignalindex;
};

/* Forward declarations of Scheduling algorithms */
void first_come_first_served(struct task *proc);
void shortest_remaining_time(struct task *proc);
void checkfeasibility(int numoftasks,struct task *proc);

int main()
{
    int i;
    int numoftasks;
    printf("enter the number of tasks needed ");
    scanf("%d", &numoftasks);
    struct task proc[numoftasks], /* List of taskes */
        proc_copy[numoftasks];    /* Backup copy of taskes */

    /* Initialize task structures */
    for (i = 0; i < numoftasks; i++)
    {
        printf("\n Enter Execution Time for task %d\n", (i + 1));
        scanf("%d", &proc[i].runtime);
        printf("\n Enter Period for task %d\n", (i + 1));
        scanf("%d", &proc[i].period);
    }

    /* Show task values */
    printf("  task and ci and pi");
   
    for (i = 0; i < numoftasks; i++)
    {
        printf("  Task %d  (%d,%d)", i + 1, proc[i].runtime,
               proc[i].period);
    }

    //check schedubality
    memcpy(proc_copy, proc, numoftasks * sizeof(struct task));
   
    checkfeasibility(numoftasks,proc_copy);


    return 0;
}

/*return 1 if task set can be scheduled by edf, o otherwise*/
int checkedf(int numoftasks,struct task *proc)
{
    int i = 0;
    double sumofutli = 0;
    
    printf("num of tasks: %d\n",numoftasks);
    for (i = 0; i < numoftasks; i++)
    {   
        
       
        double ci = proc[i].runtime;
        double pi = proc[i].period;
        
        double utiloftask = ci/pi;
        
        sumofutli = sumofutli + utiloftask;
    }

    printf("\ntask set utlization is %lf\n", sumofutli);

    if (sumofutli <= 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void checkrms(int numoftasks,struct task *proc)
{
}

void checkfeasibility(int numoftasks,struct task *proc)
{
    int edf = checkedf(numoftasks,proc);

    if (edf == 1)
    {
        printf("task set is schedulable by edf");
    }
    else
    {
        printf("task is not schedulable by edf");
    }
}

void first_come_first_served(struct task *proc)
{
    //sort task first by arrival time
    int i = 0;
    for (i = 0; i < numoftasks; i++)
    {
        int j = 0;
        for (j = i; j < numoftasks; j++)
        {
            if (proc[i].arrivaltime >= proc[j].arrivaltime)
            {

                struct task temp;
                temp = proc[i];
                proc[i] = proc[j];
                proc[j] = temp;
            }
        }
    }
    //now print them
    int time = proc[0].arrivaltime;
    int avgsum = 0;
    for (i = 0; i < numoftasks; i++)
    {

        proc[i].starttime = time;
        time = time + proc[i].runtime;
        proc[i].endtime = time;
        avgsum += (proc[i].endtime - proc[i].arrivaltime);
        printf("Process %d started at time %d\n", proc[i].orignalindex, proc[i].starttime);
        printf("Process %d finished at time %d\n", proc[i].orignalindex, proc[i].endtime);
    }
    printf("Average time from arrival to finish is %d seconds", avgsum / numoftasks);
}

void shortest_remaining_time(struct task *proc)
{
    int i = 0;
    int avgsum = 0;
    for (i = 0; i < numoftasks; i++)
    {

        int j = 0;
        for (j = i; j < numoftasks; j++)
        {
            if (proc[i].arrivaltime >= proc[j].arrivaltime)
            {
                struct task temp;
                temp = proc[i];
                proc[i] = proc[j];
                proc[j] = temp;
            }
        }
    }

    int time = 0;
    int saveindex;
    int all = numoftasks;
    int shortest = 0;

    while (all != 0)
    {
        saveindex = -1;
        shortest = 100000000;

        for (i = 0; i < numoftasks; i++)
        {
            if (proc[i].arrivaltime <= time && !proc[i].flag &&
                proc[i].remainingtime < shortest)
            {
                saveindex = i;
                shortest = proc[i].remainingtime;
            }
        }

        if (saveindex > -1)
        {
            if (proc[saveindex].remainingtime == proc[saveindex].runtime)
            {
                proc[saveindex].starttime = time;
                printf("Process %d started at time %d\n", proc[saveindex].orignalindex, proc[saveindex].starttime);
            }

            proc[saveindex].remainingtime -= 1;
            time += 1;

            if (proc[saveindex].remainingtime == 0)
            {
                proc[saveindex].flag = 1;
                proc[saveindex].endtime = time;
                all--;
                printf("Process %d finished at time %d\n", proc[saveindex].orignalindex, proc[saveindex].endtime);

                avgsum += proc[saveindex].endtime - proc[saveindex].arrivaltime;
            }
        }
        else
        {
            time++;
        }
    }

    printf("Average time from arrival to completion is %d seconds\n",
           avgsum / numoftasks);
}

void round_robin(struct task *proc)
{
    int i;
    int avgsum = 0;
    int time = 0;
    int q = 10;
    int all = numoftasks;
    int saveindx;
    int current = 0;

    for (i = 0; i < numoftasks; i++)
    {

        int j = 0;
        for (j = i; j < numoftasks; j++)
        {
            if (proc[i].arrivaltime >= proc[j].arrivaltime)
            {
                struct task temp;
                temp = proc[i];
                proc[i] = proc[j];
                proc[j] = temp;
            }
        }
    }

    while (all != 0)
    {
        saveindx = -1;

        for (i = current + 1; i < numoftasks; i++)
        {
            if (proc[i].arrivaltime <= time && !proc[i].flag)
            {
                saveindx = i;
                break;
            }
        }

        if (saveindx < 0)
        {
            current = 0;
            for (i = current; i < numoftasks; i++)
            {
                if (proc[i].arrivaltime <= time && !proc[i].flag)
                {
                    saveindx = i;
                    break;
                }
            }
        }

        if (saveindx > -1)
        {
            if (proc[saveindx].remainingtime == proc[saveindx].runtime)
            {

                proc[saveindx].starttime = time;
                printf("Process %d started at time %d\n", saveindx, proc[saveindx].starttime);
            }
            if (proc[saveindx].remainingtime <= q)
            {

                time += proc[saveindx].remainingtime;
                proc[saveindx].remainingtime = 0;
                proc[saveindx].flag = 1;
                proc[saveindx].endtime = time;
                all--;
                printf("Process %d finished at time %d\n", saveindx, proc[saveindx].endtime);

                avgsum += proc[saveindx].endtime - proc[saveindx].arrivaltime;
            }
            else
            {

                proc[saveindx].remainingtime -= q;
                time += q;
            }
            current = saveindx;
        }
        else
        {
            time++;
            current = 0;
        }
    }

    printf("Average time from arrival to completion is %d seconds\n",
           avgsum / numoftasks);
}

void round_robin_priority(struct task *proc)
{
    int i;
    int avgsum = 0;
    int time = 0;
    int q = 10;
    int all = numoftasks;
    int saveindx;
    int current = 0;
    int highest_priority;

    for (i = 0; i < numoftasks; i++)
    {

        int j = 0;
        for (j = i; j < numoftasks; j++)
        {
            if (proc[i].arrivaltime >= proc[j].arrivaltime)
            {
                struct task temp;
                temp = proc[i];
                proc[i] = proc[j];
                proc[j] = temp;
            }
        }
    }

    while (all != 0)
    {
        saveindx = -1;

        highest_priority = 0;
        for (i = 0; i < numoftasks; i++)
        {
            if (proc[i].arrivaltime <= time && !proc[i].flag &&
                highest_priority < proc[i].priority)
            {
                highest_priority = proc[i].priority;
                break;
            }
        }

        for (i = 0; i < numoftasks; i++)
        {
            if (proc[i].arrivaltime <= time && !proc[i].flag &&
                highest_priority == proc[i].priority)
            {
                saveindx = i;
                break;
            }
        }

        if (saveindx > -1)
        {
            if (proc[saveindx].remainingtime == proc[saveindx].runtime)
            {

                proc[saveindx].starttime = time;
                printf("Process %d started at time %d\n", saveindx, proc[saveindx].starttime);
            }
            if (proc[saveindx].remainingtime <= q)
            {

                time += proc[saveindx].remainingtime;
                proc[saveindx].remainingtime = 0;
                proc[saveindx].flag = 1;
                proc[saveindx].endtime = time;
                all--;
                printf("Process %d finished at time %d\n", saveindx, proc[saveindx].endtime);

                avgsum += proc[saveindx].endtime - proc[saveindx].arrivaltime;
            }
            else
            {

                proc[saveindx].remainingtime -= q;
                time += q;
            }
            current = saveindx;
        }
        else
        {
            time++;
            current = 0;
        }
    }

    printf("Average time from arrival to completion is %d seconds\n",
           avgsum / numoftasks);
}
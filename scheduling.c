
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
int numoftasks = 0;
struct task
{
    /* Values initialized for each task */
    int period;   /*the interval of the the task*/
    int runtime;  /* Time task requires to complete job */
    int priority; /* Priority of the task */
    int arrivaltime;
    int nextarrivaltime;
    int instancesP;    /*according to LCM how many are possible*/
    int instancesleft; /*how many instances left*/
    /* Values algorithm may use to track tasks */
    int starttime;
    int endtime;
    int flag;
    int remainingtime;
    int orignalindex;
};

/* Forward declarations of Scheduling algorithms */
void first_come_first_served(struct task *proc);
void rms(int numoftasks, struct task *proc);
void checkfeasibility(int numoftasks, struct task *proc);

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
    printf("\n\n Task Set: \n");

    for (i = 0; i < numoftasks; i++)
    {
        printf("  Task %d  (%d,%d)", i + 1, proc[i].runtime,
               proc[i].period);
    }

    //check schedubality
    // memcpy(proc_copy, proc, numoftasks * sizeof(struct task));

    // checkfeasibility(numoftasks, proc);
    rms(numoftasks, proc);

    return 0;
}

float tasksetutil(int numoftasks, struct task *proc)
{
    int i = 0;
    double sumofutli = 0;
    for (i = 0; i < numoftasks; i++)
    {

        double ci = proc[i].runtime;
        double pi = proc[i].period;

        double utiloftask = ci / pi;

        sumofutli = sumofutli + utiloftask;
    }

    printf("\n\ntask set utlization is %lf\n\n", sumofutli);
    return sumofutli;
}
/*return 1 if task set can be scheduled by edf, o otherwise*/
int checkedf(int numoftasks, struct task *proc)
{
    int i = 0;

    double sumofutli = tasksetutil(numoftasks, proc);

    if (sumofutli <= 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*returns the task with lowest priorty*/

int findlowestp(int numoftasks, struct task *proc)
{
    int i = 0;
    int max = 0;
    int indexofmax = 0;

    for (i = 0; i < numoftasks; i++)
    {

        double pi = proc[i].period;

        if (pi > max)
        {
            max = pi;
            indexofmax = i;
        }
    }

    return indexofmax;
}

int checkrms(int numoftasks, struct task *proc)
{
    double sumofutli = tasksetutil(numoftasks, proc);

    double x = (numoftasks);
    double y = 1 / x;
    double rmscheck = x * (pow(2.0, y) - 1);

    if (sumofutli <= rmscheck)
    {

        return 1;
    }
    else
    {

        //exact analysis required

        //first find lowest piorty task;
        int taskwithlowestpindex = findlowestp(numoftasks, proc);
        double cioflowestp = proc[taskwithlowestpindex].runtime;
        double pioflowestp = proc[taskwithlowestpindex].period;

        int wc = 0;
        int i = 0;
        int old_wc;

        do
        {
            old_wc = wc;
            wc = 0;
            if (i == 0)
            {
                wc = cioflowestp; //just ci of lowest task priorty
                i++;
            }
            else if (i == 1)
            {
                int j;

                for (j = 0; j < numoftasks; j++)
                {
                    if (j != taskwithlowestpindex)
                    {

                        double othertaskpi = proc[j].period;
                        //printf("  %lf",othertaskpi);
                        double ratio = (cioflowestp / othertaskpi);
                        // printf("  %lf",ratio);

                        wc = wc + ceil(ratio) * proc[j].runtime;
                    }
                }
                i++;

                wc = wc + cioflowestp;
                printf("  %d", wc);
                if (wc > pioflowestp)
                {
                    //not schduable
                    return 0;
                }
            }
            else
            {

                int j;

                for (j = 0; j < numoftasks; j++)
                {
                    if (j != taskwithlowestpindex)
                    {

                        double othertaskpi = proc[j].period;
                        //printf("  %lf",othertaskpi);
                        double ratio = (old_wc / othertaskpi);
                        //printf("  %lf",ratio);

                        wc = wc + ceil(ratio) * proc[j].runtime;
                    }
                }
                i++;

                wc = wc + cioflowestp;
                printf("  %d", wc);
                if (wc > pioflowestp)
                {
                    //not schduable
                    return 0;
                }
            }

        } while (wc != old_wc);

        return 1; //exact analysis confirms test
    }
}

void checkfeasibility(int numoftasks, struct task *proc)
{
    int edf = checkedf(numoftasks, proc);
    int rms = checkrms(numoftasks, proc);

    if (edf == 1)
    {
        printf("task set is schedulable by edf\n");
    }
    else
    {
        printf("task is not schedulable by edf\n");
    }

    if (rms == 1)
    {
        printf("task set is schedulable by rms\n");
    }
    else
    {
        printf("task is not schedulable by rms\n");
    }
}

/*helper function to find gcd for lcm*/
int gcd(int a, int b)
{
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

/*function to find LCM of tasks*/

int lcmtasks(int numoftasks, struct task *proc)
{
    int res = 1, i;
    for (i = 0; i < numoftasks; i++)
    {
        res = res * proc[i].period / gcd(res, proc[i].period);
    }
    return res;
}

/*are all tasks done? if 1 then yes, 0 if no */
int alltasksdone(int numoftasks, struct task *proc)
{
    int i = 0;
    for (i = 0; i < numoftasks; i++)
    {
        if (!(proc[i].instancesleft == 0))
        {
            return 0;
        }
    }
    return 1;
}

void rms(int numoftasks, struct task *proc)
{
    //sort task first by period
    int i = 0;
    for (i = 0; i < numoftasks; i++)
    {
        int j = 0;
        for (j = i; j < numoftasks; j++)
        {
            if (proc[i].period >= proc[j].period)
            {

                struct task temp;
                temp = proc[i];
                proc[i] = proc[j];
                proc[j] = temp;
            }
        }
    }

    //find LCM

    double lcm = lcmtasks(numoftasks, proc);

    printf(" LCM = %lf\n", lcm);

    //how many insances of each task? && iti;ize each task
    for (i = 0; i < numoftasks; i++)
    {
        double taskperiod = proc[i].period;
        proc[i].instancesP = floor(lcm / taskperiod);
        printf("instance::: %d", proc[i].instancesP);
        proc[i].instancesleft = proc[i].instancesP;
        proc[i].remainingtime = proc[i].runtime;
        proc[i].arrivaltime = proc[i].period;
    }

    //RMS schduler:
    int time = 0; //track time through scheduling
    i = 0;

    while (time != lcm)
    {
        if (alltasksdone(numoftasks, proc) == 1)
        {
            break;
        }
        printf("LCM:::%lf\n", lcm);
        printf("TIME:::%d\n", time);
        printf("IIIII:::%d\n", i);
        //loop all sorted tasks

        //check if we have an idle status
        if (i == numoftasks)
        {

            i = 0;
            printf("iIIIIMEEMEM %d\n", i);
            //time++;
        }

        while (i < numoftasks)
        {
            int prempit = 0;
            proc[i].nextarrivaltime = proc[i].arrivaltime + proc[i].period;
            printf("nextarrivaltime %d\n", proc[i].nextarrivaltime);
            int timetocheck = -1;
            //check if we need to check antything before this priorty wise

            //checking priorty
            int x = 1;
            while (i - x >= 0)
            {
                //if this is not the priorty task
                timetocheck = proc[i - x].arrivaltime; //set the time to check for the last pririty task.
                x++;
            }

            printf("%d ARRIVAL TIME\n", proc[i].arrivaltime);
            printf("%d check TIME\n", timetocheck);

            if (proc[i].arrivaltime == time)
            {
                proc[i].flag = 00;
            }
            if (proc[i].flag != 12)
            {
                proc[i].starttime = time;
            }

            //printf("STARTTIME %d\n",proc[i].starttime);
            //schedule while there is ci left and there no priorty task arriving
            while (proc[i].remainingtime != 0 && timetocheck != time && proc[i].instancesleft > 0)
            {
                //printf("im here");
                if (proc[i].flag != 12)
                {
                    printf("START TIME RIGHT NOW IS %d\n", proc[i].starttime);

                    time = time + 1;
                    proc[i].remainingtime = proc[i].remainingtime - 1;
                }
                else
                {

                   
                    
                        time++;
                        if ((proc[i].arrivaltime == time))
                        {
                            proc[i].flag = 00;
                            proc[i].starttime = time;
                        }
                    }

                    //if we reached a higher priorty
                    if (time == timetocheck && (i - 1 >= 0))
                    {
                        //printf("im here");
                        proc[i].endtime = time;
                        //go back to more priorty task
                        printf("Task %d,started at time %d and ended at time %d hmm\n ", i, proc[i].starttime, proc[i].endtime);

                        //if ended this instance ended
                        if (proc[i].remainingtime == 0)
                        {

                            proc[i].remainingtime = proc[i].runtime;

                            proc[i].flag = 12; //to mark its done this instance
                            if (proc[i].instancesleft == proc[i].instancesP)
                            {
                                proc[i].arrivaltime = proc[i].period;
                            }
                            else
                            {
                                proc[i].arrivaltime = proc[i].arrivaltime + proc[i].period;
                            }
                            proc[i].instancesleft--;
                        }

                        i--;
                        prempit = 1;
                        timetocheck = -1;

                        break;
                    }
                }

                if (prempit != 1)
                {
                    proc[i].endtime = time;
                    //if ended this instance ended
                    if (proc[i].remainingtime == 0)
                    {
                        proc[i].remainingtime = proc[i].runtime;

                        proc[i].flag = 12; //to mark its done this instance
                        if (proc[i].instancesleft == proc[i].instancesP)
                        {
                            proc[i].arrivaltime = proc[i].period;
                        }
                        else
                        {
                            proc[i].arrivaltime = proc[i].arrivaltime + proc[i].period;
                        }
                        proc[i].instancesleft--;
                    }
                    if (proc[i].starttime != proc[i].endtime)
                    {
                        printf("Task %d,started at time %d and ended at time %d \n", i, proc[i].starttime, proc[i].endtime);
                    }
                    i++;
                }
            }
            // sleep(2);
        }
    }

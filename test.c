
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
    int arrivaltimes[100];
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
void edf(int numoftasks, struct task *proc);


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

    // check schedubality
   
    // memcpy(proc_copy, proc, numoftasks * sizeof(struct task));

    // checkfeasibility(numoftasks, proc);
   // rms(numoftasks, proc);
    edf(numoftasks,proc);

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

void schdulerms(int time, int remaining_time)
{
    time++;
    remaining_time--;
}

void rms(int numoftasks, struct task *proc)
{
    //sort task first by period
    int i = 0;
    int j = 0;
    for (i = 0; i < numoftasks; i++)
    {

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

    //how many insances of each task? && intilize each task
    for (i = 0; i < numoftasks; i++)
    {
        double taskperiod = proc[i].period;
        proc[i].instancesP = floor(lcm / taskperiod);
        printf("instance::: %d", proc[i].instancesP);
        proc[i].instancesleft = proc[i].instancesP;
        proc[i].remainingtime = proc[i].runtime;
        proc[i].arrivaltime = proc[i].period;
        proc[i].priority = 1 + i;
    }

    //populate arrivaltimes
    for (i = 0; i < numoftasks; i++)
    {

        for (j = 0; j < proc[i].instancesP; j++)
        {
            proc[i].arrivaltimes[j] = proc[i].period * (j + 1);
        }
    }

    //RMS schduler:
    int time = 0; //track time through scheduling
    i = 0;

    while (lcm != time && alltasksdone(numoftasks, proc) != 1)
    {
        int x = 0;
        for (x = 0; x < numoftasks; x++)
        {
            if (time >= proc[x].arrivaltime)
            {
                proc[x].flag = 00;
            }
        }
        //schduling and priorty block
        i = 0;
        while (i < numoftasks)
        {
            //flag handling block
            //remove the done flag once we enter a new period

            int x = 0;
            for (x = 0; x < numoftasks; x++)
            {
                if (time >= proc[x].arrivaltime)
                {
                    proc[x].flag = 00;
                }
            }

            //handle top most priorty task
            if (i - 1 < 0)
            {

                if (proc[i].flag != 12)
                { //check if it is done or not

                    //this is where  the start time would be
                    proc[i].starttime = time;
                    while (proc[i].remainingtime > 0)
                    { //while still there is ci to schedule
                        time++;
                        proc[i].remainingtime--;
                    }
                    //this place is where schduling would end
                    proc[i].endtime = time;
                    proc[i].instancesleft--;
                    proc[i].remainingtime = proc[i].runtime;
                    proc[i].arrivaltime = proc[i].arrivaltimes[abs(proc[i].instancesP - proc[i].instancesleft) - 1]; //this the next arrival time
                    proc[i].flag = 12;                                                                               //done

                    printf("Task %d Started from %d till time %d\n", i, proc[i].starttime, proc[i].endtime);
                    i++;
                }
                else
                { //if it is done we check other tasks
                    int something_mightbethere = 5000;
                    for (j = 0; j < numoftasks; j++)
                    { //check if any of the tasks higher priorty have an arrival time
                        if (time >= proc[j].arrivaltime)
                        {
                            something_mightbethere = 20;
                        }
                        //check if there is a task not done and needs completeion
                        if (proc[j].flag != 12 && proc[i].flag == 12)
                        {
                            if (!something_mightbethere == 20)
                            {
                                something_mightbethere = j;
                            }
                        }
                    }

                    //if there is nothing there yet and this task is done then move on with time
                    if (proc[i].flag == 12 && something_mightbethere != 20)
                    {
                        time++;
                    }
                    else
                    {
                        if (something_mightbethere != 20 && something_mightbethere != 5000)
                        {
                            i = something_mightbethere; //there is a task that is  not finished
                        }
                        else
                        {
                            i++; //we move to other tasks
                        }
                    }
                }
            }
            else
            { //im not the most priorty task
                int Higher_exsitance_coming = 0;
                int something_mightbethere = 600;
                int compare_me = 50000;
                int compare_me2 = 100;
                int breaking = 0;
                proc[i].starttime = time;

                for (j = 0; j < i; j++)
                { //check if any of the tasks higher priorty have an arrival time
                    if (time >= proc[j].arrivaltime)
                    {
                        something_mightbethere = j;
                        compare_me = something_mightbethere;
                    }
                }
                for (j = 0; j < numoftasks; j++)
                {
                    //check if there is a task not done and needs completeion
                    if (proc[j].flag != 12 && proc[i].flag == 12)
                    {
                        if (compare_me == 50000)
                        {
                            something_mightbethere = j;
                            compare_me2 = 1;
                        }
                    }
                }

                //if there is nothing there yet and this task is done then move on with time
                if (proc[i].flag == 12 && something_mightbethere == 600)
                {

                    time++;
                    if (time >= proc[i].arrivaltime)
                    {
                        proc[i].starttime = time;
                        proc[i].flag = 00;
                    }
                }

                if (something_mightbethere == compare_me)
                {
                    compare_me = 5000;
                    i = something_mightbethere;
                    break; //go check who is here
                }
                else if (compare_me2 == 1)
                {

                    i = something_mightbethere;
                    proc[i].starttime = time;
                }

                while (proc[i].remainingtime > 0)
                { //while still there is ci to schedule

                    for (j = 0; j < i; j++)
                    { //check if any of the tasks higher priorty have an arrival time
                        if (time >= proc[j].arrivaltime)
                        {
                            Higher_exsitance_coming = j;
                            compare_me = Higher_exsitance_coming;
                        }
                    }

                    if (Higher_exsitance_coming == compare_me)
                    {

                        break; //go back
                    }

                    if (proc[i].flag != 12)
                    { //if not done
                        time++;
                        proc[i].remainingtime--;
                    }
                    else
                    {
                        breaking = 1;
                        break;
                    }
                }

                proc[i].endtime = time;

                //if task is done
                if (proc[i].remainingtime == 0 && proc[i].flag != 12)
                {

                    proc[i].instancesleft--;
                    proc[i].remainingtime = proc[i].runtime;
                    proc[i].arrivaltime = proc[i].arrivaltimes[abs(proc[i].instancesP - proc[i].instancesleft) - 1]; //this the next arrival time
                    proc[i].flag = 12;                                                                               //done
                    printf("Task %d Started from %d till time %d\n", i, proc[i].starttime, proc[i].endtime);
                }
                else if (proc[i].remainingtime > 0 && proc[i].flag != 12)
                {
                    printf("Task %d Started from %d till time %d , actually got premptied\n", i, proc[i].starttime, proc[i].endtime);
                }

                if (Higher_exsitance_coming == compare_me || breaking == 1)
                {
                    i = Higher_exsitance_coming;
                    compare_me = 5000;
                    break; //go check who is here
                }
                else
                {
                    i++;
                }
            }
        }
    }
}

int earliestdedlinetaskcheck(int numoftasks, struct task *proc, int current_task_index)
{
    int i = 0;
    int indexofsmallest = current_task_index;
    for (i = 0; i < numoftasks; i++)
    {
        if ((proc[current_task_index].arrivaltime > proc[i].arrivaltime))
        {
            indexofsmallest = i;
        }
    }
    return indexofsmallest;
}

void edf(int numoftasks, struct task *proc)
{
    //sort task first by period
    int i = 0;
    int j = 0;
    for (i = 0; i < numoftasks; i++)
    {

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

    //how many insances of each task? && intilize each task
    for (i = 0; i < numoftasks; i++)
    {
        double taskperiod = proc[i].period;
        proc[i].instancesP = floor(lcm / taskperiod);
        printf("instance::: %d", proc[i].instancesP);
        proc[i].instancesleft = proc[i].instancesP;
        proc[i].remainingtime = proc[i].runtime;
        proc[i].arrivaltime = proc[i].period;
        proc[i].priority = 1 + i;
    }

    //populate arrivaltimes
    for (i = 0; i < numoftasks; i++)
    {

        for (j = 0; j < proc[i].instancesP; j++)
        {
            proc[i].arrivaltimes[j] = proc[i].period * (j + 1);
        }
    }

    //EDF schduler:
    int time = 0; //track time through scheduling
    i = 0;

    while (lcm != time || alltasksdone(numoftasks, proc) != 1)
    {
        int x = 0;
        for (x = 0; x < numoftasks; x++)
        {
            if (time >= proc[x].arrivaltime)
            {
                proc[x].flag = 00;
            }
        }

        if (proc[i].instancesleft == proc[i].instancesP)
        {

            proc[i].starttime = time;
            while (proc[i].remainingtime > 0)
            {
                time++;
                proc[i].remainingtime--;
            }
            proc[i].endtime = time;
            proc[i].instancesleft--;
            proc[i].remainingtime = proc[i].runtime;
            printf("Task %d Started from %d till time %d\n", i, proc[i].starttime, proc[i].endtime);
            if (i + 1 < numoftasks)
            {
                i++;
            }
            else{
                i=0;
            }
        }
        else
        {
            proc[i].starttime = time;

            
            while (earliestdedlinetaskcheck(numoftasks, proc, i) == i && proc[i].remainingtime != 0)
            {
                time++;
                proc[i].remainingtime--;
            }

            proc[i].endtime = time;

            if (proc[i].remainingtime != 0)
            {

                printf("Task %d Started from %d till time %d , actually got premptied\n", i, proc[i].starttime, proc[i].endtime);
                i = earliestdedlinetaskcheck(numoftasks, proc, i);
            }
            else
            {   
               
                proc[i].arrivaltime = proc[i].starttime + proc[i].period; //dynamic deadline
            

                proc[i].instancesleft--;
                proc[i].remainingtime = proc[i].runtime;
                printf("Task %d Started from %d till time %d\n", i, proc[i].starttime, proc[i].endtime);
                if (i + 1 < numoftasks)
                {
                    i++;
                }
                else{
                    i=0;
                }
            }
        }
    }
}
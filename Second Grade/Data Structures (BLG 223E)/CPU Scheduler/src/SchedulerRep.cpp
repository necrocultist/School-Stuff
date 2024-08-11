#include <iostream>
#include <stdio.h>
#include <string.h>
#include "SchedulerRep.h"

using namespace std;

SchedulerRep::SchedulerRep()
{
    this->timeSliceCount = 0;
    this->totalTime = 0;
    this->mpRunningProcess = NULL;
    this->pCpuObj = new CPURep();
    this->mpProcessFIFO[0] = new FIFORep;
    this->mpProcessFIFO[1] = new FIFORep;
    this->mpProcessFIFO[2] = new FIFORep;
}
SchedulerRep::~SchedulerRep()
{
    delete pCpuObj;
    delete mpProcessFIFO[0];
    delete mpProcessFIFO[1];
    delete mpProcessFIFO[2];
}

FIFORep *SchedulerRep::getProcessFIFO(int index)
{
    return this->mpProcessFIFO[index];
}

void SchedulerRep::setProcessFIFO(FIFORep *fifo, int index)
{
    this->mpProcessFIFO[index] = fifo;
}

void SchedulerRep::setRunningProcess(ProcessRep *p)
{
    this->mpRunningProcess = p;
}

ProcessRep *SchedulerRep::getRunningProcess()
{
    return this->mpRunningProcess;
}

void SchedulerRep::pushProcess(ProcessRep *p)
{
    if (p->getProcessType() == "A")
    {
        if (getProcessFIFO(0) != NULL)
            getProcessFIFO(0)->queue(p);
        else
        {
            setProcessFIFO(new FIFORep(p), 0);
        }
    }
    else if (p->getProcessType() == "B")
    {
        if (getProcessFIFO(1) != NULL)
            getProcessFIFO(1)->queue(p);
        else
        {
            setProcessFIFO(new FIFORep(p), 1);
        }
    }
    else
    {
        if (getProcessFIFO(2) != NULL)
            getProcessFIFO(2)->queue(p);
        else
        {
            setProcessFIFO(new FIFORep(p), 2);
        }
    }

    p->setNext(NULL);
}

ProcessRep *SchedulerRep::popProcess()
{
    ProcessRep *process_to_remove = NULL;

    for (int i = 0; i < 3; i++)
    {
        if (getProcessFIFO(i)->getHead() != NULL)
        {
            process_to_remove = getProcessFIFO(i)->dequeue();
            break;
        }
    }

    return process_to_remove;
}

bool SchedulerRep::checkTimeSlice()
{
    int quantum_limit = (getRunningProcess()->getProcessType() == "A") ? 2 : (getRunningProcess()->getProcessType() == "B" ? 4 : 8);

    if (timeSliceCount != quantum_limit)
    {
        return false;
    }

    timeSliceCount = 0;
    return true;
}

ProcessRep *SchedulerRep::sendProcessToCPU(ProcessRep *p)
{
    if (p != NULL)
    {
        p->setNext(NULL);
        this->pCpuObj->runCPU(p, totalTime);
    }

    return p;
}

void SchedulerRep::schedule(string type, int id, int arrivalTime, int processTime)
{
    ProcessRep *arriving_process = new ProcessRep(type, id, arrivalTime, processTime);

    pushProcess(arriving_process);

    if (getRunningProcess() != NULL)
    {
        int fifo_rep_count = 3;
        for (int i = 0; i < fifo_rep_count; i++)
        {
            if (getProcessFIFO(i)->getHead() != NULL)
            {
                if (getProcessFIFO(i)->getHead()->getProcessType() < getRunningProcess()->getProcessType())
                {
                    timeSliceCount = 0;
                    pushProcess(getRunningProcess());
                    setRunningProcess(popProcess());
                }
            }
        }

        if (checkTimeSlice())
        {
            timeSliceCount = 0;
            pushProcess(getRunningProcess());
            setRunningProcess(popProcess());
        }
    }
    else
    {
        setRunningProcess(popProcess());
    }

    if (getRunningProcess() != NULL)
    {
        setRunningProcess(sendProcessToCPU(getRunningProcess()));

        if (getRunningProcess() == pCpuObj->getFinishedProcess()->getTail())
        {
            ProcessRep *fin;
            int fifo_rep_count = 3;

            timeSliceCount = 0;

            for (int i = 0; i < fifo_rep_count; i++)
            {
                fin = getProcessFIFO(i)->getHead();
                if (fin == NULL && i == fifo_rep_count - 1)
                {
                    totalTime++;
                    setRunningProcess(NULL);
                    return;
                }

                else
                {
                    totalTime++;
                    setRunningProcess(popProcess());
                    return;
                }
            }
            
        }
        timeSliceCount++;
    }
    totalTime++;
}

void SchedulerRep::schedule(ProcessRep *p)
{

    if (getRunningProcess() != NULL)
    {
        for (int i = 0; i < 3; i++)
        {
            if (getProcessFIFO(i)->getHead() != NULL)
            {
                if (getProcessFIFO(i)->getHead()->getProcessType() < getRunningProcess()->getProcessType())
                {
                    timeSliceCount = 0;
                    pushProcess(getRunningProcess());
                    setRunningProcess(popProcess());
                }
            }
        }

        if (checkTimeSlice())
        {
            timeSliceCount = 0;
            pushProcess(mpRunningProcess);
            setRunningProcess(popProcess());
        }
    }
    else
    {
        setRunningProcess(popProcess());
    }

    if (getRunningProcess() != NULL)
    {
        setRunningProcess(sendProcessToCPU(getRunningProcess()));

        if (getRunningProcess() == pCpuObj->getFinishedProcess()->getTail())
        {
            ProcessRep *fin;
            int fifo_rep_count = 3;

            timeSliceCount = 0;

            for (int i = 0; i < fifo_rep_count; i++)
            {
                fin = getProcessFIFO(i)->getHead();
                if (fin == NULL && i == fifo_rep_count - 1)
                {
                    totalTime++;
                    setRunningProcess(NULL);
                    return;
                }

                else
                {
                    totalTime++;
                    setRunningProcess(popProcess());
                    return;
                }
            }
        }
        timeSliceCount++;
    }
    totalTime++;
}
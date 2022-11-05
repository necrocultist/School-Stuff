#include <iostream>
#include <stdio.h>
#include <string.h>
// #include "node.h"
#include "ProcessRep.h"
#include "FIFORep.h"
#include "CPURep.h"

using namespace std;

CPURep::CPURep()
{
   this->mFinishedProcess = new FIFORep;
}

CPURep::~CPURep()
{
    delete mFinishedProcess;
}

ProcessRep* CPURep::runCPU(ProcessRep* p, int time)
{   
    if(p->remainingTime == p->getProcessTime())
        p->startTime = time;

    p->remainingTime--;

    if(p->remainingTime == 0) 
    {
        if(mFinishedProcess == NULL)
        {
            setFinishedProcess(new FIFORep());
        }
        
        p->endTime = time + 1;
        mFinishedProcess->queue(p);
    }

    return p;
}

FIFORep* CPURep::getFinishedProcess()
{
    return this->mFinishedProcess;
}

void CPURep::setFinishedProcess(FIFORep* finishedProcess)
{
    this->mFinishedProcess = finishedProcess;
}
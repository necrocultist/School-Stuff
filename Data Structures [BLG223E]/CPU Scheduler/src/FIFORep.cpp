#include <iostream>
#include <stdio.h>
#include <string.h>

#include "ProcessRep.h"
#include "FIFORep.h"

using namespace std;

FIFORep::FIFORep()
{
    mpHead = NULL;
    mpTail = NULL;
}

FIFORep::FIFORep(ProcessRep *head)
{
    this->mpHead = head;
    this->mpTail = head;
}

FIFORep::~FIFORep()
{
    ProcessRep *headof = mpHead;

    while (headof != NULL)
    {
        headof = mpHead->getNext();
        delete mpHead;
        mpHead = headof;
    }

    delete headof;
    mpHead = NULL;
    mpTail = NULL;
}

void FIFORep::setHead(ProcessRep *head)
{
    this->mpHead = head;
}

ProcessRep *FIFORep::getHead()
{
    return this->mpHead;
}

void FIFORep::setTail(ProcessRep *tail)
{
    this->mpTail = tail;
}

ProcessRep *FIFORep::getTail()
{
    return this->mpTail;
}

void FIFORep::queue(ProcessRep *p)
{

    if (mpHead == NULL)
    {
        mpHead = p;
        mpTail = p;
    }

    else
    {
        mpTail->setNext(p);
        setTail(p);
    }
}

ProcessRep *FIFORep::dequeue()
{
    ProcessRep *removed_process = mpHead;

    if (mpHead == NULL)
    {
        return NULL;
    }
    else
    {
        if (mpHead->getNext() != NULL)
        {
            mpHead = mpHead->getNext();
        }
        else
        {
            mpHead = NULL;
            mpTail = NULL;
        }
    }

    return removed_process;
}

ProcessRep *FIFORep::searchID(int id)
{
    ProcessRep *headof = mpHead;

    while (headof != NULL)
    {
        if (headof->getProcessID() == id)
            return headof;
        headof = headof->getNext();
    }

    delete headof;
    return NULL;
}

void FIFORep::printFIFO()
{
    if (mpHead != NULL)
    {
        ProcessRep *headof = mpHead;

        while (headof != NULL)
        {
            cout << headof->getProcessType() << headof->getProcessID() << " " << headof->startTime << " " << headof->endTime << " ";

            headof = headof->getNext();
        }
            delete headof;
    }

}
#include "CPUCore.h"

CPUCore::CPUCore(int id)
{
    this->id = id;
    busy = false;
    current_process = -1;
}

int CPUCore::getId() const
{
    return id;
}

bool CPUCore::isBusy() const
{
    return busy;
}

int CPUCore::getCurrentProcess() const
{
    return current_process;
}

void CPUCore::assignProcess(int pid)
{
    current_process = pid;
    busy = true;
}

void CPUCore::release()
{
    current_process = -1;
    busy = false;
}
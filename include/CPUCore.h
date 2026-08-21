#ifndef CPU_CORE_H
#define CPU_CORE_H

class CPUCore
{
private:
    int id;
    bool busy;
    int current_process;

public:
    CPUCore(int id);

    int getId() const;

    bool isBusy() const;

    int getCurrentProcess() const;

    void assignProcess(int pid);

    void release();
};

#endif
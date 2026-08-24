#ifndef CPU_CORE_H
#define CPU_CORE_H

#include <deque>

class CPUCore
{
private:
    int id;
    bool busy;
    int current_process;

    double frequency;
    bool performance_core;
    double utilization;
    
    std::deque<bool> utilization_history;

    static constexpr int UTILIZATION_WINDOW = 10;

public:
    CPUCore(int id, bool performance_core = true);

    int getId() const;

    bool isBusy() const;

    int getCurrentProcess() const;

    double getFrequency() const;

    bool isPerformanceCore() const;

    void assignProcess(int pid);

    void release();

    void setFrequency(double frequency);

    double getUtilization() const;

    void updateUtilization(bool wasBusy);


};

#endif
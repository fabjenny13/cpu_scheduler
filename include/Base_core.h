#ifndef CORE_H
#define CORE_H

#include <vector>

struct Task {
    int id;

    double utilization;   
    double work;          
    double arrivalTime;   

    
    int assignedCore;
    double startTime;
    double finishTime;
    double executionTime;
    double energy;

    Task(int id,
         double utilization,
         double work,
         double arrivalTime);
};


class Core {

private:

    int id;
    double utilization;
    double frequency;
    double busyUntil;

public:

    Core(int id);

    
    int getId() const;
    double getUtilization() const;
    double getFrequency() const;
    double getBusyUntil() const;

    
    void addUtilization(double value);
    void setFrequency(double value);
    void setBusyUntil(double value);
};




// Frequency based on CPU utilization
double chooseFrequency(double utilization);

// Power consumed at a particular frequency
double getPower(double frequency);

// Choose least-utilized core
int chooseCore(
    const std::vector<Core>& cores,
    const Task& task
);

#endif

#include <iostream>
#include <vector>

#include "Process.h"
#include "FCFS.h"
#include "SJF.h"
#include "RoundRobin.h"
#include "EnergyAwareScheduler.h"
#include "Simulator.h"
#include "Metrics.h"

int main()
{
    std::vector<Process> processes =
    {
        {1, 0, 5, 5, -1, -1, 0, 0, 0, WorkloadType::CPU_BOUND},
        {2, 1, 3, 3, -1, -1, 0, 0, 0, WorkloadType::IO_BOUND},
        {3, 2, 8, 8, -1, -1, 0, 0, 0, WorkloadType::BACKGROUND},
        {4, 3, 6, 6, -1, -1, 0, 0, 0, WorkloadType::INTERACTIVE}
    };

    // auto rr_processes = processes;

    FCFS fcfs;

    Simulator fcfs_simulator(
        processes,
        2,
        &fcfs
    );

    double fcfs_energy = fcfs_simulator.run();

    printResults(fcfs_simulator.getProcesses(), "FCFS");


    SJF sjf;

    Simulator sjf_simulator(
        processes,
        2,
        &sjf
    );

    double sjf_energy = sjf_simulator.run();

    printResults(sjf_simulator.getProcesses(), "SJF");

   
    RoundRobin rr(2);

    Simulator rr_simulator(
        processes,
        2,
        &rr
    );

    double rr_energy = rr_simulator.run();

    printResults(rr_simulator.getProcesses(), "Round Robin");

    
    EnergyAwareScheduler energy;

    Simulator energy_simulator(
        processes,
        2,
        &energy
    );

    double energy_energy = energy_simulator.run();

    printResults(
        energy_simulator.getProcesses(),
        "Energy Aware Scheduler"
    );


    std::cout << "\n===== FINAL COMPARISON =====\n";

std::cout << "FCFS Energy: "
          << fcfs_energy << " J\n";

std::cout << "SJF Energy: "
          << sjf_energy  << " J\n";

std::cout << "RR Energy: "
          << rr_energy << " J\n";

std::cout << "Energy-Aware Energy: "
          << energy_energy << " J\n";

    return 0;
}
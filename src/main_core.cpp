#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>

#include "Base_core.h"

using namespace std;

int main()
{
   

    const int NUM_CORES = 4;

    vector<Core> cores;

    for (int i = 0; i < NUM_CORES; i++)
    {
        cores.emplace_back(i);
    }

     vector<Task> tasks =
    {
        Task(1, 80, 10000, 0),
        Task(2, 30, 4000, 0),
        Task(3, 60, 7000, 0),
        Task(4, 20, 3000, 0),

        Task(5, 50, 6000, 2),
        Task(6, 40, 5000, 3)
    };


    double totalEnergy = 0;


    
    // Schedule Tasks
   

    for (Task& task : tasks)
    {
        
        // Choose a core
        

        int coreID =
            chooseCore(cores, task);


        if (coreID == -1)
        {
            cout << "Task "
                 << task.id
                 << " could not be scheduled.\n";

            continue;
        }


        Core& core = cores[coreID];

        task.assignedCore = coreID;


        
        // Update core utilization
        

        core.addUtilization(
            task.utilization
        );


        
        // Choose frequency
       

        double frequency =
            chooseFrequency(
                core.getUtilization()
            );

        core.setFrequency(frequency);


        
        // Calculate execution time
       

        task.executionTime =
            task.work /
            (frequency * 1000.0);


        
        // Start time
        

        task.startTime =
            max(
                task.arrivalTime,
                core.getBusyUntil()
            );


        
        // Finish time
        

        task.finishTime =
            task.startTime +
            task.executionTime;


       
        // Calculate power
        

        double power =
            getPower(frequency);


        
        // Calculate energy
        

        task.energy =
            power *
            task.executionTime;

        totalEnergy += task.energy;


        
        // Update core
        

        core.setBusyUntil(
            task.finishTime
        );


        
        // Display Task Information
       

        cout << fixed
             << setprecision(2);

        cout << "Task " << task.id << "\n";

        cout << "Assigned Core : "
             << core.getId() << "\n";

        cout << "Task Utilization : "
             << task.utilization
             << "%\n";

        cout << "Core Utilization : "
             << core.getUtilization()
             << "%\n";

        cout << "Frequency : "
             << frequency
             << " GHz\n";

        cout << "Power : "
             << power
             << " W\n";

        cout << "Start Time : "
             << task.startTime
             << " s\n";

        cout << "Execution Time : "
             << task.executionTime
             << " s\n";

        cout << "Finish Time : "
             << task.finishTime
             << " s\n";

        cout << "Energy : "
             << task.energy
             << " J\n";
    }


    
    // Final Results
    cout << fixed
         << setprecision(2);

    cout << "Total Energy = "
         << totalEnergy
         << " Joules\n";


    cout << "\nFinal Core States:\n";

    for (const Core& core : cores)
    {
        cout << "Core "
             << core.getId()

             << " | Utilization = "
             << core.getUtilization()

             << "% | Frequency = "
             << core.getFrequency()

             << " GHz\n";
    }


    return 0;
}

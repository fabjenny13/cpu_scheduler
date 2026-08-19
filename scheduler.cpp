#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#include <algorithm>
#include <climits>
using namespace std;

struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;   // used for Round Robin
    int completion_time;
    int waiting_time;
    int turnaround_time;
};

void printResults(vector<Process>& processes, const string& title) {
    double total_wt = 0, total_tat = 0;

    cout << "\n===== " << title << " =====\n";
    cout << left << setw(6) << "PID" << setw(10) << "Arrival" << setw(10) << "Burst"
         << setw(12) << "Completion" << setw(10) << "Waiting" << setw(12) << "Turnaround" << "\n";

    for (auto& p : processes) {
        cout << left << setw(6) << p.pid << setw(10) << p.arrival_time << setw(10) << p.burst_time
             << setw(12) << p.completion_time << setw(10) << p.waiting_time
             << setw(12) << p.turnaround_time << "\n";
        total_wt += p.waiting_time;
        total_tat += p.turnaround_time;
    }

    cout << fixed << setprecision(2);
    cout << "Average Waiting Time: " << total_wt / processes.size() << "\n";
    cout << "Average Turnaround Time: " << total_tat / processes.size() << "\n";
}

// ---------------- FCFS ----------------
void fcfs(vector<Process> processes) {
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrival_time < b.arrival_time;
    });

    int current_time = 0;
    for (auto& p : processes) {
        if (current_time < p.arrival_time)
            current_time = p.arrival_time;
        current_time += p.burst_time;
        p.completion_time = current_time;
        p.turnaround_time = p.completion_time - p.arrival_time;
        p.waiting_time = p.turnaround_time - p.burst_time;
    }

    printResults(processes, "First Come First Served (FCFS)");
}

// ---------------- SJF (non-preemptive) ----------------
void sjf(vector<Process> processes) {
    int n = processes.size();
    vector<bool> done(n, false);
    int completed = 0, current_time = 0;

    while (completed < n) {
        int idx = -1;
        int min_burst = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (!done[i] && processes[i].arrival_time <= current_time && processes[i].burst_time < min_burst) {
                min_burst = processes[i].burst_time;
                idx = i;
            }
        }

        if (idx == -1) {
            // no process has arrived yet, jump to next arrival
            current_time++;
            continue;
        }

        current_time += processes[idx].burst_time;
        processes[idx].completion_time = current_time;
        processes[idx].turnaround_time = processes[idx].completion_time - processes[idx].arrival_time;
        processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
        done[idx] = true;
        completed++;
    }

    printResults(processes, "Shortest Job First (SJF - Non Preemptive)");
}

// ---------------- Round Robin ----------------
void roundRobin(vector<Process> processes, int quantum) {
    int n = processes.size();
    for (auto& p : processes) p.remaining_time = p.burst_time;

    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrival_time < b.arrival_time;
    });

    queue<int> q;
    vector<bool> in_queue(n, false);
    int current_time = 0, completed = 0;

    q.push(0);
    in_queue[0] = true;

    while (completed < n) {
        int idx = q.front();
        q.pop();

        if (current_time < processes[idx].arrival_time)
            current_time = processes[idx].arrival_time;

        int exec_time = min(quantum, processes[idx].remaining_time);
        current_time += exec_time;
        processes[idx].remaining_time -= exec_time;

        // enqueue any processes that arrived during this slice
        for (int i = 0; i < n; i++) {
            if (!in_queue[i] && i != idx && processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) {
                q.push(i);
                in_queue[i] = true;
            }
        }

        if (processes[idx].remaining_time > 0) {
            q.push(idx);
        } else {
            processes[idx].completion_time = current_time;
            processes[idx].turnaround_time = processes[idx].completion_time - processes[idx].arrival_time;
            processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
            completed++;
        }

        if (q.empty() && completed < n) {
            for (int i = 0; i < n; i++) {
                if (!in_queue[i] && processes[i].remaining_time > 0) {
                    q.push(i);
                    in_queue[i] = true;
                    break;
                }
            }
        }
    }

    printResults(processes, "Round Robin (Quantum = " + to_string(quantum) + ")");
}

int main() {
    vector<Process> processes = {
        {1, 0, 5, 5, 0, 0, 0},
        {2, 1, 3, 3, 0, 0, 0},
        {3, 2, 8, 8, 0, 0, 0},
        {4, 3, 6, 6, 0, 0, 0}
    };

    fcfs(processes);
    sjf(processes);
    roundRobin(processes, 2);

    return 0;
}

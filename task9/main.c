#include <stdio.h>

typedef struct {
    int pid, arrival, burst, remaining;
} Process;

void fcfs(Process p[], int n) {
    int total_wait = 0, time = 0;
    for (int i = 0; i < n; i++) {
        if (time < p[i].arrival)
            time = p[i].arrival;
        total_wait += time - p[i].arrival;
        time += p[i].burst;
    }
    printf("FCFS Avg Waiting Time: %.2f\n", (double)total_wait / n);
}

void sjf(Process p[], int n) {
    int total_wait = 0, time = 0, completed = 0, is_completed[n];
    for (int i = 0; i < n; i++)
        is_completed[i] = 0;

    while (completed < n) {
        int idx = -1, min_burst = 1e9;
        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= time && !is_completed[i] && p[i].burst < min_burst) {
                min_burst = p[i].burst;
                idx = i;
            }
        }
        if (idx == -1) time++;
        else {
            total_wait += time - p[idx].arrival;
            time += p[idx].burst;
            is_completed[idx] = 1;
            completed++;
        }
    }
    printf("SJF Avg Waiting Time: %.2f\n", (double)total_wait / n);
}

void round_robin(Process p[], int n, int quantum) {
    int time = 0, completed = 0, total_wait = 0, last_exec[n];
    for (int i = 0; i < n; i++)
        last_exec[i] = p[i].arrival;

    while (completed < n) {
        int done = 1;
        for (int i = 0; i < n; i++) {
            if (p[i].remaining > 0 && p[i].arrival <= time) {
                done = 0;
                int exec_time = (p[i].remaining > quantum) ? quantum : p[i].remaining;
                total_wait += time - last_exec[i];
                time += exec_time;
                p[i].remaining -= exec_time;
                last_exec[i] = time;
                if (p[i].remaining == 0) completed++;
            }
        }
        if (done) time++;
    }
    printf("Round Robin Avg Waiting Time: %.2f\n", (double)total_wait / n);
}

int main() {
    int n, quantum;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    Process p[n];
    for (int i = 0; i < n; i++) {
        printf("Enter arrival time and burst time for process %d: ", i + 1);
        scanf("%d %d", &p[i].arrival, &p[i].burst);
        p[i].pid = i + 1;
        p[i].remaining = p[i].burst;
    }

    printf("Enter time quantum for Round Robin: ");
    scanf("%d", &quantum);

    fcfs(p, n);
    sjf(p, n);
    round_robin(p, n, quantum);
    return 0;
}


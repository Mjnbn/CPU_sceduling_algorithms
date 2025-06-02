#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
// ---------------- Scheduling Algorithms ----------------
static Metrics compute_metrics(Process p[], int n) {
    Metrics m = {0, 0, 0};
    for (int i = 0; i < n; i++) {
        int tat = p[i].completionTime - p[i].arrivalTime;
        int wt  = tat - p[i].burstTime;
        int rt  = p[i].startTime - p[i].arrivalTime;
        m.avgTurnaround += tat;
        m.avgWaiting    += wt;
        m.avgResponse   += rt;
    }
    m.avgTurnaround /= n;
    m.avgWaiting    /= n;
    m.avgResponse   /= n;
    return m;
}

// FCFS Scheduling
Metrics fcfs_metrics(Process proc[], int n) {
    Process *a = malloc(n * sizeof(Process));
    memcpy(a, proc, n * sizeof(Process));

    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++)
            if (a[j].arrivalTime < a[min].arrivalTime)
                min = j;
        if (min != i) {
            Process tmp = a[i];
            a[i] = a[min];
            a[min] = tmp;
        }
    }

    int t = 0;
    for (int i = 0; i < n; i++) {
        if (t < a[i].arrivalTime)
            t = a[i].arrivalTime;
        a[i].startTime = t;
        t += a[i].burstTime;
        a[i].completionTime = t;
    }

    Metrics res = compute_metrics(a, n);
    free(a);
    return res;
}


// SJF Scheduling (Non-preemptive)
Metrics sjf_metrics(Process proc[], int n) {
    Process *a = malloc(n * sizeof(Process));
    memcpy(a, proc, n * sizeof(Process));

    int completed = 0, t = 0;
    int started[100] = {0}, done[100] = {0};

    while (completed < n) {
        int idx = -1, min_b = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (!done[i] &&
                a[i].arrivalTime <= t &&
                a[i].burstTime < min_b) {
                min_b = a[i].burstTime;
                idx = i;
            }
        }
        if (idx < 0) {
            int next_a = INT_MAX;
            for (int i = 0; i < n; i++)
                if (!done[i] && a[i].arrivalTime < next_a)
                    next_a = a[i].arrivalTime;
            t = next_a;
            continue;
        }
        if (!started[idx]) {
            a[idx].startTime = t;
            started[idx] = 1;
        }
        t += a[idx].burstTime;
        a[idx].completionTime = t;
        done[idx] = 1;
        completed++;
    }

    Metrics res = compute_metrics(a, n);
    free(a);
    return res;
}

// Round Robin Scheduling (Revised)
Metrics rr_metrics(Process proc[], int n, int timeQuantum) {
    Process *a = malloc(n * sizeof(Process));
    memcpy(a, proc, n * sizeof(Process));
    for (int i = 0; i < n; i++)
        a[i].remainingTime = a[i].burstTime;

    Process *arr = malloc(n * sizeof(Process));
    memcpy(arr, a, n * sizeof(Process));
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++)
            if (arr[j].arrivalTime < arr[min].arrivalTime)
                min = j;
        if (min != i) {
            Process tmp = arr[i];
            arr[i] = arr[min];
            arr[min] = tmp;
        }
    }

    Process *rq = malloc(n * sizeof(Process));
    int front = 0, rear = 0, qs = 0;
    int idx_arr = 0, done = 0, t = 0;
    int started[100] = {0};

    while (done < n) {
        if (qs == 0 && idx_arr < n) {
            if (t < arr[idx_arr].arrivalTime)
                t = arr[idx_arr].arrivalTime;
        }
        while (idx_arr < n && arr[idx_arr].arrivalTime <= t) {
            rq[rear] = arr[idx_arr];
            rear = (rear + 1) % n;
            qs++;
            idx_arr++;
        }
        if (qs == 0)
            continue;

        Process cur = rq[front];
        front = (front + 1) % n;
        qs--;

        int pi = -1;
        for (int i = 0; i < n; i++)
            if (a[i].pid == cur.pid) {
                pi = i;
                break;
            }

        if (!started[pi]) {
            a[pi].startTime = t;
            started[pi] = 1;
        }

        int exec = a[pi].remainingTime < timeQuantum
                       ? a[pi].remainingTime
                       : timeQuantum;
        t += exec;
        a[pi].remainingTime -= exec;

        while (idx_arr < n && arr[idx_arr].arrivalTime <= t) {
            rq[rear] = arr[idx_arr];
            rear = (rear + 1) % n;
            qs++;
            idx_arr++;
        }

        if (a[pi].remainingTime > 0) {
            rq[rear] = a[pi];
            rear = (rear + 1) % n;
            qs++;
        } else {
            a[pi].completionTime = t;
            done++;
        }
    }
    Metrics res = compute_metrics(a, n);
    free(arr);
    free(rq);
    free(a);
    return res;
}

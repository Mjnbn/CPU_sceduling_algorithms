# CPU Scheduling Algorithms

This project implements three fundamental CPU scheduling algorithms in C as part of an Operating Systems course project.
(Documented Implementation at the bottom of the page)

## Algorithms Implemented

1. **FCFS (First Come First Serve)**
2. **SJF (Shortest Job First - Non-Preemptive)**
3. **Round Robin (RR)**

Each algorithm computes the following performance metrics:
- Average Turnaround Time
- Average Waiting Time
- Average Response Time

## File Structure

```
.
├── scheduler.h         # Declarations for structs and scheduling functions
├── scheduler.c         # Implementation of the scheduling algorithms
├── scheduler_test.c    # Test cases and assertions for validating correctness
├── Makefile            # Build automation script
└── README.md           # Project documentation (this file)
```

## How to Compile

To build the project, simply use:

```bash
make
```

This compiles the source files and produces an executable named `scheduler_test`.

## How to Run

To run the compiled test program:

```bash
make run
```

This will execute the scheduler tests and print the calculated vs. expected metrics for:
- FCFS
- SJF
- Round Robin (with specified quantum)

If all assertions pass, you will see:

```
>>> Test Case X PASSED.
...
ALL TESTS PASSED.
```

## How to Clean

To remove the compiled binary:

```bash
make clean
```

## Example Output (Truncated)

```
==== Test Case 1 ====
FCFS: Calculated: Turnaround: 15.00, Waiting: 7.33, Response: 7.33
      Expected:   Turnaround: 15.00, Waiting: 7.33, Response: 7.33
SJF:  ...
RR (Quantum = 4): ...
>>> Test Case 1 PASSED.
```

## Notes

- The `Process` struct contains fields for tracking all timing metrics.
- All algorithms are non-preemptive except RR which uses time slicing.
- The project is written in standard C and should work on any Unix-like system (Linux, macOS, WSL).

## Implementation

We implement such scheduling algorithms accordingly:

### FCFS (First Come First Serve)

We copy the `proc[]` elements into the array `a[]`, avoiding altering the original data, and then preforming a selection sort on the basis of arrival time.
Using a loop, we simulate time, and as the loops proceed, the `t` value increases, and based on that value we choose the next process to operate on and go forward until the next process.

### SJF (Shortest Job First)

The `proc[]` array is duplicated into `a[]` once again, and the processes are sorted, this time based on time burst duration. Two helper arrays named `started[]` and `done[]` are created to list which processes have begun and which have ended.
During the loop, whenever a process is found, we mark it in the `started[]` array. As long as the process is only available in the `started[]` array and not the `done[]` array, we will not start any other process.
Once a process is over, we move to the next process that has not yet been started.

### RR (Round Robin)

We again duplicate `proc[]` into `a[]`, but also a temporary `arr[]` array and sort it by arrival time. We also initialise each process with a `remainingTime` and `burstTime`.
During the loop, as well as the proceeding of time, we also keep an eye on the amount of time passed, so that we can halt a process and work on another when the we reach a multiple of the time quantuum. The remaining time is recorded to know how much more work is needed on the process, and when it reaches zero, we remove it from the `arr[]`. The `arr[]` is to make a correct circulation of the processes based on the order they first arrived.

1. Run the program with the following flags: ./process-run.py
-l 5:100,5:100. What should the CPU utilization be (e.g., the
percent of time the CPU is in use?) Why do you know this? Use the
-c and -p flags to see if you were right.

CPU utilization will be 100% for process 0 for 5 time units and then 100% for process 1 for 5 time units (totaling 10). This is
because we know the number of instructions each process has and know that each is guaranteed to only use the
CPU (5 instructions : 100% chance to use CPU)

2. Now run with these flags: ./process-run.py -l 4:100,1:0.
These flags specify one process with 4 instructions (all to use the
CPU), and one that simply issues an I/O and waits for it to be done.
How long does it take to complete both processes? Use -c and -p
to find out if you were right.

Assuming an I/O operation takes 5 time units, it takes a total of 10 time units for both processes to complete. This is because process 0 uses the CPU for its 4 instructions and process 1 takes 1 time unit for CPU to issue I/O and 5 time units for the operation to complete.

3. Now switch the order of the processes: ./process-run.py -l
1:0,4:100. What happens now? Does switching the order matter? Why? (As always, use -c and -p to see if you were right)

Switching the order would lead to the following (assuming I/O operation still takes 5 time units):

Time    PID: 0    PID: 1    CPU    I/O
1       issue I/O ready     1       0
2       waiting   running   1       1
3       waiting   running   1       1
4       waiting   running   1       1
5       waiting   running   1       1
6       done      done      1       0

First, the CPU issues I/O operation, then while the I/O is causing process 0 to wait, the CPU runs process 1's 4 instructions. Finally, both complete and 6 time units have elapsed.

4. We’ll now explore some of the other flags. One important flag is -S,
which determines how the system reacts when a process issues an
I/O. With the flag set to SWITCH ON END, the system will NOT
switch to another process while one is doing I/O, instead waiting
until the process is completely finished. What happens when you
run the following two processes, one doing I/O and the other doing
CPU work? (-l 1:0,4:100 -c -S SWITCH_ON_END)

In this case, both processes would take 9 steps. Here's why:

Time    PID: 0    PID: 1    CPU    I/O
1       issue I/O ready     1      0
2       waiting   ready     0      1
3       waiting   ready     0      1
4       waiting   ready     0      1
5       waiting   ready     0      1
6       done      running   1      0
7       done      running   1      0
8       done      running   1      0
9       done      running   1      0

First, the CPU issues I/O operation for process 0, but because we are not allowed to switch to process 1 until process 0's I/O completes, we wait for the remaining time units. Finally, process 0 finishes and process 1 runs after for 4 time units.

5. Now, run the same processes, but with the switching behavior set
to switch to another process whenever one is WAITING for I/O (-l
1:0,4:100 -c -S SWITCH ON IO). What happens now? Use -c

Same behavior as number 3 because the default behavior is to switch on I/O.

6.  One other important behavior is what to do when an I/O completes. With -I IO_RUN_LATER, when an I/O completes, the process that issued it is not necessarily run right away; rather, whatever was running at the time keeps running. What happens when
you run this combination of processes? (./process-run.py -l
3:0,5:100,5:100,5:100 -S SWITCH ON IO -I IO_RUN_LATER)
Are system resources being effectively utilized?

So first, a CPU issues I/O for process 0. Then while the actual I/O for 0 takes 4 time steps (steps when I/O column is set), process 1 runs. While first I/O finishes for 0 when 1 completes its 4th instruction, because of the IO_RUN_LATER flag, 1 finishes its 5th instruction, and then 0 resumes and CPU issues its second I/O. Then, process 2 starts and the same thing happens. Second I/O for 0 finishes when 2 completes its 4th instruction, it completes its 5th instruction, and then 0 resumes onto its final I/O. Similarly and unsurprisingly, 3 completes its 4 instructions while the third I/O for process 0 completes. Then, 3 finishes its last instruction and all processes are now complete. So, in all there are the following steps:

Time    PID: 0    PID: 1    PID: 2    PID: 3    CPU    I/O
1       issue I/O ready     ready     ready     1      0
2       waiting   running   ready     ready     1      1
3       waiting   running   ready     ready     1      1
4       waiting   running   ready     ready     1      1
5       waiting   running   ready     ready     1      1
6       ready     running   ready     ready     1      0
7       issue I/O done      ready     ready     1      0
8       waiting   done      running   ready     1      1
9       waiting   done      running   ready     1      1
10      waiting   done      running   ready     1      1
11      waiting   done      running   ready     1      1
12      ready     done      running   ready     1      0
13      issue I/O done      done      ready     1      0
14      waiting   done      done      running   1      1
15      waiting   done      done      running   1      1
16      waiting   done      done      running   1      1
17      waiting   done      done      running   1      1
18      ready     done      done      running   1      0
19      done      done      done      done      1      0

So a total of 19 steps for execution. CORRECTION: I assumed IO_RUN_LATER implied that it will resume process 0 once the currently running process finishes, but it really meant after the other processes finish so that it is its turn again. In that case, the first part up to step 6 is correct, but thereafter processes 2 and 3 would run back to back, then process 0 would resume and complete its final two I/O operations.

The fact that I initially assumed the wrong behavior for IO_RUN_LATER is a great segway into the second question: Are system resources being effectively utilized? If it was the case that IO_RUN_LATER had meant finish the current process before then resuming 0, I would say yes the resources are being effectively utilized. However, in the actual behavior, absolutely not, since there are 8 ticks where the CPU is doing nothing at all but waiting for I/O to complete.

7. Now run the same processes, but with -I IO_RUN_IMMEDIATE set,
which immediately runs the process that issued the I/O. How does
this behavior differ? Why might running a process that just completed an I/O again be a good idea?

In this case, the IO_RUN_IMMEDIATE flag behaves almost exactly how I initially thought the IO_RUN_LATER flag did, except rather than letting the currently running process finish, process 0 is always resumed first. In this case, the CPU issues first I/O for process 0, runs process 1, suspends 1, resumes 0, and issues next I/O, finishes process 1, the runs process 2 for a little bit, and follows the same pattern. The key difference in this case is there is 0 idle CPU time. This is why running a process that just completed I/O is good because it reduces idle CPU time and ensure resources are being used optimally.


8. Now run with some randomly generated processes, e.g., -s 1 -l
3:50,3:50, -s 2 -l 3:50,3:50, -s 3 -l 3:50,3:50. See
if you can predict how the trace will turn out. What happens when
you use -I IO RUN IMMEDIATE vs. -I IO RUN LATER? What happens when you use -S SWITCH ON IO vs. -S SWITCH ON END?

Let's consider seed = 1 for the above example:

python3 ./cpu-intro/process-run.py -l 3:50,3:50 -s 1

Process 0
  cpu
  io
  io

Process 1
  cpu
  cpu
  cpu

When using the flags IO_RUN_IMMEDIATE and SWITCH_ON_IO the following will occur:

process 0 runs first instruction on CPU. Then it issues I/O, which causes process 1 to start and run to completion since process 0 will be waiting for a result back.
Once process 1 finishes, then process 0 is resumed and issues its final I/O operation.

When using the flags IO_RUN_IMMEDIATE and SWITCH_ON_END the following will occur:

process 0 runs first instruction on CPU. Then it issues I/O, but rather than process 1 starting, the CPU is idle until the first I/O completes for process 0. Then, the CPU issues the second I/O, and the CPU waits until that operation finishes before starting process 1 and running to completion.

When using the flags IO_RUN_LATER and SWITCH_ON_IO the following will occur:

process 0 runs first instruction on CPU. Then it issues I/O, which causes process 1 to start and run to completion. If process 1 had more than 3 instructions it would keep running until completion before process 0 would get to complete its second I/O operation.

When using the flags IO_RUN_LATER and SWITCH_ON_END the following will occur:

process 0 runs first instruction on CPU. Then it issues first I/O, CPU waits, second I/O, then finished. Finally, process 1 would get to run.


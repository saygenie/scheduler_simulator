#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PROCESS 9
#define MIN_PROCESS 2
#define MAX_CPU_BURST 15
#define MAX_IO_BURST 5
#define MAX_ARRIVAL 10
#define MAX_PRIORITY 10

#define WALL 130

#define IO_RANDOM 5

#define BACK -1
#define EXIT -1

#define FCFS 1
#define SJF_PREEMPTIVE 2
#define SJF_NON_PREEMPTIVE 3
#define PRIORITY_PREEMPTIVE 4
#define PRIORITY_NON_PREEMPTIVE 5
#define RR 6
#define HRRN 7

#define PREEMPTIVE 1
#define NON_PREEMPTIVE 2

typedef struct Process {
	int pid;
	int cpu_burst_time;
	int io_burst_time;
	int arrival_time;
	int priority;
	int io_start;

	int remained_cpu_burst_time;
	int remained_io_burst_time;
} Process;

typedef struct Node {
	Process *process;
	struct Node *prev;
	struct Node *next;
} Node;

typedef struct Queue {
	Node *head;
	Node *tail;
	int size;
} Queue;

Process *create_process(int i);

void print_process(Process *p);

void wait_processing(Queue *wq, Queue *rq);

void init_remained_time(Process *p);

void init_queue(Queue *q);

int size_queue(Queue *q);

int is_empty(Queue *q);

void enqueue(Queue *q, Process *p);

Process *dequeue(Queue *q);

void print_queue(Queue *q);

void print_step(int i);

void print_wall();

void print_option();

void print_wrong();

void print_Gantt(int result[1000], int T);

int main() {
	srand(time(NULL));
	int step = 1;
	int num_process, algorithm, p_state;
	Process *process[MAX_PROCESS];
	Queue *ready_queue = (Queue*) malloc(sizeof(Queue));
	Queue *waiting_queue = (Queue*) malloc(sizeof(Queue));
	init_queue(ready_queue);
	init_queue(waiting_queue);

	while(1) {
		switch (step) {
			case 1: //step 1 : how many process?
			{
				for(int i=0; i<=MAX_PROCESS; i++) {
					process[i] = (Process *) malloc(sizeof(Process));
				}
				
				print_step(1);
				printf("| input number of processes(%d<= n <= %d)\n", MIN_PROCESS, MAX_PROCESS);
				printf("| exit : %d\n", EXIT);
				printf("| input : ");
				scanf("%d", &num_process);
				print_wall();
				if(num_process == EXIT) {
					for(int i=0; i<=MAX_PROCESS; i++) {
						free(process[i]);
					}
					print_wall();
					return 0;
				}
				else if(num_process < MIN_PROCESS || num_process > MAX_PROCESS) {
					print_wrong();
					break;
				}
				else if(num_process >= MIN_PROCESS && num_process <= MAX_PROCESS) {
					printf("| number of processes : %d\n", num_process);
					for(int i=1; i<=num_process; i++) {
						process[i] = create_process(i);
					}
					step = 2;
					break;
				}
			
				break; //step 1 : break
			}
			case 2: //step 2 : choose algorithm
			{
				printf("| pid :  0 => idle\n");
				for(int i=1; i<=num_process; i++) {
					print_process(process[i]);
				}
				print_step(2);
				printf("| 1. FCFS(First Come First Served)\n");
				printf("| 2. SJF_Preemptive\n");
				printf("| 3. SJF_Non_Preemptive\n");
				printf("| 4. Priority_Preemptive\n");
				printf("| 5. Priority_Non_Preemptive\n");
				printf("| 6. RR(Round Robin)\n");
				printf("| 7. HRRN(Highest Respone Ratio Next\n");
				printf("| reset : %d\n", EXIT);
				printf("| input : ");
				scanf("%d", &algorithm);
				print_wall();

				switch (algorithm) {
					case BACK: 
					{
						for(int i=0; i<=MAX_PROCESS; i++) {
							free(process[i]);
						}
						step = 1;
						break;
					}
					case FCFS:
					{
						int T = 0;
						int total_waiting_time[MAX_PROCESS+1] = {0, };
						int total_turnaround_time[MAX_PROCESS+1] = {0, };
						printf("(FCFS)\n");
						printf("|");
						while(1) {
							for(int i=1; i<=num_process; i++) {
								if(process[i]->arrival_time==T) {
									enqueue(ready_queue, process[i]);
								}
							}

							if(is_empty(ready_queue)) {
								printf("0|");
								wait_processing(waiting_queue, ready_queue);
							}
							else {
								wait_processing(waiting_queue, ready_queue);

								Process *head_process = (Process*) malloc(sizeof(Process));
								head_process = ready_queue->head->process;

								printf("%d", head_process->pid);
								head_process->remained_cpu_burst_time--;

								if(head_process->io_start != 0 && head_process->cpu_burst_time - head_process->io_start == head_process->remained_cpu_burst_time) {
									printf("|");
									enqueue(waiting_queue, dequeue(ready_queue));
								}
									
								if(!is_empty(ready_queue) && ready_queue->head->process->remained_cpu_burst_time == 0) {
									printf("|");
									Process *complete_process = (Process*) malloc(sizeof(Process));
									complete_process = dequeue(ready_queue);
									total_waiting_time[complete_process->pid] = T + 1 - complete_process->arrival_time - complete_process->cpu_burst_time - complete_process->io_burst_time;
									total_turnaround_time[complete_process->pid] = T + 1 - complete_process->arrival_time;
								}
							}

							T++;
							int check = 0;
							for(int i=1; i<=num_process; i++) {
								if(process[i]->remained_cpu_burst_time != 0)
									check = 1;
							}

							if(check==0) {
								double wait = 0;
								double turnaround = 0;

								printf("\n");
								printf("average waiting time : (");
								for(int i=1; i<=num_process; i++) {
									printf("%d ", total_waiting_time[i]);
									wait += (double) total_waiting_time[i];
									if(i!=num_process) {
										printf("+ ");
									}
								}
								printf(") / %d = %.2f\n", num_process, wait/(double)num_process);
								printf("average turnaround time : (");
								for(int i=1; i<=num_process; i++) {
									printf("%d ", total_turnaround_time[i]);
									turnaround += (double)total_turnaround_time[i];
									if(i!=num_process) {
										printf("+ ");
									}
								}
								printf(") / %d = %.2f\n", num_process, turnaround/(double)num_process);
								for(int i=1; i<=num_process; i++) {
									init_remained_time(process[i]);
								}
								init_queue(waiting_queue);
								init_queue(ready_queue);
								break;
							}
						}
						print_wall();
						break;
					}
					case SJF_PREEMPTIVE:
					{
						int T = 0;
						int total_waiting_time[MAX_PROCESS+1] = {0, };
						int total_turnaround_time[MAX_PROCESS+1] = {0, };
						printf("(SJF_Preemptive)\n");
						printf("|");
						while(1) {
							for(int i=1; i<=num_process; i++) {
								if(process[i]->arrival_time==T) {
									enqueue(ready_queue, process[i]);
								}
							}

							if(is_empty(ready_queue)) {
								printf("0|");
								wait_processing(waiting_queue, ready_queue);
							}
							else {
								Process *sj_process = (Process*) malloc(sizeof(Process));
								sj_process = ready_queue->head->process;
								Node *node = (Node*) malloc(sizeof(Node));
								node = ready_queue->head;

								int need_dequeue = 0;
								for(int i=0; i<size_queue(ready_queue); i++) {
									if(sj_process->remained_cpu_burst_time > node->process->remained_cpu_burst_time) {
										sj_process = node->process;
										need_dequeue = i;
									}
									
									node = node->next;
								}

								for(int i=0; i<need_dequeue; i++) {
									enqueue(ready_queue, dequeue(ready_queue));
								}

								if(need_dequeue != 0)
									printf("#"); //preemptive

								printf("%d", ready_queue->head->process->pid);
								sj_process->remained_cpu_burst_time--;

								if(!is_empty(waiting_queue)) {
									wait_processing(waiting_queue, ready_queue);
								}

								if(sj_process->io_start != 0 && sj_process->cpu_burst_time - sj_process->io_start == sj_process->remained_cpu_burst_time) {
									printf("|");
									enqueue(waiting_queue, dequeue(ready_queue));
								}
								
								if(!is_empty(ready_queue) && ready_queue->head->process->remained_cpu_burst_time == 0) {
									printf("|");
									Process *complete_process = (Process*) malloc(sizeof(Process));
									complete_process = dequeue(ready_queue);
									total_waiting_time[complete_process->pid] = T + 1 - complete_process->arrival_time - complete_process->cpu_burst_time - complete_process->io_burst_time;
									total_turnaround_time[complete_process->pid] = T + 1 - complete_process->arrival_time;
								}
							}

							T++;
							int check = 0;
							for(int i=1; i<=num_process; i++) {
								if(process[i]->remained_cpu_burst_time != 0)
									check = 1;
							}

							if(check==0) {
								double wait = 0;
								double turnaround = 0;

								printf("\n");
								printf("average waiting time : (");
								for(int i=1; i<=num_process; i++) {
									printf("%d ", total_waiting_time[i]);
									wait += (double) total_waiting_time[i];
									if(i!=num_process) {
										printf("+ ");
									}
								}
								printf(") / %d = %.2f\n", num_process, wait/(double)num_process);
								printf("average turnaround time : (");
								for(int i=1; i<=num_process; i++) {
									printf("%d ", total_turnaround_time[i]);
									turnaround += (double)total_turnaround_time[i];
									if(i!=num_process) {
										printf("+ ");
									}
								}
								printf(") / %d = %.2f\n", num_process, turnaround/(double)num_process);
								for(int i=1; i<=num_process; i++) {
									init_remained_time(process[i]);
								}
								init_queue(waiting_queue);
								init_queue(ready_queue);
								break;
							}
						}
						print_wall();
						break;
					}
					case SJF_NON_PREEMPTIVE:
					{
						int T = 0;
						int total_waiting_time[MAX_PROCESS+1] = {0, };
						int total_turnaround_time[MAX_PROCESS+1] = {0, };
						printf("(SJF_Non_Preemptive)\n");
						printf("|");
						while(1) {
							for(int i=1; i<=num_process; i++) {
								if(process[i]->arrival_time==T) {
									enqueue(ready_queue, process[i]);
								}
							}

							if(is_empty(ready_queue)) {
								printf("0|");
								wait_processing(waiting_queue, ready_queue);
							}
							else {
								Process *sj_process = (Process*) malloc(sizeof(Process));
								sj_process = ready_queue->head->process;
								Node *node = (Node*) malloc(sizeof(Node));
								node = ready_queue->head;

								printf("%d", ready_queue->head->process->pid);
								sj_process->remained_cpu_burst_time--;

								if(!is_empty(waiting_queue)) {
									wait_processing(waiting_queue, ready_queue);
								}

								if(sj_process->io_start != 0 && sj_process->cpu_burst_time - sj_process->io_start == sj_process->remained_cpu_burst_time) {
									printf("|");
									enqueue(waiting_queue, dequeue(ready_queue));
									int need_dequeue = 0;
									for(int i=0; i<size_queue(ready_queue); i++) {
										if(sj_process->remained_cpu_burst_time > node->process->remained_cpu_burst_time) {
											sj_process = node->process;
											need_dequeue = i;
										}
										
										node = node->next;
									}

									for(int i=0; i<need_dequeue; i++) {
										enqueue(ready_queue, dequeue(ready_queue));
									}
								}
								
								if(!is_empty(ready_queue) && ready_queue->head->process->remained_cpu_burst_time == 0) {
									printf("|");
									Process *complete_process = (Process*) malloc(sizeof(Process));
									complete_process = dequeue(ready_queue);
									total_waiting_time[complete_process->pid] = T + 1 - complete_process->arrival_time - complete_process->cpu_burst_time - complete_process->io_burst_time;
									total_turnaround_time[complete_process->pid] = T + 1 - complete_process->arrival_time;

									int need_dequeue = 0;
									for(int i=0; i<size_queue(ready_queue); i++) {
										if(sj_process->remained_cpu_burst_time > node->process->remained_cpu_burst_time) {
											sj_process = node->process;
											need_dequeue = i;
										}
										
										node = node->next;
									}

									for(int i=0; i<need_dequeue; i++) {
										enqueue(ready_queue, dequeue(ready_queue));
									}
								}
							}

							T++;
							int check = 0;
							for(int i=1; i<=num_process; i++) {
								if(process[i]->remained_cpu_burst_time != 0)
									check = 1;
							}

							if(check==0) {
								double wait = 0;
								double turnaround = 0;

								printf("\n");
								printf("average waiting time : (");
								for(int i=1; i<=num_process; i++) {
									printf("%d ", total_waiting_time[i]);
									wait += (double) total_waiting_time[i];
									if(i!=num_process) {
										printf("+ ");
									}
								}
								printf(") / %d = %.2f\n", num_process, wait/(double)num_process);
								printf("average turnaround time : (");
								for(int i=1; i<=num_process; i++) {
									printf("%d ", total_turnaround_time[i]);
									turnaround += (double)total_turnaround_time[i];
									if(i!=num_process) {
										printf("+ ");
									}
								}
								printf(") / %d = %.2f\n", num_process, turnaround/(double)num_process);
								for(int i=1; i<=num_process; i++) {
									init_remained_time(process[i]);
								}
								init_queue(waiting_queue);
								init_queue(ready_queue);
								break;
							}
						}
						print_wall();
						break;
					}
					case PRIORITY_PREEMPTIVE:
					{
						int T = 0;
						int total_waiting_time[MAX_PROCESS+1] = {0, };
						int total_turnaround_time[MAX_PROCESS+1] = {0, };
						printf("(PRIORITY_Preemptive)\n");
						printf("|");
						while(1) {
							for(int i=1; i<=num_process; i++) {
								if(process[i]->arrival_time==T) {
									enqueue(ready_queue, process[i]);
								}
							}

							if(is_empty(ready_queue)) {
								printf("0|");
								wait_processing(waiting_queue, ready_queue);
							}
							else {
								Process *priority_process = (Process*) malloc(sizeof(Process));
								priority_process = ready_queue->head->process;
								Node *node = (Node*) malloc(sizeof(Node));
								node = ready_queue->head;

								int need_dequeue = 0;
								for(int i=0; i<size_queue(ready_queue); i++) {
									if(priority_process->priority < node->process->priority) {
										priority_process = node->process;
										need_dequeue = i;
									}
									
									node = node->next;
								}

								for(int i=0; i<need_dequeue; i++) {
									enqueue(ready_queue, dequeue(ready_queue));
								}

								if(need_dequeue != 0)
									printf("#"); //preemptive

								printf("%d", ready_queue->head->process->pid);
								priority_process->remained_cpu_burst_time--;

								if(!is_empty(waiting_queue)) {
									wait_processing(waiting_queue, ready_queue);
								}

								if(priority_process->io_start != 0 && priority_process->cpu_burst_time - priority_process->io_start == priority_process->remained_cpu_burst_time) {
									printf("|");
									enqueue(waiting_queue, dequeue(ready_queue));
								}
								
								if(!is_empty(ready_queue) && ready_queue->head->process->remained_cpu_burst_time == 0) {
									printf("|");
									Process *complete_process = (Process*) malloc(sizeof(Process));
									complete_process = dequeue(ready_queue);
									total_waiting_time[complete_process->pid] = T + 1 - complete_process->arrival_time - complete_process->cpu_burst_time - complete_process->io_burst_time;
									total_turnaround_time[complete_process->pid] = T + 1 - complete_process->arrival_time;
								}
							}

							T++;
							int check = 0;
							for(int i=1; i<=num_process; i++) {
								if(process[i]->remained_cpu_burst_time != 0)
									check = 1;
							}

							if(check==0) {
								double wait = 0;
								double turnaround = 0;

								printf("\n");
								printf("average waiting time : (");
								for(int i=1; i<=num_process; i++) {
									printf("%d ", total_waiting_time[i]);
									wait += (double) total_waiting_time[i];
									if(i!=num_process) {
										printf("+ ");
									}
								}
								printf(") / %d = %.2f\n", num_process, wait/(double)num_process);
								printf("average turnaround time : (");
								for(int i=1; i<=num_process; i++) {
									printf("%d ", total_turnaround_time[i]);
									turnaround += (double)total_turnaround_time[i];
									if(i!=num_process) {
										printf("+ ");
									}
								}
								printf(") / %d = %.2f\n", num_process, turnaround/(double)num_process);
								for(int i=1; i<=num_process; i++) {
									init_remained_time(process[i]);
								}
								init_queue(waiting_queue);
								init_queue(ready_queue);
								break;
							}
						}
						print_wall();
						break;
					}
					case PRIORITY_NON_PREEMPTIVE:
					{
						int T = 0;
						int total_waiting_time[MAX_PROCESS+1] = {0, };
						int total_turnaround_time[MAX_PROCESS+1] = {0, };
						printf("(PRIORITY_Non_Preemptive)\n");
						printf("|");
						while(1) {
							for(int i=1; i<=num_process; i++) {
								if(process[i]->arrival_time==T) {
									enqueue(ready_queue, process[i]);
								}
							}

							if(is_empty(ready_queue)) {
								printf("0|");
								wait_processing(waiting_queue, ready_queue);
							}
							else {
								Process *priority_process = (Process*) malloc(sizeof(Process));
								priority_process = ready_queue->head->process;
								Node *node = (Node*) malloc(sizeof(Node));
								node = ready_queue->head;

								printf("%d", ready_queue->head->process->pid);
								priority_process->remained_cpu_burst_time--;

								if(!is_empty(waiting_queue)) {
									wait_processing(waiting_queue, ready_queue);
								}

								if(priority_process->io_start != 0 && priority_process->cpu_burst_time - priority_process->io_start == priority_process->remained_cpu_burst_time) {
									printf("|");
									enqueue(waiting_queue, dequeue(ready_queue));
									int need_dequeue = 0;
									for(int i=0; i<size_queue(ready_queue); i++) {
										if(priority_process->priority < node->process->priority) {
											priority_process = node->process;
											need_dequeue = i;
										}
										
										node = node->next;
									}

									for(int i=0; i<need_dequeue; i++) {
										enqueue(ready_queue, dequeue(ready_queue));
									}
								}
								
								if(!is_empty(ready_queue) && ready_queue->head->process->remained_cpu_burst_time == 0) {
									printf("|");
									Process *complete_process = (Process*) malloc(sizeof(Process));
									complete_process = dequeue(ready_queue);
									total_waiting_time[complete_process->pid] = T + 1 - complete_process->arrival_time - complete_process->cpu_burst_time - complete_process->io_burst_time;
									total_turnaround_time[complete_process->pid] = T + 1 - complete_process->arrival_time;

									int need_dequeue = 0;
									for(int i=0; i<size_queue(ready_queue); i++) {
										if(priority_process->priority < node->process->priority) {
											priority_process = node->process;
											need_dequeue = i;
										}
										
										node = node->next;
									}

									for(int i=0; i<need_dequeue; i++) {
										enqueue(ready_queue, dequeue(ready_queue));
									}
								}
							}

							T++;
							int check = 0;
							for(int i=1; i<=num_process; i++) {
								if(process[i]->remained_cpu_burst_time != 0)
									check = 1;
							}

							if(check==0) {
								double wait = 0;
								double turnaround = 0;

								printf("\n");
								printf("average waiting time : (");
								for(int i=1; i<=num_process; i++) {
									printf("%d ", total_waiting_time[i]);
									wait += (double) total_waiting_time[i];
									if(i!=num_process) {
										printf("+ ");
									}
								}
								printf(") / %d = %.2f\n", num_process, wait/(double)num_process);
								printf("average turnaround time : (");
								for(int i=1; i<=num_process; i++) {
									printf("%d ", total_turnaround_time[i]);
									turnaround += (double)total_turnaround_time[i];
									if(i!=num_process) {
										printf("+ ");
									}
								}
								printf(") / %d = %.2f\n", num_process, turnaround/(double)num_process);
								for(int i=1; i<=num_process; i++) {
									init_remained_time(process[i]);
								}
								init_queue(waiting_queue);
								init_queue(ready_queue);
								break;
							}
						}
						print_wall();
						break;
					}
					case RR:
					{
						int T = 0;
						int quantum;
						int quantum_check = 0;
						int total_waiting_time[MAX_PROCESS+1] = {0, };
						int total_turnaround_time[MAX_PROCESS+1] = {0, };
						printf("(Round Robin)\n");
						printf("quantum : ");
						scanf("%d", &quantum);
						printf("|");
						while(1) {
							quantum_check++;

							for(int i=1; i<=num_process; i++) {
								if(process[i]->arrival_time==T) {
									enqueue(ready_queue, process[i]);
								}
							}

							if(is_empty(ready_queue)) {
								printf("0|");
								wait_processing(waiting_queue, ready_queue);
								quantum_check = 0;
							}
							else {
								wait_processing(waiting_queue, ready_queue);

								Process *head_process = (Process*) malloc(sizeof(Process));
								head_process = ready_queue->head->process;

								printf("%d", head_process->pid);
								head_process->remained_cpu_burst_time--;

								if(head_process->io_start != 0 && head_process->cpu_burst_time - head_process->io_start == head_process->remained_cpu_burst_time) {
									printf("|");
									enqueue(waiting_queue, dequeue(ready_queue));
									quantum_check = 0;
								}
									
								if(!is_empty(ready_queue) && ready_queue->head->process->remained_cpu_burst_time == 0) {
									printf("|");
									Process *complete_process = (Process*) malloc(sizeof(Process));
									complete_process = dequeue(ready_queue);
									total_waiting_time[complete_process->pid] = T + 1 - complete_process->arrival_time - complete_process->cpu_burst_time - complete_process->io_burst_time;
									total_turnaround_time[complete_process->pid] = T + 1 - complete_process->arrival_time;
									quantum_check = 0;
								}
							}

							if(quantum == quantum_check) {
								printf("#");
								enqueue(ready_queue, dequeue(ready_queue));
								quantum_check = 0;
							}

							T++;
							int check = 0;
							for(int i=1; i<=num_process; i++) {
								if(process[i]->remained_cpu_burst_time != 0)
									check = 1;
							}

							if(check==0) {
								double wait = 0;
								double turnaround = 0;

								printf("\n");
								printf("average waiting time : (");
								for(int i=1; i<=num_process; i++) {
									printf("%d ", total_waiting_time[i]);
									wait += (double) total_waiting_time[i];
									if(i!=num_process) {
										printf("+ ");
									}
								}
								printf(") / %d = %.2f\n", num_process, wait/(double)num_process);
								printf("average turnaround time : (");
								for(int i=1; i<=num_process; i++) {
									printf("%d ", total_turnaround_time[i]);
									turnaround += (double)total_turnaround_time[i];
									if(i!=num_process) {
										printf("+ ");
									}
								}
								printf(") / %d = %.2f\n", num_process, turnaround/(double)num_process);
								for(int i=1; i<=num_process; i++) {
									init_remained_time(process[i]);
								}
								init_queue(waiting_queue);
								init_queue(ready_queue);
								break;
							}
						}
						print_wall();
						break;
					}
					case HRRN:
					{
						int T = 0;
						int total_waiting_time[MAX_PROCESS+1] = {0, };
						int total_turnaround_time[MAX_PROCESS+1] = {0, };
						double priority[MAX_PROCESS+1] = {0, };
						printf("(HRRN)\n");
						printf("|");
						while(1) {
							for(int i=1; i<=num_process; i++) {
								if(process[i]->arrival_time==T) {
									enqueue(ready_queue, process[i]);
								}
							}

							for(int i=1; i<=num_process; i++) {
								double waiting_time = T - process[i]->arrival_time - (process[i]->cpu_burst_time - process[i]->remained_cpu_burst_time) - (process[i]->io_burst_time - process[i]->remained_io_burst_time);
								double estimated_run_time = process[i]->remained_cpu_burst_time;
								priority[i] = (waiting_time + estimated_run_time) / estimated_run_time;
							}

							if(is_empty(ready_queue)) {
								printf("0|");
								wait_processing(waiting_queue, ready_queue);
							}
							else {
								Process *priority_process = (Process*) malloc(sizeof(Process));
								priority_process = ready_queue->head->process;
								Node *node = (Node*) malloc(sizeof(Node));
								node = ready_queue->head;

								printf("%d", ready_queue->head->process->pid);
								priority_process->remained_cpu_burst_time--;

								if(!is_empty(waiting_queue)) {
									wait_processing(waiting_queue, ready_queue);
								}

								if(priority_process->io_start != 0 && priority_process->cpu_burst_time - priority_process->io_start == priority_process->remained_cpu_burst_time) {
									printf("|");
									enqueue(waiting_queue, dequeue(ready_queue));
									int need_dequeue = 0;
									for(int i=0; i<size_queue(ready_queue); i++) {
										if(priority[priority_process->pid] < priority[node->process->pid]) {
											priority_process = node->process;
											need_dequeue = i;
										}
										
										node = node->next;
									}

									for(int i=0; i<need_dequeue; i++) {
										enqueue(ready_queue, dequeue(ready_queue));
									}
								}
								
								if(!is_empty(ready_queue) && ready_queue->head->process->remained_cpu_burst_time == 0) {
									printf("|");
									Process *complete_process = (Process*) malloc(sizeof(Process));
									complete_process = dequeue(ready_queue);
									total_waiting_time[complete_process->pid] = T + 1 - complete_process->arrival_time - complete_process->cpu_burst_time - complete_process->io_burst_time;
									total_turnaround_time[complete_process->pid] = T + 1 - complete_process->arrival_time;

									int need_dequeue = 0;
									for(int i=0; i<size_queue(ready_queue); i++) {
										if(priority[priority_process->pid] < priority[node->process->pid]) {
											priority_process = node->process;
											need_dequeue = i;
										}
										
										node = node->next;
									}

									for(int i=0; i<need_dequeue; i++) {
										enqueue(ready_queue, dequeue(ready_queue));
									}
								}
							}

							T++;
							int check = 0;
							for(int i=1; i<=num_process; i++) {
								if(process[i]->remained_cpu_burst_time != 0)
									check = 1;
							}

							if(check==0) {
								double wait = 0;
								double turnaround = 0;

								printf("\n");
								printf("average waiting time : (");
								for(int i=1; i<=num_process; i++) {
									printf("%d ", total_waiting_time[i]);
									wait += (double) total_waiting_time[i];
									if(i!=num_process) {
										printf("+ ");
									}
								}
								printf(") / %d = %.2f\n", num_process, wait/(double)num_process);
								printf("average turnaround time : (");
								for(int i=1; i<=num_process; i++) {
									printf("%d ", total_turnaround_time[i]);
									turnaround += (double)total_turnaround_time[i];
									if(i!=num_process) {
										printf("+ ");
									}
								}
								printf(") / %d = %.2f\n", num_process, turnaround/(double)num_process);
								for(int i=1; i<=num_process; i++) {
									init_remained_time(process[i]);
								}
								init_queue(waiting_queue);
								init_queue(ready_queue);
								break;
							}
						}
						print_wall();
						break;
					}
					default:
					{
						print_wrong();
						break;
					}
				}

				break; //step 2 : break
			}
		}
	}
	return 0;
} 

void init_queue(Queue *q) {
	q->size = 0;
	q->head = NULL;
	q->tail = NULL;
}

int size_queue(Queue *q) {
	return q->size;
}

int is_empty(Queue *q) {
	if(q->size==0)
		return 1;
	else
		return 0;
}

void enqueue(Queue *q, Process *p) {
	Node * node = (Node *) malloc(sizeof(Node));

	node->process = (Process *) malloc(sizeof(Process));
	node->process = p;
	node->prev = q->tail;
	node->next = NULL;

	if(is_empty(q)) {
		q->head = node;
	}
	else {
		q->tail->next = node;
	}
	q->tail = node;
	q->size++;
}

Process *dequeue(Queue *q) {
	Process *p = (Process *) malloc(sizeof(Process));
	
	if(is_empty(q)) {
		return p;
	}
	else if(size_queue(q) == 1) {
		p = q->head->process;
		q->head = NULL;
		q->tail = NULL;
		q->size--;
	}
	else {
		p = q->head->process;
		q->head = q->head->next;
		q->size--;
	}
	
	return p;
}

void print_queue(Queue *q) {
	Node *node = (Node*) malloc(sizeof(Node));
	node = q->head;
	for(int i=0; i<size_queue(q); i++) {
		printf("%d ", node->process->pid);
		node = node->next;
	}
	printf("\n");
}

Process *create_process(int i) {
	int cpu_burst = 1 + rand() % (MAX_CPU_BURST);
	int io_burst = rand() % (MAX_IO_BURST+1);
	int arrival_time = rand() % (MAX_ARRIVAL+1);
	int priority = 1 + rand() % (MAX_PRIORITY);
	int io_start;
	if(cpu_burst == 1)
		io_start = 0;
	else
		io_start = 1 + rand() % (cpu_burst-1);

	Process *p = (Process*) malloc(sizeof(Process));
	p->pid = i;
	p->cpu_burst_time = cpu_burst;
	if(cpu_burst==1)
		p->io_burst_time = 0;
	else
		p->io_burst_time = io_burst;

	p->arrival_time = arrival_time;
	p->priority = priority;
	init_remained_time(p);

	if(p->io_burst_time != 0 && p->cpu_burst_time != 1) {
		p->io_start = io_start;
	}
	else {
		p->io_start = 0;
	}
	
	return p;
}

void print_process(Process *p) {
	Process *process = (Process*) malloc(sizeof(Process));
	process = p;

	if(p->io_burst_time != 0 && p->cpu_burst_time != 1) {
		printf("| pid : %2d, cpu : %2d, i/o : %2d, arrival_time : %2d, prioriry : %2d, (after cpu_burst %2d, i/o start)\n", p->pid, p->cpu_burst_time, p->io_burst_time, p->arrival_time, p->priority, p->io_start);
	}
	else {
		printf("| pid : %2d, cpu : %2d, i/o : %2d, arrival_time : %2d, prioriry : %2d\n", p->pid, p->cpu_burst_time, p->io_burst_time, p->arrival_time, p->priority);
	}
}

void wait_processing(Queue *wq, Queue *rq) {
	int len = size_queue(wq);
	for(int i=0; i<len; i++) {
		wq->head->process->remained_io_burst_time--;

		if(wq->head->process->remained_io_burst_time == 0) {
			enqueue(rq, dequeue(wq));
		}
		else {
			enqueue(wq, dequeue(wq));
		}
	}
}

void init_remained_time(Process *p) {
	p->remained_cpu_burst_time = p->cpu_burst_time;
	p->remained_io_burst_time = p->io_burst_time;
}

void print_step(int i) {
	printf("step %d ", i);
	for(int i=0; i<WALL-7; i++) {
		printf("=");
	}
	printf("\n");
}

void print_wall() {
	for(int i=0; i<WALL; i++) {
		printf("=");
	}
	printf("\n");
}

void print_option() {
	printf("++option ");
	for(int i=0; i<WALL-9; i++) {
		printf("=");
	}
	printf("\n");
}

void print_wrong() {
	printf("| wrong input!!\n");
	print_wall();
}

void print_Gantt(int result[1000], int T) {
	printf("*Gantt Chart*\n");
	for(int i=0; i<=T; i++) {
		printf("%d", result[i]);
	}
	printf("\n");
}

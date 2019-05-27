#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PROCESS 10
#define MIN_PROCESS 2
#define MAX_CPU_BURST 10
#define MAX_IO_BURST 5
#define MAX_ARRIVAL 10
#define MAX_PRIORITY 10
#define WALL 80
#define EXIT -1

typedef struct Process {
	int pid;
	int remained_cpu_burst_time;
	int remained_io_burst_time;
	int cpu_burst_time;
	int io_burst_time;
	int arrival_time;
	int priority;
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

void init_remained_time(Process *p);

void init_queue(Queue *q);

int size_queue(Queue *q);

int is_empty(Queue *q);

void enqueue(Queue *q, Process *p);

Process *dequeue(Queue *q);

void print_step(int i);

void print_wall();

void print_option();

void print_wrong();

void print_algorithm(int n);


int main() {
	srand(time(NULL));
	int step = 1;
	int num_process, algorithm, p_state;
	Process *process[MAX_PROCESS];
	Queue q;
	init_queue(&q);
	
	while(1) {
		switch (step) {
			case 1: //step 1 : how many process?
			{
				for(int i=0; i<MAX_PROCESS; i++) {
					process[i] = (Process *) malloc(sizeof(Process));
				}
				
				print_step(1);
				printf("| input number of processes(%d<= n <= %d)\n", MIN_PROCESS, MAX_PROCESS);
				printf("| exit : %d\n", EXIT);
				printf("| input : ");
				scanf("%d", &num_process);
				print_wall();
				if(num_process == EXIT) {
					print_wall();
					return 0;
				}
				else if(num_process < MIN_PROCESS || num_process > MAX_PROCESS) {
					print_wrong();
					break;
				}
				else if(num_process >= MIN_PROCESS && num_process <= MAX_PROCESS) {
					printf("| number of processes : %d\n", num_process);
					printf("| pid : 0 => idle\n");
					for(int i=0; i<num_process; i++) {
						process[i] = create_process(i+1);
					}
					step = 2;
					break;
				}
			
				break; //step 1 : break
			}
			case 2: //step 2 : choose algorithm
			{
				print_step(2);
				printf("| 1. FCFS(First Come First Served)\n");
				printf("| 2. SJF(Shortest Job First)\n");
				printf("| 3. Priority\n");
				printf("| 4. RR(Round Robin)\n");
				printf("| 5. Evaluation\n");
				printf("| reset : %d\n", EXIT);
				printf("| input : ");
				scanf("%d", &algorithm);
				print_wall();

				switch (algorithm) {
					case EXIT: 
					{
						print_wall();
						for(int i=0; i<MAX_PROCESS; i++) {
							free(process[i]);
						}
						step = 1;
						break;
					}
					case 2://SJF and Priority need option.
					case 3:
					{
						while(1) {
							print_option();
							printf("| 1. Preemptive\n");
							printf("| 2. Non-Preemptive\n");
							printf("| back : %d\n", EXIT);
							printf("| input : ");
							scanf("%d", &p_state);
							print_wall();
						
							switch (p_state) {
								case EXIT: {
									print_wall();
									step = 2;
									break;
								}
								case 1:
								case 2:
								{
									step = 3;
									break;
								}
								default:
								{
									print_wrong();
									break;
								}
							}
						}
						break;
					}
					case 1://FCFS, RR doesn't need option.
					{
						
						step = 3;
						p_state = -1;
						break;
					}
					case 4:
					{
						step = 3;
						p_state = -1;
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

Process *create_process(int i) {
	int cpu_burst= 1 + rand() % (MAX_CPU_BURST);
	int io_burst = rand() % (MAX_IO_BURST+1);
	int arrival_time = rand() % (MAX_ARRIVAL+1);
	int priority = 1 + rand() % (MAX_PRIORITY);

	Process *p = (Process *) malloc(sizeof(Process));
	p->pid = i;
	p->cpu_burst_time = cpu_burst;
	p->io_burst_time = io_burst;
	p->arrival_time = arrival_time;
	p->priority = priority;
	printf("| pid : %2d, cpu : %2d, i/o : %2d, arrival_time : %2d, prioriry : %2d\n", p->pid, p->cpu_burst_time, p->io_burst_time, p->arrival_time, p->priority);
	
	return p;
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

void print_algorithm(int n) {
	printf("| algorithm : ");
	switch (n) {
		case 1:
			printf("FCFS(First Come First Served)\n");
			break;
		case 2:
			printf("SJF(Shortest Job First)\n");
			break;
		case 3:
			printf("Priority\n");
			break;
		case 4:
			printf("RR(Round Robin\n");
			break;
	}
}
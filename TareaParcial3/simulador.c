#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SIZE 4096
#define TOTAL_MEMORY 32768
#define NUM_FRAMES (TOTAL_MEMORY / PAGE_SIZE)

typedef struct {
    int pid;
    int num_pages;
    int *page_table;
} Process;

typedef struct {
    int process_id;
    int page_number;
} Frame;

Frame frames[NUM_FRAMES];
Process *processes[100];
int process_count = 0;
Process *waiting_processes[100];
int waiting_count = 0;
Process *completed_processes[100];
int completed_count = 0;

int has_enough_memory(int num_pages) {
    int available_frames = 0;
    for (int i = 0; i < NUM_FRAMES; i++) {
        if (frames[i].process_id == -1) {
            available_frames++;
        }
    }
    return available_frames >= num_pages;
}

int find_free_frame() {
    for (int i = 0; i < NUM_FRAMES; i++) {
        if (frames[i].process_id == -1) {
            return i;
        }
    }
    return -1;
}

void allocate_memory(Process *process) {
    for (int i = 0; i < process->num_pages; i++) {
        int frame = find_free_frame();
        if (frame != -1) {
            frames[frame].process_id = process->pid;
            frames[frame].page_number = i;
            process->page_table[i] = frame;
        }
    }
}

void free_memory(Process *process) {
    for (int i = 0; i < process->num_pages; i++) {
        int frame = process->page_table[i];
        frames[frame].process_id = -1;
        frames[frame].page_number = -1;
    }
}

void add_process(int pid, int num_pages) {
    Process *process = (Process *)malloc(sizeof(Process));
    process->pid = pid;
    process->num_pages = num_pages;
    process->page_table = (int *)malloc(num_pages * sizeof(int));
    if (has_enough_memory(num_pages)) {
        allocate_memory(process);
        processes[process_count++] = process;
    } else {
        waiting_processes[waiting_count++] = process;
    }
}

void remove_process(int pid) {
    for (int i = 0; i < process_count; i++) {
        if (processes[i]->pid == pid) {
            free_memory(processes[i]);
            completed_processes[completed_count++] = processes[i];
            for (int j = i; j < process_count - 1; j++) {
                processes[j] = processes[j + 1];
            }
            process_count--;
            break;
        }
    }
}

void display_state() {
    printf("Processes in execution:\n");
    for (int i = 0; i < process_count; i++) {
        printf(" - Process ID: %d, Pages: %d\n", processes[i]->pid, processes[i]->num_pages);
    }

    printf("\nMemory State:\n");
    for (int i = 0; i < NUM_FRAMES; i++) {
        if (frames[i].process_id == -1) {
            printf("Frame %d: Free\n", i);
        } else {
            printf("Frame %d: Process %d, Page %d\n", i, frames[i].process_id, frames[i].page_number);
        }
    }

    printf("\nProcesses waiting for memory:\n");
    for (int i = 0; i < waiting_count; i++) {
        printf(" - Process ID: %d, Pages: %d\n", waiting_processes[i]->pid, waiting_processes[i]->num_pages);
    }

    printf("\nProcesses completed:\n");
    for (int i = 0; i < completed_count; i++) {
        printf(" - Process ID: %d, Pages: %d\n", completed_processes[i]->pid, completed_processes[i]->num_pages);
    }
}

int main() {
    // Initialize frames
    for (int i = 0; i < NUM_FRAMES; i++) {
        frames[i].process_id = -1;
        frames[i].page_number = -1;
    }

    int option, pid, num_pages;
    while (1) {
        printf("\nOptions:\n");
        printf("1. Add process\n");
        printf("2. Terminate process\n");
        printf("3. Exit\n");

        printf("Select an option: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                printf("Enter process ID: ");
                scanf("%d", &pid);
                printf("Enter number of pages: ");
                scanf("%d", &num_pages);
                add_process(pid, num_pages);
                display_state();
                break;
            case 2:
                printf("Enter process ID to terminate: ");
                scanf("%d", &pid);
                remove_process(pid);
                display_state();
                break;
            case 3:
                return 0;
            default:
                printf("Invalid option. Please try again.\n");
        }
    }

    return 0;
}

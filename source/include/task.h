#ifndef TASK_H
#define TASK_H

#define KERNEL_STACK_SIZE 0x8000

typedef struct thread {
  uint32_t esp, ebp, eip;

  uint8_t fp_regs[108];
  uint8_t padding[32];

  page_directory_t *page_directory;
} thread_t;

typedef struct process {
  uint32_t pid;
  char* name;
  thread_t thread;

  struct process* next;

  uint32_t stack;
  uint32_t user_stack;

} process_t;

volatile process_t* current_process;

typedef struct task
{
  int id;
  uint32_t esp, ebp;
  uint32_t eip;
  page_directory_t *page_directory;
  struct task *next;

} task_t;

page_directory_t* clone_directory(page_directory_t *src);
static page_table_t* clone_table(page_table_t *src, uint32_t *physAddr);

void init_tasking();
void task_switch();

uint32_t fork();

void move_stack(void *new_stack_start, uint32_t size);

int getpid();

#endif

#ifndef TASK_H
#define TASK_H

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

int fork();

void move_stack(void *new_stack_start, uint32_t size);

int getpid();

#endif

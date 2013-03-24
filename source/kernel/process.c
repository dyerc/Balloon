#include <kernel.h>

process_t* root_proc;
extern uint32_t initial_esp;

process_t* init_kernel_process()
{
  process_t *init = kmalloc(sizeof(process_t));
  init->pid = 0;
  init->name = "kernel";

  init->stack = initial_esp + 1;
  init->user_stack = 0;

  root_proc = init;

  return init;
}

void set_process_environment(process_t *proc, page_directory_t *directory)
{
  proc->thread.page_directory = directory;
}

process_t* schedule()
{
  if (current_process->next)
    return current_process->next;

  return root_proc;
}

process_t* spawn_process(volatile process_t *parent)
{
  process_t *proc = kmalloc(sizeof(process_t));
  proc->pid = 1;
  proc->name = "forked";

  proc->thread.esp = 0;
  proc->thread.ebp = 0;
  proc->thread.eip = 0;

  root_proc->next = proc;
  proc->next = 0;

  proc->stack = kmalloc(KERNEL_STACK_SIZE) + KERNEL_STACK_SIZE; // Stack grows down
  proc->user_stack = parent->user_stack;

  return proc;
}

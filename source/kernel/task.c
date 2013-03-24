#include <kernel.h>

// Some externs are needed to access members in paging.c...
extern page_directory_t *kernel_directory;
extern page_directory_t *current_directory;
extern process_t* root_proc;

process_t *proc_queue = 0;

void init_tasking()
{
  asm volatile("cli");

  current_process = init_kernel_process();
  set_process_environment((process_t*)current_process, current_directory);
  switch_page_directory(current_process->thread.page_directory);

  asm volatile("sti");
}

uint32_t fork()
{
  asm volatile("cli");

  uint32_t esp, ebp, eip;

  process_t *parent = current_process;
  page_directory_t *dir = clone_directory(current_directory);

  process_t *new_proc = spawn_process(current_process);

  set_process_environment(new_proc, dir);

  eip = read_eip();

  if (current_process == parent)
  {
    asm volatile ("mov %%esp, %0" : "=r" (esp));
    asm volatile ("mov %%ebp, %0" : "=r" (ebp));

    if (current_process->stack > new_proc->stack)
    {
      new_proc->thread.esp = esp - (current_process->stack - new_proc->stack);
      new_proc->thread.ebp = ebp - (current_process->stack - new_proc->stack);
    } else {
      new_proc->thread.esp = esp + (new_proc->stack - current_process->stack);
      new_proc->thread.ebp = ebp - (current_process->stack - new_proc->stack);
    }

    memcpy((void *)(new_proc->stack - KERNEL_STACK_SIZE), (void *)(current_process->stack - KERNEL_STACK_SIZE), KERNEL_STACK_SIZE);

    new_proc->thread.eip = eip;

    asm volatile("sti");

    return new_proc->pid;
  }
  else
  {
    return 0;
  }
}

void switch_task()
{
  if (!root_proc)
    return;

  kprintf("\nSwitch\n");

  uint32_t esp, ebp, eip;
  asm volatile ("mov %%esp, %0" : "=r" (esp));
  asm volatile ("mov %%ebp, %0" : "=r" (ebp));

  eip = read_eip();
  if (eip == 0x10000)
  {
    return;
  }

  current_process->thread.eip = eip;
  current_process->thread.esp = esp;
  current_process->thread.ebp = ebp;

  current_process = schedule();

  eip = current_process->thread.eip;
  esp = current_process->thread.esp;
  ebp = current_process->thread.ebp;

  current_directory = current_process->thread.page_directory;
  switch_page_directory(current_directory);
  // Update TSS esp

  asm volatile (
    "mov %0, %%ebx\n"
    "mov %1, %%esp\n"
    "mov %2, %%ebp\n"
    "mov %3, %%cr3\n"
    "mov $0x10000, %%eax\n" /* read_eip() will return 0x10000 */
    "jmp *%%ebx"
    : : "r" (eip), "r" (esp), "r" (ebp), "r" (current_directory->physicalAddr)
    : "%ebx", "%esp", "%eax");
}

static page_table_t *clone_table(page_table_t *src, uint32_t *physAddr)
{
    // Make a new page table, which is page aligned.
    page_table_t *table = (page_table_t*)kmalloc_aligned_physical(sizeof(page_table_t), physAddr);
    // Ensure that the new table is blank.
    memset(table, 0, sizeof(page_directory_t));

    // For every entry in the table...
    int i;
    for (i = 0; i < 1024; i++)
    {
        // If the source entry has a frame associated with it...
        if (src->pages[i].frame)
        {
            // Get a new frame.
            alloc_frame(&table->pages[i], 0, 0);
            // Clone the flags from source to destination.
            if (src->pages[i].present) table->pages[i].present = 1;
            if (src->pages[i].rw) table->pages[i].rw = 1;
            if (src->pages[i].user) table->pages[i].user = 1;
            if (src->pages[i].accessed) table->pages[i].accessed = 1;
            if (src->pages[i].dirty) table->pages[i].dirty = 1;
            // Physically copy the data across. This function is in process.s.
            copy_page_physical(src->pages[i].frame*0x1000, table->pages[i].frame*0x1000);
        }
    }
    return table;
}

page_directory_t *clone_directory(page_directory_t *src)
{
    uint32_t phys;
    // Make a new page directory and obtain its physical address.
    page_directory_t *dir = (page_directory_t*)kmalloc_aligned_physical(sizeof(page_directory_t), &phys);
    // Ensure that it is blank.
    memset(dir, 0, sizeof(page_directory_t));

    // Get the offset of tablesPhysical from the start of the page_directory_t structure.
    uint32_t offset = (uint32_t)dir->tablesPhysical - (uint32_t)dir;

    // Then the physical address of dir->tablesPhysical is:
    dir->physicalAddr = phys + offset;

    // Go through each page table. If the page table is in the kernel directory, do not make a new copy.
    int i;
    for (i = 0; i < 1024; i++)
    {
        if (!src->tables[i])
            continue;

        if (kernel_directory->tables[i] == src->tables[i])
        {
            // It's in the kernel, so just use the same pointer.
            dir->tables[i] = src->tables[i];
            dir->tablesPhysical[i] = src->tablesPhysical[i];
        }
        else
        {
            // Copy the table.
            uint32_t phys;
            dir->tables[i] = clone_table(src->tables[i], &phys);
            dir->tablesPhysical[i] = phys | 0x07;
        }
    }
    return dir;
}

int getpid()
{
  return current_process->pid;
}

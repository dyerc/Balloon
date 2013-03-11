#include <kernel.h>
#include "ramfs.h"

extern multiboot_t *multiboot;

uint32_t ramfs_read(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer)
{
  ramfs_file_header_t header = ramfs_file_headers[node->inode];

  if (offset > header.length)
    return 0;

  if (offset + size > header.length)
    size = header.length - offset;

  memcpy(buffer, (uint8_t*)(header.offset + offset), size);

  return size;
}

uint8_t ramfs_mount()
{
  uint32_t ramdisk_start = *((uint32_t*)multiboot->mods_addr);
  uint32_t ramdisk_end = *(uint32_t*)(multiboot->mods_addr + 4);

  ramfs_header = (ramfs_header_t*)ramdisk_start;
  ramfs_file_headers = (ramfs_file_header_t*)(ramdisk_start + sizeof(ramfs_header_t));

  ramfs_nodes = (fs_node_t*)kmalloc(sizeof(fs_node_t) * ramfs_header->nfiles);
  uint32_t i;
  for (i = 0; i < ramfs_header->nfiles; i++)
  {
    ramfs_file_headers[i].offset += ramdisk_start;

    memcpy(ramfs_nodes[i].name, &ramfs_file_headers[i].name, strlen(ramfs_file_headers[i].name));

    ramfs_nodes[i].read = &ramfs_read;

  }

  kprintf("%d files in ramdisk", ramfs_header->nfiles);
}

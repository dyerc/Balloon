#include <kernel.h>
#include "iso9660.h"

extern uint8_t atapi_read(uint8_t drive, uint32_t lba, uint8_t numsects, uint16_t selector, uint32_t edi);
extern unsigned char ide_buf[2048];

int iso9660_mount()
{
  char* block;
  iso9660_volume_descriptor_t *root;
  iso9660_directory_entry_t *root_dir_entry;

  root = (iso9660_volume_descriptor_t*)ide_buf;

  kprintf("\n\nIdentifier: %s\n", root->identifier);

  root_dir_entry = (iso9660_directory_entry_t*)&root->root_dir_record + sizeof(iso9660_directory_entry_t) + 1;

  uint32_t dir_name_length = root_dir_entry->name_length;
  char *name = root_dir_entry + sizeof(iso9660_directory_entry_t);

  kprintf("length: %d name: %s", dir_name_length, name);
}

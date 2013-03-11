#include <kernel.h>

uint32_t read_fs(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer)
{
  if (node->read)
  {
    uint32_t ret = node->read(node, offset, size, buffer);
    return ret;
  }
  else
  {
    return 0;
  }
}

#ifndef RAMFS_H
#define RAMFS_H

typedef struct
{
  uint32_t nfiles;

} __attribute__(( packed )) ramfs_header_t;

typedef struct
{
  uint8_t magic;
  int8_t name[64];
  uint32_t offset;
  uint32_t length;

} __attribute__(( packed )) ramfs_file_header_t;

ramfs_header_t *ramfs_header;
ramfs_file_header_t *ramfs_file_headers;

fs_node_t *ramfs_nodes;

uint8_t ramfs_probe();
uint8_t ramfs_mount();

#endif

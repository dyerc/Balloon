#ifndef FS_H
#define FS_H

struct fs_node;

typedef uint32_t  (*read_type_t)(struct fs_node*, uint32_t, uint32_t, uint8_t*);
typedef uint32_t  (*write_type_t)(struct fs_node*, uint32_t, uint32_t, uint8_t*);
typedef void      (*open_type_t)(struct fs_node*);
typedef void      (*close_type_t)(struct fs_node*);

typedef struct fs_node
{
  char name[128];
  uint32_t mask;
  uint32_t uid;
  uint32_t gid;
  uint32_t flags;
  uint32_t inode;
  uint32_t length;
  uint32_t impl;

  read_type_t read;

} fs_node_t;

#endif

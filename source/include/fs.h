#ifndef FS_H
#define FS_H

struct fs_node;

typedef uint32_t  (*read_type_t)(struct fs_node*, uint32_t, uint32_t, uint8_t*);
typedef uint32_t  (*write_type_t)(struct fs_node*, uint32_t, uint32_t, uint8_t*);
typedef void      (*open_type_t)(struct fs_node*);
typedef void      (*close_type_t)(struct fs_node*);

typedef struct fs_node {
  char name[128];
  uint32_t flags;

} fs_node_t;

#endif

#include <stdio.h>
#include <dirent.h>

typedef struct
{
  unsigned char magic;
  char name[64];
  unsigned int offset;
  unsigned int length;

} ramfs_file_header_t;

int main(int argc, char *argv[])
{
  ramfs_file_header_t headers[64];
  DIR *dir;
  char *cwd[256];
  struct dirent *ent;
  int nheaders = 0, i = 0;
  unsigned int off = sizeof(ramfs_file_header_t) * 64 + sizeof(int);

  getcwd(cwd, sizeof(cwd));
  printf("cwd: %s\n", cwd);

  if ((dir = opendir(cwd)) != NULL)
  {
    while ((ent = readdir(dir)) != NULL)
    {
      if (strlen(ent->d_name) < 3)
        continue;

      strcpy(headers[i].name, ent->d_name);
      headers[i].offset = off;

      printf("File: %s 0x%x\n", headers[i].name, off);
      FILE *in = fopen(headers[i].name, "r");
      if (!in)
        printf("File not found %s\n", headers[i].name);

      fseek(in, 0, SEEK_END);

      headers[i].length = ftell(in);
      off += headers[i].length;

      fclose(in);

      headers[i].magic = 0xBL;
      i++;
      nheaders++;
    }

    closedir(dir);
  }

  printf("Written something %d:%d\n", off, nheaders);

  FILE *wstream = fopen("../ramd", "w");
  unsigned char *data = (unsigned char *)malloc(off);
  //memset(data, 0x0, off);

  fwrite(&nheaders, sizeof(int), 1, wstream);
  fwrite(headers, sizeof(ramfs_file_header_t), 64, wstream);



  for(i = 0; i < nheaders; i++)
  {
   FILE *stream = fopen(headers[i].name, "r");
   printf("Writing: %s[%d]\n", headers[i].name, headers[i].length);
   unsigned char *buf = (unsigned char *)malloc(headers[i].length);
   fread(buf, 1, headers[i].length, stream);
   fwrite(buf, 1, headers[i].length, wstream);
   fclose(stream);
   free(buf);
  }

  fclose(wstream);
  free(data);

  return 0;
}

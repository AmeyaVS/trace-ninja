#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>

#define MAX_BUF 512
#define MAX_FILENAME_SZ 64

static int pid = 0;
static void dump_map(void);

int main(int argc, char** argv) {
  unsigned int val = 1;
  void* handle;
  unsigned int (*func_do_process)(unsigned int);

  // Before
  dump_map();

  handle = dlopen("build/libfibo.so", RTLD_LAZY);
  if(!handle) {
    fprintf(stderr, "Error: %s\n", dlerror());
    return EXIT_FAILURE;
  }

  // After
  dump_map();

  *(void**)(&func_do_process) = dlsym(handle, "fibo");
  if(!func_do_process) {
    fprintf(stderr, "Error: %s\n", dlerror());
    dlclose(handle);
    return EXIT_FAILURE;
  }

  fprintf(stdout, "%u: %u\n", val, func_do_process(val));

  dlclose(handle);

  // After Closing
  dump_map();

  handle = NULL;
  handle = dlopen("build/libfact.so", RTLD_LAZY);
  if(!handle) {
    fprintf(stderr, "Error: %s\n", dlerror());
    return EXIT_FAILURE;
  }

  // After Opening
  dump_map();

  *(void**)(&func_do_process) = dlsym(handle, "fact");
  if(!func_do_process) {
    fprintf(stderr, "Error: %s\n", dlerror());
    dlclose(handle);
    return EXIT_FAILURE;
  }

  fprintf(stdout, "%u: %u\n", val, func_do_process(val));

  dlclose(handle);

  // After closing
  dump_map();
  return EXIT_SUCCESS;
}

static void dump_map(void) {
  const char filePath[] = "/proc/self/maps";
  char buf[MAX_BUF];
  FILE *fi = fopen(filePath, "r");
  if(fi) {
    char fName[MAX_FILENAME_SZ];
    snprintf(fName, MAX_FILENAME_SZ, "map.%d.log", pid);
    FILE *fo = fopen(fName, "w");
    if(fo) {
      while(fgets(buf, MAX_BUF - 1, fi)) {
#if DEBUG
        fprintf(stdout, "%s", buf);
#endif // DEBUG
        fprintf(fo, "%s", buf);
      }
      fclose(fo);
    } else {
      fprintf(stderr, "Unable to create: %s file!\n", fName);
    }
    fclose(fi);
  } else {
    fprintf(stderr, "Unable to open: %s\n", filePath);
    exit(-1);
  }
  ++pid;
}



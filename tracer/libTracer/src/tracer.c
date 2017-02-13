#include "tracer.h"

/**
 * Maximum line buffer size = 4096
 * Maximum Filename length = 64
 */
#define MAX_BUF 4096
#define MAX_FILENAME_SZ 64

/**
 * Global Trace File handle.
 */
static FILE *fDump = NULL;
enum Bool {
  false = 0,
  true = 1
};
static enum Bool fDumpOpen = false;
/**
 * Global pid.
 */
static pid_t pid = 0;
/**
 * @brief: trace_init initialize the global state.
 */
void trace_init(void) __attribute__((constructor, no_instrument_function));
/**
 * @ brief: trace_exit cleanup the tracers.
 */
void trace_exit(void) __attribute__((destructor, no_instrument_function));
/**
 * @breif: Dump Memory map for the process.
 */
static void dump_map(void) __attribute__((no_instrument_function));
/**
 * @brief: Open function trace file.
 */
static void trace_file_open(void) __attribute__((no_instrument_function));
/**
 * @brief: Close function trace file.
 */
static void trace_file_close(void) __attribute__((no_instrument_function));
/**
 * @brief: Dump trace.
 */
static void trace_write(struct traceAttributes *tb) __attribute__((no_instrument_function));
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
/**
 * @ brief: Entry Tracing functions.
 */
void __cyg_profile_func_enter(void *func, void *callsite) __attribute__((no_instrument_function));
/**
 * @ brief: Exit Tracing functions.
 */
void __cyg_profile_func_exit(void *func, void *callsite) __attribute__((no_instrument_function));
#ifdef __cplusplus
}
#endif // __cplusplus


/**
 * Global Trace Initialization function.
 */
void trace_init(void) {
  pid = getpid();
  dump_map();
  trace_file_open();
}

/**
 * @brief: Global Trace Exit Function.
 */
void trace_exit(void) {
  trace_file_close();
}


/**
 * @brief: Dump trace.
 */
static void trace_write(struct traceAttributes *tb) {
  if(fDumpOpen == false) {
    trace_init();
  }
  fwrite(tb, sizeof(struct traceAttributes), 1, fDump);
};

static void trace_file_open(void) {
  if (!fDumpOpen) {
    // Open function trace file.
    char fName[MAX_FILENAME_SZ];
    snprintf(fName, MAX_FILENAME_SZ, "trace.%d.bin", pid);
    fDump = fopen(fName, "ab+");
    if(fDump) {
      fprintf(stderr, "Trace File Opened: %s\n", fName);
      fDumpOpen = true;
    } else {
      fprintf(stderr, "Unable to create: %s file!\n", fName);
      exit(-1);
    }
  }
}

static void trace_file_close(void) {
  if(fDumpOpen) {
    fclose(fDump);
    fDump = NULL;
    fDumpOpen = false;
  }
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
}

void __cyg_profile_func_enter(void *func, void *callsite) {
  struct traceAttributes tb;
  clock_gettime(CLOCK_MONOTONIC, &tb.tInstant);
  tb.type = 'B';
  tb.func = func;
  tb.callsite = callsite;
  tb.currentThread = pthread_self();
  trace_write(&tb);
}

void __cyg_profile_func_exit(void *func, void *callsite) {
  struct traceAttributes tb;
  clock_gettime(CLOCK_MONOTONIC, &tb.tInstant);
  tb.type = 'E';
  tb.func = func;
  tb.callsite = callsite;
  tb.currentThread = pthread_self();
  trace_write(&tb);
}


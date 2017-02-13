#include "utils.h"
#include <cstring>
#include <cstdio>
#include <iostream>

char* gnu_basename(char *path) {
  char *base = strrchr(path, '/');
  return base ? base + 1: path;
}

static void print_json_header(FILE *jsonFileHandle) {
  fprintf(jsonFileHandle, "{\"traceEvents\":[\n");
}

static void print_json_traces(FILE *traceIn, FILE *jsonFileHandle, pid_t pid) {
  traceAttributes tData;
  fread(&tData, sizeof(traceAttributes), 1, traceIn);
  while(!feof(traceIn)) {
    uint64_t tIns = tData.tInstant.tv_nsec % 1000;
    uint64_t tInstant = (tData.tInstant.tv_sec * 1000000) + (tData.tInstant.tv_nsec / 1000);
    //uint64_t tInstant = (tData.tInstant.tv_sec * 1000000000) + (tData.tInstant.tv_nsec);
    //tInstant = tInstant / 1000;
    //fprintf(jsonFileHandle, "{\"ts\":%lu,\"ph\":\"%c\",\"pid\":%u,\"tid\":%lu,\"name\":\"%p\"}",
    //        tInstant, tData.type, pid, tData.currentThread, tData.func);
    fprintf(jsonFileHandle, "{\"ts\":%lu.%03lu,\"ph\":\"%c\",\"pid\":%u,\"tid\":%lu,\"name\":\"%p\"}",
            tInstant, tIns, tData.type, pid, tData.currentThread, tData.func);
    fread(&tData, sizeof(traceAttributes), 1, traceIn);
    if (!feof(traceIn)) {
      fprintf(jsonFileHandle, ",\n");
    } else {
      fprintf(jsonFileHandle, "\n");
    }
  }
}

static void print_json_tail(FILE *jsonFileHandle) {
  fprintf(jsonFileHandle, "],\n\"displayTimeUnit\": \"ns\"\n}\n");
  //fprintf(jsonFileHandle, "]\n}\n");
}

pid_t extractPID(const char* traceFile) {
  char buf[MAX_LEN] = {'\0'};
  strncpy(buf, traceFile, MAX_LEN);
  buf[MAX_LEN-1] = '\0';
  char* traceFileBaseName = gnu_basename(buf);
#if DEBUG
  fprintf(stderr, "Basename: %s\n", traceFileBaseName);
#endif // DEBUG
  char *temp = strtok(traceFileBaseName, ".");
  temp = strtok(NULL, ".");
  pid_t pid = atoi(temp);
  return pid;
}

void print_json(const char *tracefile, const char* outFile) {
  FILE *jsonFileHandle = NULL;
  if (outFile == NULL) {
    jsonFileHandle = stdout;
  } else {
    jsonFileHandle = fopen(outFile, "w");
    if (jsonFileHandle == NULL) {
      std::cerr << "Unable to create output json file\n";
      exit(-1);
    }
  }
  pid_t pid = extractPID(tracefile);
  FILE *fTraceIn = fopen(tracefile, "rb");
  if (fTraceIn != NULL) {
    print_json_header(jsonFileHandle);
    print_json_traces(fTraceIn, jsonFileHandle, pid);
    print_json_tail(jsonFileHandle);
    if (outFile == NULL) {
      fclose(jsonFileHandle);
    }
  }
  fclose(fTraceIn);
}

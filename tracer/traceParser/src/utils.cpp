#include <cstring>
#include <cstdio>
#include <iostream>
#include "utils.h"
#include "mapParser.h"

char* gnu_basename(char *path) {
  char *base = strrchr(path, '/');
  return base ? base + 1: path;
}

static void print_json_header(FILE *jsonFileHandle) {
  fprintf(jsonFileHandle, "{\"traceEvents\":[\n");
}

static void resolve_symbol(FILE *jsonFileHandle, void* funcHandle, std::vector<map_client::mapEntry>& mapDb, std::vector<nm_client::nmEntry>& nmDb) {
  if (nmDb.size() > 0) {
    using mapIt = std::vector<map_client::mapEntry>::iterator;
    using nmIt = std::vector<nm_client::nmEntry>::iterator;

    mapIt bMap = mapDb.begin();
    mapIt eMap = mapDb.end();
    uint64_t address = uint64_t(funcHandle);
    uint64_t baseOffset = 0;
    char *fileName = NULL;
    bool mapFound = false;
    bool symbFound = false;
    for(;bMap != eMap; ++bMap) {
      if (((*bMap).range.bOffset >= address) && (address <= (*bMap).range.eOffset)) {
        baseOffset = (*bMap).range.bOffset;
        if ((*bMap).filePath) {
          std::stringstream ss;
          ss << (*bMap).filePath;
          char buf[MAX_LEN] = {'\0'};
          snprintf(buf, MAX_LEN, "%s", ss.str().c_str());
          fileName = gnu_basename(buf);
#if DEBUG
          std::cout << "Map Found: " << fileName << std::endl;
#endif // DEBUG
          mapFound = true;
        }
        break;
      }
    }
    if (mapFound) {
      nmIt bNm = nmDb.begin();
      nmIt eNm = nmDb.end();

      for(; bNm != eNm; ++bNm) {
        char type = (*bNm).type;
        //std::cout << "Symb: " << (*bNm).symbName << std::endl;
        // Fix-Me: Find best algorithm to search for the symbol fast.
        // Currently brute force is used.
        if (strstr((*bNm).fileName.c_str(), fileName) != NULL) {
          switch(type) {
            case 't':
            case 'T':
            case 'W':
            case 'w':
              if ((*bNm).offset) {
                if (((*bNm).offset == address) || ((*bNm).offset == (address - baseOffset))) {
                  if ((strstr((*bNm).symbName.c_str(), ".text") == NULL)&&(strstr((*bNm).symbName.c_str(), ".weak") == NULL)) {
                    symbFound = true;
#if DEBUG
                    std::cout << "Symb Found: " << fileName << (*bNm).symbName << std::endl;
#endif // DEBUG
                  }
                }
              }
            break;
            default:
            break;
          }
          if (symbFound) {
            break;
          }
        }
      }

      if (symbFound) {
#if DEBUG
        fprintf(jsonFileHandle, "%s\", \"func\": \"%p\"}", (*bNm).symbName.c_str(), funcHandle);
#else
        fprintf(jsonFileHandle, "%s\"}", (*bNm).symbName.c_str());
#endif // DEBUG
      } else {
        fprintf(jsonFileHandle, "%p\"}", funcHandle);
      }
    } else {
      fprintf(jsonFileHandle, "%p\"}", funcHandle);
    }
  } else {
    fprintf(jsonFileHandle, "%p\"}", funcHandle);
  }
}

static void print_json_traces(FILE *traceIn, FILE *jsonFileHandle, pid_t pid, std::vector<map_client::mapEntry>& mapDb, std::vector<nm_client::nmEntry>& nmDb) {
  traceAttributes tData;
  fread(&tData, sizeof(traceAttributes), 1, traceIn);
  while(!feof(traceIn)) {
    uint64_t tIns = tData.tInstant.tv_nsec % 1000;
    uint64_t tIus = tData.tInstant.tv_nsec / 1000;
    uint64_t tInstant = (tData.tInstant.tv_sec * 1000000) + (tIus);
    fprintf(jsonFileHandle, "{\"ts\":%lu.%03lu,\"ph\":\"%c\",\"pid\":%u,\"tid\":%lu,\"name\":\"",
            tInstant, tIns, tData.type, pid, (uint64_t)tData.currentThread);
    if (tData.type == 'B') {
      resolve_symbol(jsonFileHandle, tData.func, mapDb, nmDb);
    } else {
      fprintf(jsonFileHandle, "\"}");
    }
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
}

static pid_t extractPID(const char* traceFile) {
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

void print_json(const char *tracefile, const char* outFile, std::vector<nm_client::nmEntry>& nmDb) {
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
  // Populate the map info from map.<pid>.log file generated from trace.
  char buf[MAX_LEN] = {'\0'};
  strncpy(buf, tracefile, MAX_LEN);
#if DEBUG
  std::cout << "Hello: " << buf << std::endl;
#endif // DEBUG
  char* pos = gnu_basename(buf);
  *pos = '\0';
  char mapfile[MAX_LEN] = {'\0'};
  snprintf(mapfile, MAX_LEN, "%smap.%d.log", buf, pid);
#if DEBUG
  std::cout << "som: " << mapfile << std::endl;
#endif // DEBUG
  std::vector<map_client::mapEntry> mapDb;
  populateMapDb(mapfile, mapDb);

  FILE *fTraceIn = fopen(tracefile, "rb");
  if (fTraceIn != NULL) {
    print_json_header(jsonFileHandle);
    print_json_traces(fTraceIn, jsonFileHandle, pid, mapDb, nmDb);
    print_json_tail(jsonFileHandle);
    if (outFile == NULL) {
      fclose(jsonFileHandle);
    }
  }
  fclose(fTraceIn);
}

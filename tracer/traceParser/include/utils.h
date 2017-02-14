#ifndef UTILS_H_
#define UTILS_H_

#include <vector>
#include "tracer.h"
#include "nmParser.h"
#define MAX_LEN 512

char* gnu_basename(char *path);
void print_json(const char* tracefile, const char* outFile, std::vector<nm_client::nmEntry>& nmDb);

#endif // UTILS_H_

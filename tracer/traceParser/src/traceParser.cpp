#include <iostream>
#include <fstream>
#include <string>
#include "utils.h"

void usage(const char* pgName) {
  fprintf(stderr, "Usage: %s -t <binary_trace_file> -s <symbol_mapping_file> -o <output file>\n", pgName);
  fprintf(stderr, "Options:\n\t-t <binary_trace_file>: Path to the binray trace file generated.\n\t-s <symbol_mapping_file>: Symbol mapping file created from nm/addr2line tool.\n\t-o <output file>: Output File in JSON format for Chrome about:tracing tool. Optional in-case the option is missing stdout will be used to spit out the traces.\n\t-h help: This help.\n");
}

int main(int argc, char *argv[]) {
  char option = '\0';
  char *tracefile = NULL;
  char *symbfile = NULL;
  char *outFile = NULL;
  if (argc == 1) {
    goto error;
  }
  while((option = getopt(argc, argv, "t:s:o:h")) != EOF) {
    switch(option) {
      case 't':
        tracefile = optarg;
        break;
      case 'o':
        outFile = optarg;
        break;
      case 's':
        symbfile = optarg;
        break;
      case 'h':
        goto error;
         break;
      default:
        fprintf(stderr, "Try %s -h\n", argv[0]);
        exit(-1);
        break;
    }
  }

  if (tracefile) {
    print_json(tracefile, outFile);
  } else {
    fprintf(stderr, "Input trace file not provided!\n");
    goto error;
  }

  return 0;
error:
  usage(argv[0]);
  exit(-1);
}

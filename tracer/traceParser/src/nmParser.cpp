#include "nmParser.h"

namespace nm_client {

  std::ostream& operator<<(std::ostream& os, nmMetaData const& symbInfo) {
    os << symbInfo.symbFile << ":" << symbInfo.symbLine;
    return os;
  }

  std::ostream& operator<<(std::ostream& os, nmEntry const& nEntry) {
    os << nEntry.fileName << ":";
    if (nEntry.offset) {
      os << std::hex << nEntry.offset;
      os << std::dec;
    } else {
    }
    os << " " << nEntry.type << " " << nEntry.symbName;
    if (nEntry.symbInfo) {
      os << nEntry.symbInfo;
    }
    return os;
  }

} // nm_client

void populateNMDb(const char* nmFile, std::vector<nm_client::nmEntry>& nmDb) {
  if (nmFile != NULL) {
    std::ifstream nmFileHandle(nmFile, std::ios::in);
    if (nmFileHandle.is_open()) {
      std::string line;
      while(std::getline(nmFileHandle, line)) {
        parseNMEntry(line, nmDb);
      }
      nmFileHandle.close();
    } else {
      std::cerr << "Unable to open: " << nmFile << std::endl;
      exit(-1);
    }
  }
}

void parseNMEntry(std::string const& input, std::vector<nm_client::nmEntry>& nmDb) {
  using It = std::string::const_iterator;
  NMParse<It> const nmP;

  It f = input.begin();
  It l = input.end();

  nm_client::nmEntry nmData;

  bool ok = qi::phrase_parse(f, l, nmP, qi::ascii::space, nmData);

  if (ok) {
    nmDb.push_back(nmData);
#if DEBUG
    std::cout << nmData << std::endl;
#endif // DEBUG
  } else {
    std::cout << "NMParse failed: (" << input << ")\n";
    exit(-1);
  }

  if (f != l) {
    std::cout << "Remaining unparsed input: '" << std::string(f, l) << "'\n";
  }
}

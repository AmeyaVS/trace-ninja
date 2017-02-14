#include "mapParser.h"

namespace map_client {
  std::ostream& operator<<(std::ostream& os, mapRange const& mr) {
    os << std::hex << mr.bOffset << "-" << mr.eOffset;
    return os;
  }

  std::ostream& operator<<(std::ostream& os, mapDev const& md) {
    os << md.major << ":" << md.minor;
    return os;
  }

  std::ostream& operator<<(std::ostream& os, mapEntry const& mEntry) {
    os << mEntry.range
      << " "
      << mEntry.permissions
      << " "
      << mEntry.offset
      << " "
      << mEntry.dev
      << " ";
    os << std::dec;
    os << mEntry.inode;
    if (mEntry.filePath) {
      os << mEntry.filePath;
    }
    return os;
  }

} // map_client

void parseMapEntry(std::string const& input, std::vector<map_client::mapEntry>& mapDb) {
  using It = std::string::const_iterator;
  MapParse<It> const mapP;

  It f = input.begin();
  It l = input.end();
  map_client::mapEntry mapData;

  bool ok = qi::phrase_parse(f, l, mapP, qi::ascii::space, mapData);

  if (ok) {
#if DEBUG
    std::cout << mapData << std::endl;
#endif // DEBUG
    mapDb.push_back(mapData);
  } else {
    std::cout << "MapParser failed: (" << input << ")\n";
    exit(-1);
  }

  if (f != l) {
    std::cout << "Remaining unparsed input: '" << std::string(f, l) << "'\n";
  }

}

void populateMapDb(const char* mapFile, std::vector<map_client::mapEntry>& mapDb) {
  if (mapFile != NULL) {
    std::ifstream mapFileHandle(mapFile, std::ios::in);
    if (mapFileHandle.is_open()) {
      std::string line;
      while(std::getline(mapFileHandle, line)) {
        parseMapEntry(line, mapDb);
      }
      mapFileHandle.close();
    } else {
      std::cerr << "Unable to open: " << mapFile << std::endl;
      exit(-1);
    }
  }
}

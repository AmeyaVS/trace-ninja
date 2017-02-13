#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

#include <boost/spirit/include/qi.hpp>
#include <boost/optional/optional_io.hpp>
#include <boost/fusion/adapted.hpp>

namespace qi = boost::spirit::qi;

namespace map_client {
  namespace qi = boost::spirit::qi;
  namespace ascii = boost::spirit::ascii;

  struct mapRange {
    uint64_t bOffset; //< Begin Offset
    uint64_t eOffset; //< End Offset
  }; // mapRange

  struct mapDev {
    uint64_t major; //< major number
    uint64_t minor; //< minor number
  }; // mapDev

  struct mapEntry {
    mapRange range; //< memmap range
    std::string permissions; //< map Permission
    uint64_t offset; //< Offset
    mapDev dev; //< Dev major:minor
    uint64_t inode; //< Inode number
    boost::optional<std::string> filePath; //< FilePath
  }; // mapEntry

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

BOOST_FUSION_ADAPT_STRUCT(map_client::mapDev, major, minor);
BOOST_FUSION_ADAPT_STRUCT(map_client::mapRange, bOffset, eOffset);
BOOST_FUSION_ADAPT_STRUCT(map_client::mapEntry, range, permissions, offset, dev, inode, filePath);

template<typename It, typename Skipper = qi::ascii::space_type>
struct MapParse : qi::grammar<It, map_client::mapEntry(), Skipper> {
  MapParse() : MapParse::base_type(start) {
    using namespace qi;

    // lexmes
    num = uint_parser<uint64_t, 16, 1, 16>{};
    dev = (num >> lit(':') >> num);
    filePath = +(graph);
    perms = +(graph - lit(' '));
    range = (num >> lit('-') >> num);

    start = range
            >> perms
            >> num
            >> dev
            >> uint_
            >> -filePath;

    BOOST_SPIRIT_DEBUG_NODES((start)(num)(dev)(filePath)(perms)(range));
  }

  private:
  qi::rule<It, map_client::mapEntry(), Skipper> start;
  qi::rule<It, map_client::mapDev(), Skipper> dev;
  qi::rule<It, map_client::mapRange(), Skipper> range;

  // lexmes
  qi::rule<It, std::string()> filePath;
  qi::rule<It, std::string()> perms;
  qi::rule<It, uint64_t()> num;
}; // MapParse

void mapParse(std::string const& input) {
  using It = std::string::const_iterator;
  MapParse<It> const mP;

  It f = input.begin(), l = input.end();
  map_client::mapEntry mData;

  bool ok = qi::phrase_parse(f, l, mP, qi::ascii::space, mData);

  if (ok) {
    std::cout << mData << std::endl;
  } else {
    std::cout << "Parser failed: (" << input << ")\n";
  }

  if (f != l) {
    std::cout << "Remaining unparsed input: '" << std::string(f, l) << "'\n";
  }
}

int main(int argc, char *argv[]) {
  std::ifstream iFile;
  if (argc == 2) {
    iFile.open(argv[1], std::ios::in);
    if (iFile.is_open()) {
      std::string line;
      while(std::getline(iFile, line)) {
        mapParse(line);
      }
      iFile.close();
    } else {
      std::cerr << "Unable to open: " << argv[1] << std::endl;
      exit(-1);
    }
  }
}

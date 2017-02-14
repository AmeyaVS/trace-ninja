#ifndef MAPPARSER_H_
#define MAPPARSER_H_

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

  std::ostream& operator<<(std::ostream& os, mapRange const& mr);
  std::ostream& operator<<(std::ostream& os, mapDev const& md);
  std::ostream& operator<<(std::ostream& os, mapEntry const& mEntry);
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
    filePath = *(char_);
    perms = +(graph - lit(' '));
    range = (num >> lit('-') >> num);

    start = range
            >> perms
            >> num
            >> dev
            >> num
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


void parseMapEntry(std::string const& input, std::vector<map_client::mapEntry>& mapDb);
void populateMapDb(const char* mapFile, std::vector<map_client::mapEntry>& mapEntry);

#endif // MAPPARSER_H_

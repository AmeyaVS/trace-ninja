#ifndef NMPARSER_H_
#define NMPARSER_H_

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

#include <boost/spirit/include/qi.hpp>
#include <boost/optional/optional_io.hpp>
#include <boost/fusion/adapted.hpp>

namespace qi = boost::spirit::qi;

namespace nm_client {
  namespace qi = boost::spirit::qi;
  namespace ascii = boost::spirit::ascii;

  // nm symbol debug info: <filename>:<line number>
  struct nmMetaData {
    std::string symbFile;
    size_t symbLine;
  }; // nmMetaData

  struct nmEntry {
    std::string fileName;
    boost::optional<uint64_t> offset;
    char type;
    std::string symbName;
    boost::optional<nmMetaData> symbInfo;
  }; // nmEntry

  std::ostream& operator<<(std::ostream& os, nmMetaData const& symbInfo);
  std::ostream& operator<<(std::ostream& os, nmEntry const& nEntry);
} // nm_client

BOOST_FUSION_ADAPT_STRUCT(nm_client::nmMetaData, symbFile, symbLine);
BOOST_FUSION_ADAPT_STRUCT(nm_client::nmEntry, fileName, offset, type, symbName, symbInfo);

template<typename It, typename Skipper = qi::ascii::space_type>
struct NMParse : qi::grammar<It, nm_client::nmEntry(), Skipper> {
  NMParse() : NMParse::base_type(start) {
    using namespace qi;

    // lexmes
    fileName = +(graph - ':') >> ':';
    offset = uint_parser<uint64_t, 16, 1, 16>{};
    symbName = +(char_ - lit("\t"));
    symbInfo = +(graph - ':') >> ':' >> uint_;

    start = fileName
            >> -offset
            >> char_
            >> symbName
            >> -symbInfo;

    BOOST_SPIRIT_DEBUG_NODES((start)(fileName)(offset)(symbName)(symbInfo))
  }

  private:
  qi::rule<It, nm_client::nmMetaData(), Skipper> symbInfo;
  qi::rule<It, nm_client::nmEntry(), Skipper> start;

  // lexmes
  qi::rule<It, std::string()> fileName;
  qi::rule<It, uint64_t()> offset;
  qi::rule<It, std::string()> symbName;
}; // NMParse

void parseNMEntry(std::string const& input, std::vector<nm_client::nmEntry>& nmDb);
void populateNMDb(const char* nmFile, std::vector<nm_client::nmEntry>& nmDb);

#endif // NMPARSER_H_

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

#include <boost/spirit/include/qi.hpp>
#include <boost/optional/optional_io.hpp>
#include <boost/fusion/adapted.hpp>

/**
 * UINT64: 0: std::string: string_hexParser
 *         1: uint64_t
 */
#define UINT64 1
/**
 * UINT64_PARSER: 0: use hex parser.
 *                1: use uint_parser<uint64_t, 16, 1, 16>{}
 */
#define UINT64_PARSER 1

namespace qi = boost::spirit::qi;

namespace client {
  namespace qi = boost::spirit::qi;
  namespace ascii = boost::spirit::ascii;

  // nm symbol debug info: <filename>:<line number>
  struct nmMetaData {
    std::string symbFile;
    size_t symbLine;
  }; // nmMetaData

  struct nmEntry {
    std::string fileName;
#if UINT64
    boost::optional<uint64_t> offset;
#else
    boost::optional<std::string> offset;
#endif // UINT64
    char type;
    std::string symbName;
    boost::optional<nmMetaData> symbInfo;
  }; // nmEntry

  std::ostream& operator<<(std::ostream& os, nmMetaData const& symbInfo) {
    os << symbInfo.symbFile << ":" << symbInfo.symbLine;
    return os;
  }

  std::ostream& operator<<(std::ostream& os, nmEntry const& nEntry) {
    os << nEntry.fileName << ":";
    if (nEntry.offset) {
      //os << std::setfill('0')  << std::setw(16);
      os << std::hex << nEntry.offset;
      os << std::dec;
    } else {
      os << std::setfill(' ') << std::setw(16);
    }
    os << " " << nEntry.type << " " << nEntry.symbName;
    if (nEntry.symbInfo) {
      os << nEntry.symbInfo;
    }
    return os;
  }
} // client

BOOST_FUSION_ADAPT_STRUCT(client::nmMetaData, symbFile, symbLine);
BOOST_FUSION_ADAPT_STRUCT(client::nmEntry, fileName, offset, type, symbName, symbInfo);

template<typename It, typename Skipper = qi::ascii::space_type>
struct NMParse : qi::grammar<It, client::nmEntry(), Skipper> {
  NMParse() : NMParse::base_type(start) {
    using namespace qi;

    // lexmes
    fileName = +(graph - ':') >> ':';
#if UINT64
#if UINT64_PARSER
    offset = uint_parser<uint64_t, 16, 1, 16>{};
#else
    offset = hex;
#endif // UINT64_PARSER
#else
    offset = +(char_("0-9a-fA-F"));
#endif // UINT64
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
  qi::rule<It, client::nmMetaData(), Skipper> symbInfo;
  qi::rule<It, client::nmEntry(), Skipper> start;

  // lexmes
  qi::rule<It, std::string()> fileName;
#if UINT64
  qi::rule<It, uint64_t()> offset;
#else
  qi::rule<It, std::string()> offset;
#endif // UINT64
  qi::rule<It, std::string()> symbName;

}; // NMParse


void parse(std::string const& input) {
  using It = std::string::const_iterator;
  NMParse<It> const P;

  It f = input.begin(), l = input.end();
  client::nmEntry data;

  bool ok = qi::phrase_parse(f, l, P, qi::ascii::space, data);

  if (ok) {
    //std::cout << "Parse Succeeded: " << data << std::endl;
    std::cout << data << std::endl;
  } else {
    std::cout << "Parse failed: (" << input << ")\n";
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
      std::string  line;
      while(std::getline(iFile, line)) {
        parse(line);
      }
      iFile.close();
    } else {
      std::cerr << "Unable to open: " << argv[1] << std::endl;
      exit(-1);
    }
  }
  return 0;
}

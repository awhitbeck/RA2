#ifndef STRING_PARSER_H
#define STRING_PARSER_H

#include <string>
#include <vector>

#include "TString.h"

class StringParser {
public:
  static std::vector<double> strToDoubles(const std::string &str);
  static bool split(const std::string &str, const std::string &delim, std::string &first, std::string &second);
  static bool split(const std::string &str, const std::string &delim, std::vector<std::string> &parts);
  static void trim(std::string &str);
};



std::vector<double> StringParser::strToDoubles(const std::string &str) {
  std::vector<double> vals;
  std::vector<std::string> strs;
  split(str," ",strs);
  for(std::vector<std::string>::const_iterator it = strs.begin();
      it != strs.end(); ++it) {
    const TString tstr(*it);
    vals.push_back(tstr.Atof());
  }

  return vals;
}


bool StringParser::split(const std::string &str, const std::string &delim, std::string &first, std::string &second) {
  bool hasDelim = false;
  first = str;
  second = "";
  if( str.find(delim) != std::string::npos ) {
    hasDelim = true;
    size_t end = str.find(delim);
    first = str.substr(0,end);
    second = str.substr(end+delim.size(),str.size());
  }
  trim(first);
  trim(second);

  return hasDelim;
}


bool StringParser::split(const std::string &str, const std::string &delim, std::vector<std::string> &parts) {
  std::string rest = str;
  std::string part1 = "";
  std::string part2 = "";
  while( split(rest,delim,part1,part2) ) {
    parts.push_back(part1);
    rest = part2;
  }
  parts.push_back(rest);

  return split(str,delim,part1,part2);
}


void StringParser::trim(std::string &str) {
  while( str.size() && str[0] == ' ' ) str.erase(0,1);
  while( str.size() && str[str.size()-1] == ' ' ) str.erase(str.size()-1,str.size());    
}
#endif

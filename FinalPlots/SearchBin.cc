#include "SearchBin.h"
#include "StringParser.h"

#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


void SearchBin::print() const {
  std::cout << "SearchBin " << idx_ << ": " << yield_ << ", -" << statDn_ << " (stat), +" << statUp_ << " (stat), -" << systDn_ << " (syst), +" << systUp_ << " (syst)" << std::endl;
}


// One line per search bin
std::vector<SearchBin*> SearchBin::createFromTxtFile(const std::string &input) {
  std::vector<SearchBin*> bins;

  // Open file for reading
  std::ifstream file(input.c_str());
  if( !file.is_open() ) {
    std::cerr << "\n\nERROR error opening file '" << input << "'\n";
    throw std::exception();
  }

  // Loop over lines and parse
  unsigned int binIdx = 0;
  std::string line("");
  while( !file.eof() ) {
    std::getline(file,line);
    if( !line.empty() ) {
      if( line.at(0) != '#' ) {
	++binIdx;
	if( binIdx > 36 ) {
	  std::cerr << "\n\nERROR: more than 36 search bins specified in '" << input << "'" << std::endl;
	  throw std::exception();
	}
	std::vector<double> vals = StringParser::strToDoubles(line);
	if( vals.size() == 4 ) {
	  bins.push_back(new SearchBin(binIdx,vals.at(0),vals.at(1),vals.at(1),vals.at(2),vals.at(3)));
	} else if( vals.size() == 5 ) {
	  bins.push_back(new SearchBin(binIdx,vals.at(0),vals.at(1),vals.at(2),vals.at(3),vals.at(4)));
	} else {
	  std::cerr << "\n\nERROR: wrong bin format in '" << input << "'" << std::endl;
	  std::cerr << "  should be : 'yield  stat  systDn  systUp'" << std::endl;
	  std::cerr << "         or : 'yield  statDn  statUp  systDn  systUp'" << std::endl;
	  std::cerr << "  but it is : '" << line << "'" << std::endl;
	  throw std::exception();
	}

      }
    }
  }
  file.close();
  if( bins.size() != 36 ) {
    std::cerr << "\n\nERROR: wrong number (" << bins.size() << ") of search bins specified in '" << input << "'" << std::endl;
    throw std::exception();
  }

  return bins;
}


// One column per search bin
std::vector<SearchBin*> SearchBin::createFromTxtFile2(const std::string &input) {
  std::vector<SearchBin*> bins;

  // Open file for reading
  std::ifstream file(input.c_str());
  if( !file.is_open() ) {
    std::cerr << "\n\nERROR error opening file '" << input << "'\n";
    throw std::exception();
  }

  // Loop over lines and parse
  std::vector< std::vector<double> > matrix;
  unsigned int nLines = 0;
  std::string line("");
  while( !file.eof() ) {
    std::getline(file,line);
    if( !line.empty() ) {
      if( line.at(0) != '#' ) {
	++nLines;
	if( nLines > 5 ) {
	  std::cerr << "\n\nERROR: more than 5 values specified per search bin in '" << input << "'" << std::endl;
	  throw std::exception();
	}
	std::vector<double> vals = StringParser::strToDoubles(line);
	if( vals.size() != 36 ) {
	  std::cerr << "\n\nERROR: wrong number (" << vals.size() << ") of search bins in '" << input << "'" << std::endl;
	  throw std::exception();
	}
	matrix.push_back(vals);
      }
    }
  }
  file.close();
  for(unsigned int i = 0; i < 36; ++i) {
    bins.push_back(new SearchBin(i+1,matrix.at(0).at(i),matrix.at(1).at(i),matrix.at(2).at(i),matrix.at(3).at(i),matrix.at(4).at(i)));
  }

  return bins;
}

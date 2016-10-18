#ifndef CC_SOURCE_FILE_READER
#define CC_SOURCE_FILE_READER

#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "types.cc"

using namespace std;

class SourceFileReader {
 private:
  string fileName;
  ifstream fin;

 public:
  SourceFileReader(string _fileName)
      : fileName(_fileName), fin(fileName.c_str()){};

  ~SourceFileReader() { fin.close(); };

  const char_type get();
  const char_type peek();

  const string peek_n(unsigned int);
  const string get_n(unsigned int);
  const string get_while(vector<char_range_type>);
  const string get_until(char_type);

  bool is_end();
};

const char_type SourceFileReader::get() {
  char_type in;
  if (fin.get(in)) {
    return in;
  }
  return '\0';
}

const char_type SourceFileReader::peek() { return peek_n(1)[0]; }

const string SourceFileReader::peek_n(unsigned int n) {
  auto pos = fin.tellg();

  string out = get_n(n);
  fin.clear();
  fin.seekg(pos);

  return out;
}

const string SourceFileReader::get_n(unsigned int n) {
  // @todo instead of file ops use a buffer to save time
  string out = "";

  for (int i = 0; i < n && !is_end(); i++) {
    auto res = get();
    out += res;
  }

  return out;
}

const string SourceFileReader::get_while(vector<char_range_type> char_ranges) {
  string out = "";
  while (!is_end()) {
    char_type ch = peek();
    bool is_match = false;
    for (const char_range_type& range : char_ranges) {
      if (ch >= range.first && ch <= range.second) {
        out.push_back(ch);
        get();
        is_match = true;
        break;
      }
    }

    if (!is_match) break;
  }
  return out;
}

const string SourceFileReader::get_until(char_type ch_limit) {
  string out = "";
  while (!is_end()) {
    char_type ch = peek();
    if (ch == ch_limit) break;

    out.push_back(ch);
    get();
  }
  return out;
}

bool SourceFileReader::is_end() { return fin.eof(); }

#endif
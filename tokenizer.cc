#ifndef CC_TOKENIZER
#define CC_TOKENIZER

#include <iostream>
#include <vector>
#include "source_file_reader.cc"
#include "token.cc"
#include "token_rule.cc"
#include "types.cc"

using namespace std;

class Tokenizer {
 private:
  SourceFileReader* reader;
  vector<Token*> tokens;
  vector<Token* (*)(SourceFileReader*)> rules;

 public:
  Tokenizer(SourceFileReader* _sfr) : reader(_sfr) {
    rules = {token_rule__php_open_tag, token_rule__variable,
             token_rule__keyword,      token_rule__string,
             token_rule__numeric,      token_rule__special_chars,
             token_rule__spec_name};
  };
  ~Tokenizer();

  vector<Token*> get_tokens() { return tokens; };
  void run();
};

Tokenizer::~Tokenizer() {}

void Tokenizer::run() {
  while (!reader->is_end()) {
    for (auto rule : rules) {
      Token* token = rule(reader);
      if (token != nullptr) {
        tokens.push_back(token);
        cout << *token << endl;
        goto skip_run_while;
      }
    }
    reader->get();

  skip_run_while:;
  }
}

#endif

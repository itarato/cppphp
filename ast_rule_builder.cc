#ifndef CC_AST_RULE_BUILDER
#define CC_AST_RULE_BUILDER

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct ASTRuleGroup;

enum ASTRuleTokenType { NOTHING, TOKEN, DEFINITION, GROUP, OPTION };

ASTRuleTokenType token_type_of(string);

struct ASTRuleToken {
  ASTRuleTokenType type;
  union {
    Token token;
    string definition;
    ASTRuleGroup* group;
  } value;
};

struct ASTRuleGroup {
  vector<ASTRuleToken*> items;
  bool flagAnyNumberRepeat;
};

class AstRuleBuilder {
 private:
  ifstream fin;
  map<string, ASTRuleGroup> rules;

  void parseRule(string&);

 public:
  AstRuleBuilder() : fin("./ast.rule"){};

  void build();
};

void AstRuleBuilder::build() {
  string word;
  while (!fin.eof()) {
    getline(fin, word);
    parseRule(word);
  }
}

void AstRuleBuilder::parseRule(string& rule_raw) {
  istringstream is(rule_raw);
  string rule_name;
  is >> rule_name;
  rule_name.pop_back();

  string raw_token;
  while (is.good()) {
    is >> raw_token;
    ASTRuleTokenType type = token_type_of(raw_token);
  }
}

ASTRuleTokenType token_type_of(string raw) {
  if (raw[0] == 'T') return ASTRuleTokenType::TOKEN;
  if (raw[0] == '_') return ASTRuleTokenType::NOTHING;
  if (raw[0] == '(') return ASTRuleTokenType::GROUP;
  if (raw[0] == '|') return ASTRuleTokenType::OPTION;

  return ASTRuleTokenType::DEFINITION;
}

#endif
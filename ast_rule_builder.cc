#ifndef CC_AST_RULE_BUILDER
#define CC_AST_RULE_BUILDER

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "token.cc"

using namespace std;

Token ast_raw_token_to_token(string);

struct ASTRuleConcatGroup;
struct ASTRuleOrGroup;

enum ASTRuleTokenType {
  NOTHING,
  TOKEN,
  DEFINITION,
  OR_GROUP,
  CONCAT_GROUP,
  OPTION,
  GROUP_CLOSE
};

ASTRuleTokenType token_type_of(string);

struct ASTRuleToken {
  ASTRuleTokenType type;
  union {
    Token token;
    string definition;
    ASTRuleOrGroup* or_group;
    // Nothing is only defined in the struct's `type`.
  } value;

  void debug(string);
};

struct ASTRuleConcatGroup {
  vector<ASTRuleToken*> tokens;

  void debug(string);
};

struct ASTRuleOrGroup {
  vector<ASTRuleConcatGroup*> options;
  bool flagAnyNumberRepeat;

  void debug(string);
};

class AstRuleBuilder {
 private:
  ifstream fin;
  map<string, ASTRuleOrGroup*> rules;

  void parse_rule(string&);
  ASTRuleOrGroup* parse_rule_group(istringstream&);

 public:
  AstRuleBuilder() : fin("./ast.rule"){};

  const map<string, ASTRuleOrGroup*>* get_rules() const { return &rules; };

  void build();
};

ASTRuleOrGroup* AstRuleBuilder::parse_rule_group(istringstream& is) {
  ASTRuleOrGroup* group = new ASTRuleOrGroup{};
  ASTRuleConcatGroup* currentConcatGroup = new ASTRuleConcatGroup{};

  group->options.push_back(currentConcatGroup);

  string raw;
  while (is.good()) {
    is >> raw;
    ASTRuleTokenType type = token_type_of(raw);

    if (type == ASTRuleTokenType::GROUP_CLOSE) {
      if (raw.size() >= 2 && raw[1] == '*') {
        group->flagAnyNumberRepeat = true;
      }
      break;
    } else if (type == ASTRuleTokenType::OR_GROUP) {
      ASTRuleOrGroup* _group = parse_rule_group(is);
      ASTRuleToken* _token = new ASTRuleToken{};
      _token->type = ASTRuleTokenType::OR_GROUP;
      _token->value.or_group = _group;
      currentConcatGroup->tokens.push_back(_token);
    } else if (type == ASTRuleTokenType::NOTHING) {
      ASTRuleToken* _token = new ASTRuleToken{};
      _token->type = ASTRuleTokenType::NOTHING;
      currentConcatGroup->tokens.push_back(_token);
    } else if (type == ASTRuleTokenType::TOKEN) {
      ASTRuleToken* _token = new ASTRuleToken{};
      _token->type = ASTRuleTokenType::TOKEN;
      _token->value.token = ast_raw_token_to_token(raw);
      currentConcatGroup->tokens.push_back(_token);
    } else if (type == ASTRuleTokenType::DEFINITION) {
      ASTRuleToken* _token = new ASTRuleToken{};
      _token->type = ASTRuleTokenType::DEFINITION;
      _token->value.definition = raw;
      currentConcatGroup->tokens.push_back(_token);
    } else if (type == ASTRuleTokenType::OPTION) {
      currentConcatGroup = new ASTRuleConcatGroup{};
      group->options.push_back(currentConcatGroup);
    } else {
      throw "Unexpected AST rule type.";
    }
  }

  return group;
}

void AstRuleBuilder::build() {
  string word;
  while (!fin.eof()) {
    getline(fin, word);
    parse_rule(word);
  }

  for (auto it : rules) {
    cout << it.first << endl;
    it.second->debug("  ");
    cout << endl;
  }
}

void AstRuleBuilder::parse_rule(string& rule_raw) {
  istringstream is(rule_raw);
  string rule_name;
  is >> rule_name;
  rule_name.pop_back();

  ASTRuleOrGroup* group = parse_rule_group(is);
  rules[rule_name] = group;
}

// STRUCT METHODS /////////////////////////////////////////////////////////////

void ASTRuleOrGroup::debug(string indent) {
  cout << indent << "OR GROUP" << (flagAnyNumberRepeat ? " (*):" : ":") << endl;
  for (const auto e : options) {
    e->debug(indent + "  ");
  }
}

void ASTRuleToken::debug(string indent) {
  if (type == ASTRuleTokenType::NOTHING) {
    cout << indent << "NOTHING" << endl;
  } else if (type == ASTRuleTokenType::TOKEN) {
    cout << indent << "TOKEN: " << value.token << endl;
  } else if (type == ASTRuleTokenType::DEFINITION) {
    cout << indent << "DEFINITION: " << value.definition << endl;
  } else if (type == ASTRuleTokenType::OR_GROUP) {
    value.or_group->debug(indent);
  } else {
    cout << indent << "unknown token" << endl;
  }
}

void ASTRuleConcatGroup::debug(string indent) {
  cout << indent << "GROUP:" << endl;
  for (const auto e : tokens) {
    e->debug(indent + "  ");
  }
}

// HELPERS ////////////////////////////////////////////////////////////////////

ASTRuleTokenType token_type_of(string raw) {
  if (raw[0] == 'T') return ASTRuleTokenType::TOKEN;
  if (raw[0] == '_') return ASTRuleTokenType::NOTHING;
  if (raw[0] == '(') return ASTRuleTokenType::OR_GROUP;
  if (raw[0] == '|') return ASTRuleTokenType::OPTION;
  if (raw[0] == ')') return ASTRuleTokenType::GROUP_CLOSE;

  return ASTRuleTokenType::DEFINITION;
}

Token ast_raw_token_to_token(string raw) {
  raw = raw.substr(2, raw.size() - 3);  // Remove trailing 'T(' and leading ')'.
  auto eq_pos = raw.find('=');
  string rawTokenType;
  string value;

  if (eq_pos != string::npos) {
    rawTokenType = raw.substr(0, eq_pos);
    value = raw.substr(eq_pos + 1);
  } else {
    rawTokenType = raw;
    value = "";
  }

  return string_to_token(rawTokenType, value);
}

#endif
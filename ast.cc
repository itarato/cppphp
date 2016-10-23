#ifndef CC_AST
#define CC_AST

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "ast_rule_builder.cc"
#include "token.cc"

using namespace std;

struct ASTNode {
  string type;
  vector<ASTNode*> children;
};

class AST {
 private:
  ASTNode* root;
  AstRuleBuilder ruleBuilder;
  map<string, ASTRuleOrGroup*>* rules;
  /// Evaluating rules can get into an infinite loop when a sub-rule is the
  /// same as a parent rule (any parent) without progressing on the token line.
  /// This loop set is to store parent rule pointers, however practically we
  /// only need or-group checks.
  set<void*> loop_set;

 public:
  AST(vector<Token*>);

  bool try_or_group(vector<Token*>::iterator*, vector<Token*>::iterator*,
                    ASTRuleOrGroup*);
  bool try_concat_group(vector<Token*>::iterator*, vector<Token*>::iterator*,
                        ASTRuleConcatGroup*);
  bool try_token(vector<Token*>::iterator*, vector<Token*>::iterator*,
                 ASTRuleToken*);
};

AST::AST(vector<Token*> tokens) {
  ruleBuilder = AstRuleBuilder();
  ruleBuilder.build();

  rules = ruleBuilder.get_rules();

  auto begin = tokens.begin();
  auto end = tokens.end();
  try_or_group(&begin, &end, (*rules)["PROG"]);
}

bool AST::try_or_group(vector<Token*>::iterator* current,
                       vector<Token*>::iterator* end,
                       ASTRuleOrGroup* or_group) {
  if ((*current) == (*end)) return false;

  bool had_any_match = false;
  bool has_match;

  auto self_loop_pos = loop_set.find(or_group);
  /// Loop is detected (one parent is the same as current without using any
  /// tokens). Return false as this would lead to nowhere.
  if (self_loop_pos != loop_set.end()) return false;

  loop_set.insert(or_group);

  do {
    has_match = false;
    for (auto concat_group : or_group->options) {
      if ((*current) == (*end)) break;

      Token* backup = **current;
      bool match = try_concat_group(current, end, concat_group);
      if (match) {
        has_match = true;
        had_any_match = true;
        break;
      } else {
        **current = backup;
      }
    }
  } while (has_match && or_group->flagAnyNumberRepeat);

  // If it's wildcarded, than 0 counts as true.
  return had_any_match || or_group->flagAnyNumberRepeat;
}

bool AST::try_concat_group(vector<Token*>::iterator* current,
                           vector<Token*>::iterator* end,
                           ASTRuleConcatGroup* concat_group) {
  if ((*current) == (*end)) return false;

  for (auto token : concat_group->tokens) {
    bool match = try_token(current, end, token);
    if (!match) {
      return false;
    }
  }
  return true;
}

bool AST::try_token(vector<Token*>::iterator* current,
                    vector<Token*>::iterator* end, ASTRuleToken* token) {
  if ((*current) == (*end)) return false;

  if (token->type == ASTRuleTokenType::NOTHING) {
    cout << "try token - nothing" << endl;
    return true;
  } else if (token->type == ASTRuleTokenType::TOKEN) {
    cout << "try token - token - " << token->value.token;

    if ((**current)->type == token->value.token.type) {
      if (token->value.token.orig == "" ||
          token->value.token.orig == (**current)->orig) {
        cout << " - YES" << endl;
        (*current)++;
        loop_set.clear();
        return true;
      }
    }

    cout << " - NO" << endl;
    return false;

  } else if (token->type == ASTRuleTokenType::DEFINITION) {
    cout << "try token - definition - " << token->value.definition << endl;
    return try_or_group(current, end, (*rules)[token->value.definition]);
  } else if (token->type == ASTRuleTokenType::OR_GROUP) {
    cout << "try token - or group" << endl;
    return try_or_group(current, end, token->value.or_group);
  }

  throw "Unexpected token type at trying phase.";
}

#endif

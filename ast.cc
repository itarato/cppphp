#ifndef CC_AST
#define CC_AST

#include <iostream>
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
  for (auto concat_group : or_group->options) {
    bool match = try_concat_group(current, end, concat_group);
    if (match) {
      return true;
    }
  }

  // If it's wildcarded, than 0 counts as true.
  return or_group->flagAnyNumberRepeat;
}

bool AST::try_concat_group(vector<Token*>::iterator* current,
                           vector<Token*>::iterator* end,
                           ASTRuleConcatGroup* concat_group) {
  for (auto token : concat_group->tokens) {
    bool match = try_token(current, end, token);
    if (!match) {
      // @todo there should be some rollback here
      return false;
    }
  }
  return true;
}

bool AST::try_token(vector<Token*>::iterator* current,
                    vector<Token*>::iterator* end, ASTRuleToken* token) {
  if (token->type == ASTRuleTokenType::NOTHING) {
    cout << "try token - nothing" << endl;
    return true;
  } else if (token->type == ASTRuleTokenType::TOKEN) {
    cout << "try token - token - " << token->value.token;

    // End of all source tokens.
    if ((*current) == (*end)) {
      cout << " - NO" << endl;
      return false;
    }

    if ((**current)->type == token->value.token.type) {
      if (token->value.token.orig == "" || token->value.token.orig == (**current)->orig) {
        cout << " - YES" << endl;
        (*current)++;
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

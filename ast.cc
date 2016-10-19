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

 public:
  AST(vector<Token>*);
};

AST::AST(vector<Token>* tokens) {
  ruleBuilder = AstRuleBuilder();
  ruleBuilder.build();

  auto rules = ruleBuilder.get_rules();
}

#endif
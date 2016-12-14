#ifndef CC_AST
#define CC_AST

#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include "ast_rule_builder.cc"
#include "token.cc"

using namespace std;

// STRUCTS ////////////////////////////////////////////////////////////////////

struct ASTNode {
  string type;
  vector<ASTNode*> children;

  void debug(string) const;
};

void ASTNode::debug(string padding = "") const {
  cout << padding << type << endl;
  for (const auto& child : children) {
    child->debug(padding + "  ");
  }
}

// CLASS //////////////////////////////////////////////////////////////////////

class AST {
 private:
  ASTNode* root;
  AstRuleBuilder rule_builder;
  map<string, ASTRuleOrGroup*>* rules;
  /// Evaluating rules can get into an infinite loop when a sub-rule is the
  /// same as a parent rule (any parent) without progressing on the token line.
  /// This loop set is to store parent rule pointers, however practically we
  /// only need or-group checks.
  set<void*> loop_set;

 public:
  AST(vector<Token*>);
  ~AST();

  ASTNode* try_or_group(vector<Token*>::iterator*, vector<Token*>::iterator*,
                        ASTRuleOrGroup*);
  ASTNode* try_concat_group(vector<Token*>::iterator*,
                            vector<Token*>::iterator*, ASTRuleConcatGroup*);
  ASTNode* try_token(vector<Token*>::iterator*, vector<Token*>::iterator*,
                     ASTRuleToken*);

  ASTNode* get_root() { return root; };

  void debug() { root->debug(); };
};

AST::AST(vector<Token*> tokens) {
  rule_builder.build();

  rules = rule_builder.get_rules();

  auto begin = tokens.begin();
  auto end = tokens.end();

  root = try_or_group(&begin, &end, (*rules)["PROG"]);
}

AST::~AST() {
  delete root;
}

ASTNode* AST::try_or_group(vector<Token*>::iterator* current,
                           vector<Token*>::iterator* end,
                           ASTRuleOrGroup* or_group) {
  if ((*current) == (*end)) return nullptr;

  bool had_any_match = false;
  bool has_match;

  auto self_loop_pos = loop_set.find(or_group);
  /// Loop is detected (one parent is the same as current without using any
  /// tokens). Return false as this would lead to nowhere.
  if (self_loop_pos != loop_set.end()) return nullptr;

  loop_set.insert(or_group);

  ASTNode* node = new ASTNode{"or"};

  do {
    has_match = false;
    for (auto concat_group : or_group->options) {
      if ((*current) == (*end)) break;

      Token* backup = **current;
      ASTNode* child = try_concat_group(current, end, concat_group);
      if (child != nullptr) {
        has_match = true;
        had_any_match = true;
        node->children.push_back(child);
        break;
      } else {
        **current = backup;
      }
    }
  } while (has_match && or_group->flagAnyNumberRepeat);

  // If it's wildcarded, than 0 counts as true.
  return (had_any_match || or_group->flagAnyNumberRepeat) ? node : nullptr;
}

ASTNode* AST::try_concat_group(vector<Token*>::iterator* current,
                               vector<Token*>::iterator* end,
                               ASTRuleConcatGroup* concat_group) {
  if ((*current) == (*end)) return nullptr;

  ASTNode* node = new ASTNode{"concat"};

  for (auto token : concat_group->tokens) {
    ASTNode* child = try_token(current, end, token);
    if (child == nullptr) return nullptr;
    node->children.push_back(child);
  }

  return node;
}

ASTNode* AST::try_token(vector<Token*>::iterator* current,
                        vector<Token*>::iterator* end, ASTRuleToken* token) {
  if ((*current) == (*end)) return nullptr;

  if (token->type == ASTRuleTokenType::NOTHING) {
#ifdef DEBUG
    cout << "try token - nothing" << endl;
#endif
    return new ASTNode{"nothing"};
  } else if (token->type == ASTRuleTokenType::TOKEN) {
#ifdef DEBUG
    cout << "try token - token - " << token->value.token;
#endif

    if ((**current)->type == token->value.token.type) {
      if (token->value.token.orig == "" ||
          token->value.token.orig == (**current)->orig) {
#ifdef DEBUG
        cout << " - YES" << endl;
#endif
        (*current)++;
        loop_set.clear();

        ostringstream os;
        os << (**(*current - 1));
        return new ASTNode{os.str()};
      }
    }

#ifdef DEBUG
    cout << " - NO" << endl;
#endif
    return nullptr;

  } else if (token->type == ASTRuleTokenType::DEFINITION) {
#ifdef DEBUG
    cout << "try token - definition - " << token->value.definition << endl;
#endif
    return try_or_group(current, end, (*rules)[token->value.definition]);
  } else if (token->type == ASTRuleTokenType::OR_GROUP) {
#ifdef DEBUG
    cout << "try token - or group" << endl;
#endif
    return try_or_group(current, end, token->value.or_group);
  }

  throw "Unexpected token type at trying phase.";
}

#endif

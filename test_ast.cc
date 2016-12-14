#pragma once

#include <string>

#include "test_case.cc"
#include "source_file_reader.cc"
#include "tokenizer.cc"
#include "ast.cc"

using namespace std;

class AstTest : public TestCase {
private:
    void testAst() {
        SourceFileReader sfr("test_cases/ast_simple_test.php");
        Tokenizer tkn(&sfr);
        
        tkn.run();
        
        AST ast(tkn.get_tokens());
        
        ASTNode *root = ast.get_root();
        root->debug("  ");
    }
    
public:
    virtual string getName() { return "AST"; };
    virtual void run() {
        testAst();
    }
};

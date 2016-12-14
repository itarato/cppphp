#include <iostream>
#include <vector>

#include "test_case.cc"
#include "test_tokenizer.cc"

using namespace std;

int main() {
    vector<TestCase *> tests{
        new TokenizerTest(),
    };
    
    for (const auto & t : tests) {
        t->run();
        t->report();
    }
    
    return 0;
}

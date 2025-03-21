#include <Test.h>
#include <DFA.h>
#include <NFA.h>

int main() {
    Test<DFA> t1("words.in"); t1.run();
    Test<NFA> t2("words.in"); t2.run();
    return 0;
}
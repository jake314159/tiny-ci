#include <string>

#include "MakeTest.h"

using namespace std;

int MakeTest::performTest(Result &test)
{
    cout << "Lets run a make test for " << testName << endl;
    test.setReturnValue(0);
    return 1;
}

#include <string>

#include "MakeTest.h"

using namespace std;

#define MAKE_CHECK "make check 1> /dev/null 2> /dev/null"

int MakeTest::checkForChange()
{
    std::string newHash = getLastHash(dir);
    cout << "The old hash is: " << lastHash << endl;
    cout << "The new hash is: " << newHash <<endl;

    int returnValue = newHash.compare(lastHash);
    if( returnValue != 0) {
        lastHash = newHash;
    }

    return returnValue;
}

int MakeTest::performTest(Result &test)
{
    cout << "Lets run a make test for " << testName << endl;

    int value;
    // Check if processor is available
    if (!system(NULL)) {
        exit (EXIT_FAILURE);
    }

    //run command
    std::string cmd = std::string("cd ") + dir + " && " + MAKE_CHECK;
    value = system ((char*)cmd.c_str());

    test.setReturnValue(value);
    return value;
}

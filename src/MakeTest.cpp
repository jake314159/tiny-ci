#include <string>
#include <fstream>
#include <streambuf>
#include "MakeTest.h"

using namespace std;

#define MAKE_CHECK "make check"
#define MAKE "make"
#define TEMP "/home/jake/git/tiny-ci/temp-tiny-ci.txt"
#define PIPE_TO_TEMP " 1> '/home/jake/git/tiny-ci/temp-tiny-ci.txt' 2> '/home/jake/git/tiny-ci/temp-tiny-ci.txt'"

#define RETURN_STRING_SIZE 1024

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
    std::string cmd;
    if(doTest) {
        cmd = std::string("cd ") + dir + " && " + MAKE_CHECK + PIPE_TO_TEMP;
    } else {
        cmd = std::string("cd ") + dir + " && " + MAKE + PIPE_TO_TEMP;
    }
    cout << "Running: " << cmd << endl;
    value = system((char*)cmd.c_str());

    //Load the text from the tempory file
    std::ifstream t(TEMP);
    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    remove(TEMP); //and delete it

    test.setReturnValue(value);
    test.setReturnString(str);

    return value;
}

#include <string>
#include <fstream>
#include <streambuf>
#include "MavenTest.h"
#include "dir.h"

using namespace std;

#define MVN "mvn package"
#define MAKE "make"

int MavenTest::performTest(Result &test)
{
    int value;
    // Check if processor is available
    if (!system(NULL)) {
        exit (EXIT_FAILURE);
    }

    //run command
    std::string cmd = std::string("cd ") + dir + " && " + GIT_PULL + " && " + MVN + " 1> " + TEMP + " 2> " + TEMP;

    value = system((char*)cmd.c_str());

    //Load the text from the tempory file
    std::ifstream t(TEMP);
    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    remove(TEMP); //and delete it

    if(value) {
        mode = PASS;
    } else {
        mode = FAIL;
    }

    test.setReturnValue(value);
    test.setReturnString(str);

    return value;
}


void MavenTest::saveTest(FILE *fp)
{
    cout << "MavenTest saveTest(..) not implemented" << endl;
}


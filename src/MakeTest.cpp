#include <string>
#include <fstream>
#include <streambuf>
#include "MakeTest.h"
#include "dir.h"

using namespace std;

#define MAKE_CHECK "make check"
#define MAKE "make"

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
        cmd = std::string("cd ") + dir + " && " + MAKE_CHECK + " 1> " + TEMP + " 2> " + TEMP;
    } else {
        cmd = std::string("cd ") + dir + " && " + MAKE + " 1> " + TEMP + " 2> " + TEMP;
    }
    cout << "Running: " << cmd << endl;
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


void MakeTest::saveTest(FILE *fp)
{
    fprintf(fp,"%s;%s;%s;%d\n",(char*)testName.c_str(), (char*)dir.c_str(), (char*)url.c_str(), doTest);
}

MakeTest MakeTest::parseSaveString(string inString)
{
    std::string s = inString;
    std::string delim = ";";

    std::string parts[4];

    unsigned int start = 0U;
    unsigned int end = s.find(delim, 0);
    int i=0;
    while (i < 4 && end != std::string::npos)
    {
        parts[i] = s.substr(start, end - start);
        i++;
        start = end + delim.length();
        end = s.find(delim, start);
    }

    bool doTest = true;
    if(!parts[3].compare("0")) doTest = false;

    MakeTest t0(parts[0], parts[1], parts[2], doTest);
    return t0;
}


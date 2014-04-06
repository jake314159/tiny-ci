using namespace std;
#include <iostream>
#include <vector>
#include <unistd.h> //Unix timer. Note this makes this code Unix only!
#include "MakeTest.h"
#include "gitTools.h"

//delay of 60s
#define DELAY 60

const std::string gitRootDir = "tiny-ci-bin";

std::vector<MakeTest> tests;

int onFail(Result &result)
{
    cout << endl << "##############################################################" << 
            endl << "The test you have just run has failed! You better check it out" <<
            endl << "##############################################################" << endl;
    cout << result.getReturnString() << endl << endl;
    return 0;
}

int main(int argc, char *argv[])
{
    //pullNewGit("git@github.com:jake314159/markdown-latex.git", gitRootDir + "/markdown-latex");

    MakeTest t1("markdown-latex", gitRootDir + "/markdown-latex", "git@github.com:jake314159/markdown-latex.git", true);
    tests.push_back(t1);
    MakeTest t2("avr-snake", gitRootDir + "/avr-snake", "git@github.com:jake314159/avr-snake.git", false);
    tests.push_back(t2);
    MakeTest t3("broken markdown-latex", "/home/jake/git/markdown-latex-fail-tests", "", true);
    tests.push_back(t3);
    Result r;

    while(true) {
        int size = tests.size();
        for (int i=0; i<size; i++) {
            int check = tests.at(i).checkForChange();
            cout << "Check for change in " << tests.at(i).getTestName() << ": " << check << endl;
            if(check) {
                if(tests.at(i).performTest(r)) {
                    onFail(r);
                }
                cout << "Result output of the test is " << r.getReturnValue() << endl;
            } else {
                cout << "We don't need to run the test again" << endl;
            }
            cout << endl << endl;
        }
        sleep(DELAY);
    }
}


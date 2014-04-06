using namespace std;
#include <iostream>
#include "MakeTest.h"
#include "gitTools.h"

const std::string gitRootDir = "tiny-ci-bin";

int main(int argc, char *argv[])
{
    //pullNewGit("git@github.com:jake314159/markdown-latex.git", gitRootDir + "/markdown-latex");

    MakeTest t("markdown-latex", gitRootDir + "/markdown-latex", "git@github.com:jake314159/markdown-latex.git");
    //MakeTest t("/home/jake/git/markdown-latex-fail-tests");
    Result r;

    for(int i=0; i<2; i++) {
        int check = t.checkForChange();
        cout << "Check for change: " << check << endl;
        if(check) {
            t.performTest(r);
            cout << "Result output of the test is " << r.getReturnValue() << endl;
        } else {
            cout << "We don't need to run the test again" << endl;
        }
        cout << endl << endl;
    }
    //cout << "Check for change: " << t.checkForChange() << endl;

    //cout << "What is the git hash? " << getLastHash("/home/jake/git/avr-snake") << endl;
}

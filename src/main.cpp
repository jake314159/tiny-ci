using namespace std;
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h> //Unix timer. Note this makes this code Unix only!
#include "MakeTest.h"
#include "gitTools.h"
#include "dir.h"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

//delay of 60s
#define DELAY 60


const char *homedir;

std::string gitRootDir = "";

std::vector<MakeTest> tests;

void initHomeDir()
{
    struct passwd *pw = getpwuid(getuid());
    homedir = pw->pw_dir;
    //cout << "Home dir " << homedir << endl;
    gitRootDir = std::string(homedir) + "/" + HOME_DIR;
    //cout << "git root dir " << gitRootDir << endl;
}

int onFail(Result &result)
{
    cout << endl << "##############################################################" << 
            endl << "The test you have just run has failed! You better check it out" <<
            endl << "##############################################################" << 
            endl;
    cout << result.getReturnString() << endl << endl;
    return 0;
}

void addTaskToStore(MakeTest &t)
{
    FILE *fp = fopen( (char*)(std::string(gitRootDir) + "/.tasks").c_str(),"a");  
    cout << "fp is " << fp << endl;
    if(fp != NULL) {     
        t.saveTest(fp);      
        fclose(fp);
    } else {
        cout << "Err fp is null " << endl;
    }
}

int main(int argc, char *argv[])
{
    initHomeDir();

    MakeTest t0("tiny-ci", gitRootDir + "/tiny-ci", "git@github.com:jake314159/tiny-ci.git", false);
    tests.push_back(t0);
    MakeTest t1("markdown-latex", gitRootDir + "/markdown-latex", "git@github.com:jake314159/markdown-latex.git", true);
    tests.push_back(t1);
    MakeTest t2("avr-snake", gitRootDir + "/avr-snake", "git@github.com:jake314159/avr-snake.git", false);
    tests.push_back(t2);
    MakeTest t3("broken markdown-latex", "/home/jake/git/markdown-latex-fail-tests", "", true);
    tests.push_back(t3);

    /*for (int i=0; i<tests.size(); i++) {
        addTaskToStore(tests.at(i));
    }*/

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


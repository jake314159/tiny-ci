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

void updateTasksFromFile()
{
    std::string taskFile = (std::string(gitRootDir) + "/.tasks");
    std::ifstream input( taskFile );
    for( std::string line; getline( input, line ); )
    {
        MakeTest t = MakeTest::parseSaveString(line);
        bool newTask = true;
        for(int i=0; i<tests.size(); i++) {
            //cout << "Comparing " << tests.at(i).getTestName() << " and " << t.getTestName() << endl;
            if(!tests.at(i).getTestName().compare(t.getTestName())) {
                newTask = false;
                //cout << "They are the same!" << endl;
            }
        }
        if(newTask) {
            tests.push_back(t);
        }
    }
}

int start()
{
    updateTasksFromFile();

    Result r;
    
    while(true) {
        updateTasksFromFile();
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
    return 0;
}

void printHelp()
{
    cout << "Did you mean 'tiny-ci start'?" << endl << endl;
}

int main(int argc, char *argv[])
{
    initHomeDir();
    cout << "argc" << argc << endl;
    if(argc <= 1) {
        cout << "Not enough arguments!" << endl;
        printHelp();
        return 1;
    }
    if(!std::string(argv[1]).compare("start")) {
        start();
    } else if(!std::string(argv[1]).compare("add")) {
        if(argc < 4) {
            cout << "Not enough arguments to make a new task" << endl;
            printHelp();
            return 1;
        }
        
        bool runTest = false;
        if(argc > 4 && !std::string(argv[4]).compare("true")) {
            runTest = true;
        }

        cout << "Name:      " << argv[2] << endl;
        cout << "Directory: " << gitRootDir + "/" + argv[2] << endl;
        cout << "Git URL:   " << argv[3] << endl;
        cout << "Run test:  " << runTest << endl << endl;

        
        MakeTest t(argv[2], gitRootDir + "/" + argv[2], argv[3], runTest);
        addTaskToStore(t);
    } else {
        cout << "Incorrect argument!" << endl;
        printHelp();
        return 1;
    }
    return 0;
}

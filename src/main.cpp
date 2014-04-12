
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>    //Unix timer. Note this makes this code Unix only!

#include <unistd.h>    // Used to get home directory (Unix only)
#include <sys/types.h> // 
#include <pwd.h>       //

#include <time.h>      //for the time
#include <boost/regex.hpp>

//delay between checks in seconds
#define DELAY 60

//Regex to check for things which shouldn't be passed to a bash shell
boost::regex badInput(".*(>|<|&&|\\|).*");

using namespace std;

//#include "MakeTest.h"
#include "gitTools.h"
#include "dir.h"
#include "database.h"

const char *homedir;

std::string gitRootDir = "";

std::vector<MakeTest> tests;

test_database db;

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
    //Prints the fail string (here as an example for when we wan't to use it properly)
    /*cout << endl << "##############################################################" << 
            endl << "The test you have just run has failed! You better check it out" <<
            endl << "##############################################################" << 
            endl;
    cout << result.getReturnString() << endl << endl;*/
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
        int size = tests.size();
        for(int i=0; i<size; i++) {
            //cout << "Comparing " << tests.at(i).getTestName() << " and " << t.getTestName() << endl;
            if(!tests.at(i).getTestName().compare(t.getTestName())) {
                newTask = false;
                //cout << "They are the same!" << endl;
            }
        }
        if(newTask) {
            tests.push_back(t);
            t.createNewRepo(); //TODO This may not need to be run if the files are already there from before
        }
    }
}

void listTasks()
{
    updateTasksFromFile();
    int size = tests.size();
    for(int i=0; i<size; i++) {
        MakeTest test = tests.at(i);
        TestMode mode = test.getMode();
        if(mode == PASS) {
            cout << "[ PASS ]";
        } else if(mode == FAIL) {
            cout << "[ FAIL ]";
        } else if(mode == UNTESTED) {
            cout << "[      ]";
        } else if(mode == PAUSED) {
            cout << "[PAUSED]";
        } else {
            cout << "[ ERR  ]";
        }
        cout << "  " << test.getTestName() << endl;
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

            time_t rawtime;
            time (&rawtime);

            if(check) {
                if(tests.at(i).performTest(r)) {
                    onFail(r);
                    cout << "[ FAIL ] " << tests.at(i).getTestName() << " failed on " << ctime (&rawtime); //ctime ends with a '\n'
                } else {
                    //Success
                    cout << "[ PASS ] " << tests.at(i).getTestName() << " passed on " << ctime (&rawtime); //ctime ends with a '\n' 
                }
            }
        }
        sleep(DELAY);
    }
    return 0;
}

void printHelp()    
{
    cout    << "Did you mean one of these?"                                         << endl << endl

            << "tiny-ci start             Starts the server program"                << endl
            << "tiny-ci list              Lists all the current tasks"              << endl
            << "tiny-ci add name gitURL   Adds a test to run"                       << endl << endl;
}

void printAddHelp()
{
    cout 
        << "tiny-ci add can be used to add tasks to be run by the tiny-ci program." << endl << endl

        << "The required arguments are:"                                            << endl
        << "   <name>:      The name of the new task"                               << endl
        << "   <gitURL>:    The url for the git repository"                         << endl << endl

        << "The command should then be in the form:"                                << endl
        << "   tiny-ci add <name> <gitURL>"                                         << endl;
}

int main(int argc, char *argv[])
{
    initHomeDir();
    if(!db.isOpen()) {
        return 13;
    }

    if(argc <= 1) {
        cout << "Not enough arguments!" << endl;
        printHelp();
        return 1;
    }

    if(!std::string(argv[1]).compare("start")) {
        start();
    } else if(!std::string(argv[1]).compare("add")) {
        
        if(argc >= 3 && !std::string(argv[2]).compare("help")) {
            printAddHelp();
            return 0;
        } else if(argc < 4) {
            cout << "Not enough arguments to make a new task" << endl;
            printHelp();
            return 1;
        }

        if(boost::regex_match(argv[2], badInput)) {
            cout << "Badly formed test name " << argv[2] << ". Unable to continue" << endl;
            return 10;
        } else if(boost::regex_match(argv[3], badInput)) {
            cout << "Badly formed git url. Unable to continue" << endl;
            return 10;
        }
        
        bool runTest = false;
        if(argc > 4 && !std::string(argv[4]).compare("true")) {
            runTest = true;
        }

        cout << "Name:      " << argv[2] << endl;
        cout << "Directory: " << gitRootDir + "/" + argv[2] << endl;
        cout << "Git URL:   " << argv[3] << endl;
        cout << "Run test:  " << runTest << endl << endl;
        cout << "Git url check: " << boost::regex_match(argv[3], badInput) << endl;
        
        MakeTest t(argv[2], gitRootDir + "/" + argv[2], argv[3], runTest);
        db.addTask(t);
        //t.createNewRepo();
        //addTaskToStore(t);
    } else if(!std::string(argv[1]).compare("list")) {
        db.getTasks(&tests);
        listTasks();
        //db.listTasks(); //TODO this is the only line which should be here
    } else {
        cout << "Incorrect argument!" << endl;
        printHelp();
        return 1;
    }

    return 0;
}


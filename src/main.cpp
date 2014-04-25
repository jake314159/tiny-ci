
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

//Regex to check for things which shouldn't be passed to a bash shell or SQL
boost::regex badInput(".*(>|<|&&|\\||DELETE|INSERT|UPDATE|=|DROP|;).*");

using namespace std;

//#include "MakeTest.h"
#include "gitTools.h"
#include "dir.h"
#include "database.h"
void printHelp();
void printAddHelp();

//#include "MakeTest.h"


const char *homedir;

std::string gitRootDir = "";

std::vector<Test*> tests;

test_database db;

void initHomeDir()
{
    struct passwd *pw = getpwuid(getuid());
    homedir = pw->pw_dir;
    //cout << "Home dir " << homedir << endl;
    gitRootDir = std::string(homedir) + "/" + HOME_DIR;
    //cout << "git root dir " << gitRootDir << endl;
}

int onFail(string testName, Result &result)
{
    // Check if processor is available
    if (!system(NULL)) {
        exit (EXIT_FAILURE);
    }

    //run command
    std::string cmd = std::string("python ") + homedir + "/" + HOME_DIR + "/processError.py '" + testName + "' '" + result.getReturnString() + "'";

    return system((char*)cmd.c_str());
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
    int size = tests.size();
    for(int i=0; i<size; i++) {
        delete tests.at(i);
    }
    tests.clear();
    db.getTasks(&tests);
    
    for(int i=0; i<size; i++) {
        string lastHash = db.getTestRunsHash(std::to_string(tests.at(i)->getID()));
        tests.at(i)->setLastHash( lastHash );
    }
}

void listTasks()
{
    updateTasksFromFile();
    int size = tests.size();
    for(int i=0; i<size; i++) {
        Test *test = tests.at(i);

        //convert the ID to a string
        stringstream ss;
        ss << test->getID();

        TestMode mode = db.getLastTestResult( ss.str() );//test->getMode();
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
        cout << "  " << test->getID() << ") " << test->getTestName() << endl;
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
            int check = tests.at(i)->checkForChange();

            time_t rawtime;
            time (&rawtime);

            if(check) {
                if(tests.at(i)->performTest(r)) {
                    onFail(tests.at(i)->getTestName(), r);
                    cout << "[ FAIL ] " << tests.at(i)->getTestName() << " failed on " << ctime (&rawtime); //ctime ends with a '\n'
                } else {
                    //Success
                    cout << "[ PASS ] " << tests.at(i)->getTestName() << " passed on " << ctime (&rawtime); //ctime ends with a '\n' 
                }
                db.addTestRun(tests.at(i)->getID(), tests.at(i)->getStoredLastHash(), r.getReturnValue());
            }
        }
        sleep(DELAY);
    }
    return 0;
}

int handleAddTask(int argc, char *argv[])
{
    if(argc >= 3 && !std::string(argv[2]).compare("help")) {
        printAddHelp();
        return 0;
    } else if(argc >= 3 && !std::string(argv[2]).compare("make")) {

        if(argc < 5) {
            cout << "Not enough arguments to make a new task" << endl;
            printHelp();
            return 1;
        }

        if(boost::regex_match(argv[3], badInput)) {
            cout << "Badly formed test name " << argv[3] << ". Unable to continue" << endl;
            return 10;
        } else if(boost::regex_match(argv[4], badInput)) {
            cout << "Badly formed git url. Unable to continue" << endl;
            return 10;
        }
        
        bool runTest = false;
        if(argc > 5 && !std::string(argv[5]).compare("true")) {
            runTest = true;
        }

        cout << "Name:      " << argv[3] << endl;
        cout << "Directory: " << gitRootDir + "/" + argv[3] << endl;
        cout << "Git URL:   " << argv[4] << endl;
        cout << "Run test?:  " << (runTest ? "Yes" : "No") << endl << endl;
        //cout << "Git url check: " << boost::regex_match(argv[4], badInput) << endl;
        
        MakeTest t(-1, argv[3], gitRootDir + "/" + argv[3], argv[4], runTest);
        db.addTask(t);
        t.createNewRepo();
    } else if(argc >= 3 && !std::string(argv[2]).compare("maven")) {

        if(argc < 4) {
            cout << "Not enough arguments to make a new task" << endl;
            printHelp();
            return 1;
        }

        if(boost::regex_match(argv[3], badInput)) {
            cout << "Badly formed test name " << argv[3] << ". Unable to continue" << endl;
            return 10;
        } else if(boost::regex_match(argv[4], badInput)) {
            cout << "Badly formed git url. Unable to continue" << endl;
            return 10;
        }
        
        cout << "Name:      " << argv[3] << endl;
        cout << "Directory: " << gitRootDir + "/" + argv[3] << endl;
        cout << "Git URL:   " << argv[4] << endl;
        //cout << "Git url check: " << boost::regex_match(argv[4], badInput) << endl;
        
        MavenTest t(-1, argv[3], gitRootDir + "/" + argv[3], argv[4]);
        db.addTask(t);
        t.createNewRepo();
    } else if(argc >= 3 && !std::string(argv[2]).compare("bash")) {

        if(argc < 5) {
            cout << "Not enough arguments to make a new task" << endl;
            printHelp();
            return 1;
        }

        if(boost::regex_match(argv[3], badInput)) {
            cout << "Badly formed test name " << argv[3] << ". Unable to continue" << endl;
            return 10;
        } else if(boost::regex_match(argv[4], badInput)) {
            cout << "Badly formed bash file name. Unable to continue" << endl;
            return 10;
        } else if(boost::regex_match(argv[5], badInput)) {
            cout << "Badly formed git url. Unable to continue" << endl;
            return 10;
        }
        
        /*bool runTest = false;
        if(argc > 5 && !std::string(argv[5]).compare("true")) {
            runTest = true;
        }*/

        cout << "Name:      " << argv[3] << endl;
        cout << "Directory: " << gitRootDir + "/" + argv[3] << endl;
        cout << "Bash script:  " << argv[4] << endl << endl;
        cout << "Git URL:   " << argv[5] << endl;        
        //cout << "Git url check: " << boost::regex_match(argv[4], badInput) << endl;
        
        BashTest t(-1, argv[3], gitRootDir + "/" + argv[3], string(argv[4]), string(argv[5]));
        db.addTask(t);
        t.createNewRepo();
    } else {
        cout << "I don't know what '" << argv[2] << "' is sorry" << endl;
        printAddHelp();
    }
    return 0;
}

void printHelp()    
{
    cout    << "Did you mean one of these?"                                         << endl << endl

            << "tiny-ci start             Starts the server program"                << endl
            << "tiny-ci list              Lists all the current tasks"              << endl
            << "tiny-ci remove ID         Removes the task with the ID given"       << endl
            << "tiny-ci add name gitURL   Adds a test to run"                       << endl << endl;
}
void printAddHelp()
{
    cout 
        << "tiny-ci add can be used to add tasks to be run by the tiny-ci program." << endl << endl

        << "The required arguments are:"                                                    << endl
        << "   <name>:      The name of the new task"                                       << endl
        << "   <gitURL>:    The url for the git repository"                                 << endl
        << "   <runtest?>:  Should tiny-ci attempt to run any tests? ('true' or 'false')"   << endl
        << "   <bashFile>:  The name of the bash file to run"                               << endl << endl

        << "The command should then be in the form:"                                        << endl
        << "   tiny-ci add make <name> <gitURL>"                                            << endl
        << "   tiny-ci add make <name> <gitURL> <runTest?>"                                 << endl
        << "   tiny-ci add maven <name> <gitURL>"                                           << endl
        << "   tiny-ci add bash <name> <bashFile> <gitURL>"                                 << endl;
}

bool checkArgs(int argc, char *argv[])
{
    bool result = true;
    for(int i=1; i<argc; i++) {
        if(boost::regex_match(argv[i], badInput)) {
            cout << "Badly formed input '" << argv[i] << "'. Unable to continue" << endl;
            result = false;
        }
    }
    return result;
}

int main(int argc, char *argv[])
{
    //Check that all inputs don't have anything banned
    if(!checkArgs(argc, argv)) {
        return 101;
    }

    initHomeDir();
    db.openConnection();
    db.initTable();
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
        
        return handleAddTask(argc, argv);
        //addTaskToStore(t);
    } else if(!std::string(argv[1]).compare("remove")) {
        if(argc<3) {
            cout << "Not enough arguments";
            printHelp();
            return 1;
        } else if(boost::regex_match(argv[2], badInput)) {
            cout << "Badly formed input " << argv[2] << ". Unable to continue" << endl;
            return 13;
        }

        cout << endl << endl << "You are about to delete the task shown below:" << endl;
        db.listTask(argv[2]);
        cout << endl << "Are you sure you want to delete this (y/n)? ";
        string response = "n";
        cin >> response;
        if(!response.compare("y")) {
            db.deleteTask(argv[2]);
            cout << "Task deleted" << endl;
        } else {
            cout << "Delete canceled" << endl;
        }

    } else if(!std::string(argv[1]).compare("list")) {
        if(argc >= 3 && !boost::regex_match(argv[2], badInput)) {
            db.listTestRuns(argv[2]);
            //cout << "The last hash was: "<< db.getTestRunsHash(argv[2]) << endl;
        } else {
            db.getTasks(&tests);
            listTasks();
        }
    } else if(!std::string(argv[1]).compare("dev-fail")) {
        // Unpublished dev option which runs a fake test fail
        // Can be used to test that fails are handled as expected
        Result r;
        r.setReturnValue(1);
        r.setReturnString("An example of a failed test\n\nAn example of a failed test\n");
        onFail("Example-Test", r);
    } else {
        cout << "Incorrect argument!" << endl;
        printHelp();
        return 1;
    }

    return 0;
}


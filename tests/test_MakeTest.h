/*
   --------------------------------
      tiny-ci  
      test_MakeTest.h
   -------------------------------- 

   Copyright 2014 Jacob Causon

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
 
#include <stdio.h>
#include "minunit.h"

static char* test_BASIC_CONSTRUCTOR() {
    MakeTest t("testName", "./testTemp/testDir", "testURL");
    static char errMsg1[] = "Basic constructor name not being set correctly";
    mu_assert(errMsg1, !t.getTestName().compare("testName"));
    return 0;
}

static char* test_PARSE_SAVE_STRING() {
    MakeTest t = MakeTest::parseSaveString("testName;./testTemp/testDir;testURL;0");
    static char errMsg1[] = "String constructor name not being set correctly";
    mu_assert(errMsg1, !t.getTestName().compare("testName"));
    return 0;
}

static char* test_PAUSE() {
    MakeTest t("testName", "./testTemp/testDir", "testURL");
    t.pause();
    static char errMsg1[] = "Test can't be paused";
    mu_assert(errMsg1, t.getMode() == PAUSED);
    return 0;
}

static char* test_UNPAUSE() {
    MakeTest t("testName", "./testTemp/testDir", "testURL");
    t.pause();
    static char errMsg1[] = "Test can't be paused";
    mu_assert(errMsg1, t.getMode() == PAUSED);
    t.unpause();
    static char errMsg2[] = "Test can't be unpaused";
    mu_assert(errMsg2, !(t.getMode() == PAUSED));
    return 0;
}

static char* all_MakeTest_tests() {
    mu_run_test(test_BASIC_CONSTRUCTOR);
    mu_run_test(test_PARSE_SAVE_STRING);
    mu_run_test(test_PAUSE);
    mu_run_test(test_UNPAUSE);

    return 0;
}

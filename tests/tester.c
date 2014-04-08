/*
   --------------------------------
      tiny-ci  
      tester.c
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
 
#include "../src/MakeTest.h"

int tests_run = 0;
int successCount = 0;
int failCount = 0;

#include "test_MakeTest.h"
#include "test_Result.h"

int main(int argc, char **argv) {
    all_MakeTest_tests();
    all_Result_tests();

    printf("Tests run: %d\n", tests_run);

    if(failCount > 0) {
        printf("SOME TESTS FAILED\n");
    } else {
        printf("ALL TESTS PASSED\n");
    }

    return failCount != 0;
}

/*
   --------------------------------
      tiny-ci  
      test_Result.h
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

static char* test_SET_RETURN_VALUE() {
    Result r;
    r.setReturnValue(100);
    static char errMsg1[] = "Return value for Result isn't being set correctly";
    mu_assert(errMsg1, r.getReturnValue()==100);
    return 0;
}

static char* test_SET_RETURN_STRING() {
    Result r;
    string returnString = "A test return string";
    r.setReturnString(returnString);
    static char errMsg1[] = "Return string for Result isn't being set correctly";
    mu_assert(errMsg1, !r.getReturnString().compare(returnString));
    return 0;
}

static char* all_Result_tests() {
    mu_run_test(test_SET_RETURN_VALUE);
    mu_run_test(test_SET_RETURN_STRING);

    return 0;
}

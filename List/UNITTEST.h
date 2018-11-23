//
// Created by max_l on 10.10.2018.
//

#ifndef UNTITLED4_UNITTEST_H
#define UNTITLED4_UNITTEST_H

#define UNITTEST(what, opera, truth)                                                                                   \
{                                                                                                                      \
    if(!((what) opera (truth)))                                                                                        \
        {                                                                                                              \
            printf("FAIL test!!! !(%s %s %s) in %d line : %s == ", #what, #opera, #truth, __LINE__, #what);            \
            std::cout << what << '\n';                                                                                 \
        }                                                                                                              \
    else                                                                                                               \
        printf("Test passed %s %s %s\n", #what, #opera, #truth);                                                                                       \
};

bool Unittest();

#endif //UNTITLED4_UNITTEST_H


#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include "dyn_survey.h"

/* output functions */
void print_output(short tests[], int num_tests, 
    char questions[][MAX_WORD_LEN], int num_questions,
    char likert_level_descriptions[][MAX_WORD_LEN], int num_likert_levels,
    Response responses[], int num_respondents, 
    int frequencies[MAX_OPTIONS][MAX_Q], double scores[][MAX_CATEGORIES],
    float average_scores[]);

#endif

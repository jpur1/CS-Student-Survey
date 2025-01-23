#ifndef _PROCESSING_H_
#define _PROCESSING_H_

#include "dyn_survey.h"

void convert_answers(char answers[MAX_Q][MAX_STUDENTS][MAX_WORD_LEN], int frequencies[MAX_OPTIONS][MAX_Q], int num_students);
int reverse_score(int score);
void compute_scores(char answers[MAX_Q][MAX_STUDENTS][MAX_WORD_LEN], char direct_reverse[MAX_Q][MAX_WORD_LEN], int num_students,
                     int num_q_per_cat[MAX_CATEGORIES], int q_to_cat[MAX_Q], float cat_totals[MAX_STUDENTS][MAX_CATEGORIES],
                     int cat_q_count[MAX_STUDENTS][MAX_CATEGORIES]);
void compute_average(float cat_totals[MAX_STUDENTS][MAX_CATEGORIES], int cat_q_count[MAX_STUDENTS][MAX_CATEGORIES], int num_students);

#endif

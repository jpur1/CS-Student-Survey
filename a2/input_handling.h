#ifndef _INPUT_HANDLING_H_
#define _INPUT_HANDLING_H_

#include "dyn_survey.h"
#include <time.h>

/* Prototypes for input handling */
int tokenize_line(char *line, char phrases[][MAX_WORD_LEN], char *delimiter);
int process_tests(char phrases[][MAX_WORD_LEN], int num_phrases, short tests[]);
int process_questions(char phrases[][MAX_WORD_LEN], int num_phrases, char questions[][MAX_WORD_LEN]);
void process_question_types(char phrases[][MAX_WORD_LEN], int num_phrases, short question_types[]);
int process_likert_level_descriptions(char phrases[][MAX_WORD_LEN], int num_phrases, char likert_level_descriptions[][MAX_WORD_LEN]);
int process_responses(char phrases[][MAX_WORD_LEN], int num_phrases);
void process_response(char phrases[][MAX_WORD_LEN], int num_phrases, short question_types[], Response responses[], int num_respondents);
void process_filter(char phrases[][MAX_WORD_LEN], int num_phrases, Response responses[], int num_responses);
short convert_response_to_scale(char *token, short direct_scale);
short convert_response_to_scale(char *token, short direct_scale);

#endif

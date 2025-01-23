#ifndef _DYN_SURVEY_H_
#define _DYN_SURVEY_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_Q 38
#define MAX_OPTIONS 6
#define MAX_LINE_LEN 3000
#define MAX_WORD_LEN 200
#define MAX_WORDS 200
#define MAX_STUDENTS 1000
#define MAX_CATEGORIES 5

typedef struct respondent {
    char field[MAX_WORD_LEN];
    char origin[MAX_WORD_LEN];
    char date_of_birth[MAX_WORD_LEN];
    int filtered;
} Respondent;

typedef struct response {
    Respondent respondent;
    short answers[MAX_Q];
} Response;

extern char *cat_labels[MAX_CATEGORIES];

#endif

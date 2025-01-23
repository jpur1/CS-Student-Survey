#include "input_handling.h"

/*
 * Purpose: Tokenizes the given string line using a respective delimiter.
 *          Stores each token or word into an array of strings (words).
 * Parameters:
 *  - line: Pointer to the input string.
 *  - words: 2D array of characters where each tokenized word is stored.
 *  - divider: A string that is a delimiter used to split the words for tokenizing.
 * Returns: The number of tokens or words found in the input string.
 */
int tokenize_line(char *line, char words[][MAX_WORD_LEN], char *divider) {
    int num_words = 0;
    char *token = strtok(line, divider);
    while (token) {
        strncpy(words[num_words], token, MAX_WORD_LEN);
        num_words++;
        token = strtok(NULL, divider);
    }
    return num_words;
}

/* store test configuration to only run specified tests */
int process_tests(char phrases[][MAX_WORD_LEN], int num_phrases, short tests[]) {
    /* for every phrase in phrases, convert it to a bit and store it  */
    for (int i = 0; i < num_phrases; i++) {
        if (phrases[i][0] == '1') {
            tests[i] = 1;
        }
        else {
            tests[i] = 0;
        }
    }
    return num_phrases;          
}

/* store survey questions */
int process_questions(char phrases[][MAX_WORD_LEN], int num_phrases, 
    char questions[][MAX_WORD_LEN]) {

    /* for every phrase in phrases, store it as a question  */
    for (int i = 0; i < num_phrases; i++) {
        strncpy(questions[i], phrases[i], MAX_WORD_LEN);
    }
    return num_phrases;          
}

/* store survey question types (direct or reverse) */
void process_question_types(char phrases[][MAX_WORD_LEN], int num_phrases, 
    short question_types[]) {

    /* for every phrase in phrases, convert it to a bit and store it  */
    for (int i = 0; i < num_phrases; i++) {
        if (strcmp(phrases[i], "Direct") == 0) {
            question_types[i] = 1;
        }
        else {
            question_types[i] = 0;
        }
    }   
    return;     
}

/* store possible answers - Likert levels */
int process_likert_level_descriptions(char phrases[][MAX_WORD_LEN], int num_phrases, 
    char likert_level_descriptions[][MAX_WORD_LEN]) {

    /* for every phrase in phrases, store it as a possible Likert answer  */
    for (int i = 0; i < num_phrases; i++) {
        strncpy(likert_level_descriptions[i], phrases[i], MAX_WORD_LEN);
    }
    return num_phrases;          
}

/* store possible answers - Likert levels */
int process_responses(char phrases[][MAX_WORD_LEN], int num_phrases) {
    return atoi(phrases[0]);
}

/* stores next survey response */
void process_response(char phrases[][MAX_WORD_LEN], int num_phrases, 
    short question_types[], 
    Response responses[], int num_respondents) {

    /* Store respondent information (indices 0 to 2) */
    strncpy(responses[num_respondents].respondent.field, phrases[0], MAX_WORD_LEN);
    strncpy(responses[num_respondents].respondent.origin, phrases[1], MAX_WORD_LEN);
    strncpy(responses[num_respondents].respondent.date_of_birth, phrases[2], MAX_WORD_LEN);

    /* for every phrase in phrases, convert it to a scale and store it as a response
       indices start from 0 in question_types and responses
       index starts from 3 in phrases to ignore respondent's information
     */
    for (int i = 3; i < num_phrases; i++) {
        short scale = convert_response_to_scale(phrases[i], question_types[i-3]);
        responses[num_respondents].answers[i-3] = scale;
    }
    return;     
}

/* convert Likert item to numerical scale */
short convert_response_to_scale(char *token, short direct_scale) {
    short scale;
    if (strcmp(token, "fully disagree") == 0) {
        scale = 1;
    }
    else if (strcmp(token, "disagree") == 0) {
        scale = 2;
    }
    else if (strcmp(token, "partially disagree") == 0) {
        scale = 3;
    }
    else if (strcmp(token, "partially agree") == 0) {
        scale = 4;
    }
    else if (strcmp(token, "agree") == 0) {
        scale = 5;
    }
    else {
        scale = 6;
    }

    return scale;
}

/* store survey question types (direct or reverse) */
void process_filter(char phrases[][MAX_WORD_LEN], int num_phrases, 
    Response responses[], int num_responses) {

    int filter_index = atoi(phrases[0]);

    if (filter_index == 0) {
        for (int i = 0; i < num_responses; i++) {
            if (strcmp(responses[i].respondent.field, phrases[1]) != 0) {
                responses[i].respondent.filtered = 0;
            }
        }
    }
    else if (filter_index == 1) {
        for (int i = 0; i < num_responses; i++) {
            if (strcmp(responses[i].respondent.origin, phrases[1]) != 0) {
                responses[i].respondent.filtered = 0;
            }
        }
    }
    else if (filter_index == 2) {
        int lower_limit = atoi(phrases[1]);
        int upper_limit = atoi(phrases[2]);
        time_t now = time(NULL);
        struct tm other_date;

        /* start bug fix */
        strptime("2023-01-01", "%F", &other_date);
        difftime(now, mktime(&other_date));
        /* end bug fix */

        for (int i = 0; i < num_responses; i++) {
            // printf("%s\n", responses[i].respondent.date_of_birth);
            strptime(responses[i].respondent.date_of_birth, "%F", &other_date);
            time_t other = mktime(&other_date);
            double dt =  difftime(now, other) / 31536000.0;
            int age = (int)dt;
            // printf("%d years old\n", age);
            if ((age < lower_limit) || (age > upper_limit)) {
                responses[i].respondent.filtered = 0;
            }
        }

    }
    return;     
}
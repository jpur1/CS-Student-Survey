#include "output.h"

/*
 * Purpose: Prints the formatted survey statistics to stdout.
 * Parameters:
 *  - tests: Indicates which tests (output types) are enabled.
 *  - questions: Array of survey questions.
 *  - num_questions: Number of survey questions.
 *  - likert_level_descriptions: Array of Likert scale descriptions.
 *  - num_likert_levels: Number of Likert levels.
 *  - responses: Array of all survey responses.
 *  - num_responses: Total number of responses.
 *  - frequencies: Relative frequencies of Likert responses.
 *  - scores: Scores for each respondent in each category.
 *  - average_scores: Average scores for all respondents per category.
 * Returns: Nothing (prints to stdout).
 */
void print_output(short tests[], int num_tests,
    char questions[][MAX_WORD_LEN], int num_questions,
    char likert_level_descriptions[][MAX_WORD_LEN], int num_likert_levels,
    Response responses[], int num_responses,
    int frequencies[MAX_OPTIONS][MAX_Q], double scores[][MAX_CATEGORIES],
    float average_scores[]) {

    int num_respondents = 0;

    /* Count filtered respondents */
    for (int i = 0; i < num_responses; i++) {
        if (responses[i].respondent.filtered) {
            num_respondents++;
        }
    }

    /* Print header */
    printf("Examining Science and Engineering Students' Attitudes Towards Computer Science\n");
    printf("SURVEY RESPONSE STATISTICS\n");
    printf("\nNUMBER OF RESPONDENTS: %d\n", num_respondents);

    /* Test 1: Print relative frequencies */
    if (tests[0]) {
        printf("\nFOR EACH QUESTION BELOW, RELATIVE PERCENTUAL FREQUENCIES ARE COMPUTED FOR EACH LEVEL OF AGREEMENT\n");
        for (int i = 0; i < num_questions; i++) {
            printf("\n%s\n", questions[i]);
            for (int j = 0; j < num_likert_levels; j++) {
                printf("%d: %s\n", frequencies[j][i], likert_level_descriptions[j]);  // Changed format for int
            }
        }
    }

    /* Test 2: Print individual respondent scores */
    if (tests[1]) {
        printf("\nSCORES FOR ALL THE RESPONDENTS\n\n");
        for (int i = 0; i < num_responses; i++) {
            if (responses[i].respondent.filtered) {
                printf("C:%.2f, I:%.2f, G:%.2f, U:%.2f, P:%.2f\n",
                       scores[i][0], scores[i][1], scores[i][2], scores[i][3], scores[i][4]);
            }
        }
    }

    /* Test 3: Print average scores across categories */
    if (tests[2]) {
        printf("\nAVERAGE SCORES PER RESPONDENT\n\n");
        printf("C:%.2f, I:%.2f, G:%.2f, U:%.2f, P:%.2f\n",
               average_scores[0], average_scores[1], average_scores[2], average_scores[3], average_scores[4]);
    }
}

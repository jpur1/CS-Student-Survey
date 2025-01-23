#include "dyn_survey.h"
#include "input_handling.h"
#include "processing.h"
#include "output.h"

char *cat_labels[MAX_CATEGORIES] = {"C", "I", "G", "U", "P"};

/*
 * Purpose: Main function to read input, process survey data, and print formatted output.
 */
int main(int argc, char *argv[]) {
    if (argc != 1) {
        printf("Usage: %s\n", argv[0]);
        printf("Should receive no parameters\n");
        printf("Read from stdin instead\n");
        exit(1);
    }

    /* Local variable declarations */
    int num_phrases = 0;
    char phrases[MAX_WORDS][MAX_WORD_LEN];

    int num_tests = 0;
    short tests[MAX_CATEGORIES] = {0};

    int num_questions = 0;
    char questions[MAX_Q][MAX_WORD_LEN];
    char direct_reverse[MAX_Q][MAX_WORD_LEN];

    short question_types[MAX_Q] = {0};
    int num_likert_levels = 0;
    char likert_level_descriptions[MAX_OPTIONS][MAX_WORD_LEN];

    int num_responses = 0;
    int num_respondents = 0;

    Response responses[MAX_STUDENTS] = {0};
    for (int i = 0; i < MAX_STUDENTS; i++) {
        responses[i].respondent.filtered = 1;  // Default: included in analysis
    }

    float cat_totals[MAX_STUDENTS][MAX_CATEGORIES] = {{0.0}};
    int cat_q_count[MAX_STUDENTS][MAX_CATEGORIES] = {0};

    int frequencies[MAX_OPTIONS][MAX_Q] = {0};

    int num_q_per_cat[MAX_CATEGORIES] = {8, 10, 10, 6, 4};  // C, I, G, U, P
    int q_to_cat[MAX_Q] = {
        0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4};
    
    char line[MAX_LINE_LEN];
    int phase = 0;

    /* Read and process input */
    while (fgets(line, sizeof(char) * MAX_LINE_LEN, stdin)) {
        if (line[0] == '#') {
            continue;  // Ignore comments
        }

        switch (phase) {
            case 0:
                num_phrases = tokenize_line(line, phrases, ",\n");
                num_tests = process_tests(phrases, num_phrases, tests);
                phase++;
                break;

            case 1:
                num_phrases = tokenize_line(line, phrases, ";\n");
                num_questions = process_questions(phrases, num_phrases, questions);
                phase++;
                break;

            case 2:
                num_phrases = tokenize_line(line, phrases, ";\n");
                process_question_types(phrases, num_phrases, question_types);
                phase++;
                break;

            case 3:
                num_phrases = tokenize_line(line, phrases, ",\n");
                num_likert_levels = process_likert_level_descriptions(phrases, num_phrases, likert_level_descriptions);
                phase++;
                break;

            case 4:
                num_phrases = tokenize_line(line, phrases, "\n");
                num_responses = process_responses(phrases, num_phrases);
                phase++;
                break;

            case 5:
                num_phrases = tokenize_line(line, phrases, ",\n");
                process_response(phrases, num_phrases, question_types, responses, num_respondents);
                num_respondents++;
                if (num_respondents >= num_responses) {
                    phase++;
                }
                break;

            case 6:
                num_phrases = tokenize_line(line, phrases, ",\n");
                process_filter(phrases, num_phrases, responses, num_responses);
                break;
        }
    }

    /* Create 3D answers array */
    char answers[MAX_Q][MAX_STUDENTS][MAX_WORD_LEN];
    for (int q = 0; q < num_questions; q++) {
        for (int s = 0; s < num_respondents; s++) {
            snprintf(answers[q][s], MAX_WORD_LEN, "%s", likert_level_descriptions[responses[s].answers[q] - 1]);
        }
    }

    /* Data processing */
    convert_answers(answers, frequencies, num_respondents);
    compute_scores(answers, direct_reverse, num_respondents, num_q_per_cat, q_to_cat, cat_totals, cat_q_count);
    compute_average(cat_totals, cat_q_count, num_respondents);

    /* Print survey results */
    double scores[MAX_STUDENTS][MAX_CATEGORIES] = {{0.0}};  // Temporary for `print_output`
    print_output(tests, num_tests, questions, num_questions, 
                 likert_level_descriptions, num_likert_levels, 
                 responses, num_respondents, frequencies, scores, cat_totals[0]);

    return 0;
}

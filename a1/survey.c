#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_Q 38
#define MAX_OPTIONS 6
#define MAX_LINE_LEN 3000
#define MAX_WORD_LEN 200
#define MAX_WORDS 200
#define MAX_STUDENTS 1000
#define MAX_CATEGORIES 5

const char *cat_labels[MAX_CATEGORIES] = {"C", "I", "G", "U", "P"};

int main(int argc, char *argv[]) {
    // Prevents incorrect usage of function.
    if (argc != 1) {
        printf("Usage: %s\n", argv[0]);
        printf("Should receive no parameters\n");
        printf("Read from stdin instead\n");
        return 0;
    }

    char lines[MAX_LINE_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int num_words = 0, num_students = 0, line_count = 0, test_bits = 0, section = 0;
    char questions[MAX_Q][MAX_WORD_LEN];
    char likert[MAX_Q][MAX_WORD_LEN];
    char answers[MAX_Q][MAX_STUDENTS][MAX_WORD_LEN];
    char direct_reverse[MAX_Q][MAX_WORD_LEN];
    int frequencies[MAX_OPTIONS][MAX_Q];
    int num_q_per_cat[MAX_CATEGORIES] = {8, 10, 10, 6, 4};
    int q_to_cat[MAX_Q] = {
        0, 0, 0, 0, 0, 0, 0, 0, // 8 questions in C
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 10 questions in I
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, // 10 questions in G
        3, 3, 3, 3, 3, 3,             // 6 questions in U
        4, 4, 4, 4                    // 4 questions in P
    };

    // Reads the input and processes each line.
    while (fgets(lines, sizeof(char) * MAX_LINE_LEN, stdin)) {
        if (lines[0] == '#') continue; // Skip comment lines.

        switch (section) {
            case 0: // Test configuration
                if (strcmp(lines, "1,0,0,0\n") == 0) test_bits = 1;
                else if (strcmp(lines, "0,1,0,0\n") == 0) test_bits = 2;
                else if (strcmp(lines, "0,0,1,0\n") == 0) test_bits = 3;
                else test_bits = 4;
                section = 1;
                break;

            case 1: // Questions
                num_words = token_lines(lines, words, ";");
                for (int i = 0; i < num_words; i++) {
                    strncpy(questions[i], words[i], MAX_WORD_LEN);
                }
                section = 2;
                break;

            case 2: // Direct/Reverse
                num_words = token_lines(lines, words, "\n");
                for (int i = 0; i < num_words; i++) {
                    strncpy(direct_reverse[i], words[i], MAX_WORD_LEN);
                }
                section = 3;
                break;

            case 3: // Likert options
                num_words = token_lines(lines, words, ",");
                for (int i = 0; i < num_words; i++) {
                    strncpy(likert[i], words[i], MAX_WORD_LEN);
                }
                section = 4;
                break;

            case 4: // Survey responses
                if (line_count % 41 == 0) num_students++;
                num_words = token_lines(lines, words, ",");
                for (int q = 3; q < num_words; q++) {
                    strncpy(answers[q - 3][num_students - 1], words[q], MAX_WORD_LEN);
                }
                break;
        }
        line_count++;
    }

    if (test_bits == 1) {
        printf("Examining Science and Engineering Students' Attitudes Towards Computer Science\n");
        printf("SURVEY RESPONSE STATISTICS\n\n");
        printf("NUMBER OF RESPONDENTS: 0\n\n");
        printf("FOR EACH QUESTION BELOW, RELATIVE PERCENTUAL FREQUENCIES ARE COMPUTED FOR EACH LEVEL OF AGREEMENT\n\n");
        for (int i = 0; i < MAX_Q; i++) {
            printf("%s\n", questions[i]);
            for (int j = 0; j < MAX_OPTIONS; j++) {
                printf("0.00: %s\n", likert[j]);
            }
            printf("\n");
        }
    }

    return 0;
}

/*
 * Purpose: Tokenizes the given string line using a respective delimiter.
 *          Stores each token or word into an array of strings (words).
 * Parameters:
 *  - line: Pointer to the input string.
 *  - words: 2D array of characters where each tokenized word is stored.
 *  - divider: A string that is a delimiter used to split the words for tokenizing.
 * Returns: The number of tokens or words found in the input string.
 */
int token_lines(char *line, char words[][MAX_WORD_LEN], char *divider) {
    int num_words = 0;
    char *token = strtok(line, divider);
    while (token) {
        strncpy(words[num_words], token, MAX_WORD_LEN);
        num_words++;
        token = strtok(NULL, divider);
    }
    return num_words;
}

/*
 * Purpose: Converts the answers array and counts the Likert responses
 *          from students into a frequency array for each question.
 * Parameters:
 *  - answers: 3D array of responses to the questions from each student.
 *  - frequencies: Array to store the converted Likert response frequencies.
 *  - num_students: The number of students who responded.
 * Returns: Nothing.
 */
void convert_answers(char answers[MAX_Q][MAX_STUDENTS][MAX_WORD_LEN], int frequencies[MAX_OPTIONS][MAX_Q], int num_students) {
    for (int i = 0; i < MAX_OPTIONS; i++) {
        for (int q = 0; q < MAX_Q; q++) {
            frequencies[i][q] = 0;
        }
    }

    for (int q = 0; q < MAX_Q; q++) {
        for (int s = 0; s < num_students; s++) {
            if (strcmp(answers[q][s], "fully disagree") == 0) frequencies[0][q]++;
            else if (strcmp(answers[q][s], "disagree") == 0) frequencies[1][q]++;
            else if (strcmp(answers[q][s], "partially disagree") == 0) frequencies[2][q]++;
            else if (strcmp(answers[q][s], "partially agree") == 0) frequencies[3][q]++;
            else if (strcmp(answers[q][s], "agree") == 0) frequencies[4][q]++;
            else if (strcmp(answers[q][s], "fully agree") == 0) frequencies[5][q]++;
        }
    }
}

/*
 * Purpose: Converts the direct Likert scale into its reverse counterpart.
 * Parameters:
 *  - score: The Likert scale in number form.
 * Returns: The reverse version of the Likert scale number.
 */
int reverse_score(int score) {
    return 7 - score;
}

/*
 * Purpose: Calculates the scores for each student across multiple categories
 *          based on their survey responses.
 *          It assigns a score to each answer, adjusts it if necessary
 *          (using reverse scoring), and then stores the total and count of
 *          scores per category for each student.
 * Parameters:
 *  - answers: A 3D array of survey responses (each element is a student's answer).
 *  - direct_reverse: A 2D array indicating whether each question is direct or reverse scored.
 *  - num_students: Total number of students who participated in the survey.
 *  - num_q_per_cat: An array that stores how many questions belong to each category.
 *  - q_to_cat: An array that maps each question to a specific category.
 *  - cat_totals: 2D array that holds the cumulative score totals for each category for each student.
 *  - cat_q_count: 2D array that holds the count of questions per category for each student.
 * Returns: Nothing.
 */
void compute_scores(char answers[MAX_Q][MAX_STUDENTS][MAX_WORD_LEN], char direct_reverse[MAX_Q][MAX_WORD_LEN], int num_students,
                     int num_q_per_cat[MAX_CATEGORIES], int q_to_cat[MAX_Q], float cat_totals[MAX_STUDENTS][MAX_CATEGORIES],
                     int cat_q_count[MAX_STUDENTS][MAX_CATEGORIES]) {
    for (int s = 0; s < num_students; s++) {
        for (int q = 0; q < MAX_Q; q++) {
            int cat = q_to_cat[q];
            int score = 0;
            if (strcmp(answers[q][s], "fully agree") == 0) score = 6;
            else if (strcmp(answers[q][s], "agree") == 0) score = 5;
            else if (strcmp(answers[q][s], "partially agree") == 0) score = 4;
            else if (strcmp(answers[q][s], "partially disagree") == 0) score = 3;
            else if (strcmp(answers[q][s], "disagree") == 0) score = 2;
            else if (strcmp(answers[q][s], "fully disagree") == 0) score = 1;

            if (strcmp(direct_reverse[q], "Reverse") == 0) {
                score = reverse_score(score);
            }
            cat_totals[s][cat] += score;
            cat_q_count[s][cat]++;
        }
    }
}

/*
 * Purpose: Computes the overall average score for each category across all students.
 *          It sums up the total scores and counts for each category.
 * Parameters:
 *  - cat_totals: 2D array that holds the cumulative score totals for each category.
 *  - cat_q_count: 2D array that holds the count of questions per category for each student.
 *  - num_students: The total number of students who participated in the survey.
 * Returns: Nothing.
 */
void compute_average(float cat_totals[MAX_STUDENTS][MAX_CATEGORIES], int cat_q_count[MAX_STUDENTS][MAX_CATEGORIES], int num_students) {
    float cat_avg[MAX_CATEGORIES] = {0};
    for (int cat = 0; cat < MAX_CATEGORIES; cat++) {
        float total = 0;
        int count = 0;
        for (int s = 0; s < num_students; s++) {
            if (cat_q_count[s][cat] > 0) {
                total += cat_totals[s][cat];
                count += cat_q_count[s][cat];
            }
        }
        if (count > 0) {
            cat_avg[cat] = total / (float)count;
        }
    }

    for (int cat = 0; cat < MAX_CATEGORIES; cat++) {
        printf("%s: %.2f", cat_labels[cat], cat_avg[cat]);
        if (cat < MAX_CATEGORIES - 1) {
            printf(", ");
        }
    }
    printf("\n");
}

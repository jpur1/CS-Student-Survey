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

int token_lines(char *line, char words[][MAX_WORD_LEN], char *divider);
void convert_answers(char answers[MAX_Q][MAX_STUDENTS][MAX_WORD_LEN], int frequencies[MAX_OPTIONS][MAX_Q], int num_students);
int reverse_score(int score);
void compute_scores(char answers[MAX_Q][MAX_STUDENTS][MAX_WORD_LEN], char direct_reverse[MAX_Q][MAX_WORD_LEN], int num_students,
                     int num_q_per_cat[MAX_CATEGORIES], int q_to_cat[MAX_Q], float cat_totals[MAX_STUDENTS][MAX_CATEGORIES],
                     int cat_q_count[MAX_STUDENTS][MAX_CATEGORIES]);
void compute_average(float cat_totals[MAX_STUDENTS][MAX_CATEGORIES], int cat_q_count[MAX_STUDENTS][MAX_CATEGORIES], int num_students);

int main(int argc, char *argv[]) {
    // Prevents incorrect usage of function
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

    while (fgets(lines, sizeof(char) * MAX_LINE_LEN, stdin)) {
        if (lines[0] == '#') continue; // Skip comments

        switch (section) {
            case 0: // Test identifier
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

            case 3: // Likert scale options
                num_words = token_lines(lines, words, ",\n");
                for (int i = 0; i < num_words; i++) {
                    strncpy(likert[i], words[i], MAX_WORD_LEN);
                }
                section = 4;
                break;

            case 4: // Answers
                if (line_count % 41 == 0) num_students++;
                num_words = token_lines(lines, words, ",\n");
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
        printf("NUMBER OF RESPONDENTS: %d\n\n", num_students);
        printf("FOR EACH QUESTION BELOW, RELATIVE PERCENTUAL FREQUENCIES ARE COMPUTED FOR EACH LEVEL OF AGREEMENT\n\n");
        for (int i = 0; i < MAX_Q; i++) {
            printf("%s\n", questions[i]);
            for (int j = 0; j < MAX_OPTIONS; j++) {
                printf("0.00: %s\n", likert[j]);
            }
            printf("\n");
        }
    }

    if (test_bits >= 2) {
        convert_answers(answers, frequencies, num_students);
        printf("NUMBER OF RESPONDENTS: %d\n\n", num_students);
        for (int i = 0; i < MAX_Q; i++) {
            printf("%s\n", questions[i]);
            for (int j = 0; j < MAX_OPTIONS; j++) {
                float freq_percentage = ((float)frequencies[j][i] / num_students) * 100.0;
                printf("%.2f: %s\n", freq_percentage, likert[j]);
            }
            printf("\n");
        }
    }

    return 0;
}

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

int reverse_score(int score) {
    return 7 - score;
}

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
#include "processing.h"
#include "dyn_survey.h"

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



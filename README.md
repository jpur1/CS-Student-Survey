# CS-Student-Survey
-**Using Static & Dynamic Memory**-

## Overview

The Department of Computer Science (CSC) at the University of Victoria is conducting a pilot study to understand students’ attitudes towards Computer Science. To assist with this, SENG 265 students were tasked with developing a software system to manage and analyze survey data.

This system leverages a questionnaire adapted from the study *Examining Science and Engineering Students’ Attitudes Towards Computer Science* by Hoegh and Moskal (2009). The survey gathers opinions across five categories—Confidence, Interest, Gender, Usefulness, and Professionalism—using a Likert scale with six levels of agreement.

## Survey Structure

- **Demographics**: Students provide information about their field of study (Engineering, Health, Arts), whether they were born in Canada (yes/no), and their date of birth.
- **Questions**: The survey contains 38 questions, distributed across the following categories:
  - **Confidence (C)**: 8 items
  - **Interest (I)**: 10 items
  - **Gender (G)**: 10 items
  - **Usefulness (U)**: 6 items
  - **Professionalism (P)**: 4 items
- **Likert Scale Responses**: Fully Disagree, Disagree, Partially Disagree, Partially Agree, Agree, Fully Agree.

### Scoring

Scores for each category are calculated by averaging the responses to its questions, with adjustments based on whether the question is phrased as *Direct* or *Reverse*:
- Direct questions map responses from 1 (Fully Disagree) to 6 (Fully Agree).
- Reverse questions map responses in the opposite direction (6 to 1).

Example formulas:
- **Confidence**: \( C = \frac{\text{Sum of responses to Confidence questions}}{8} \)
- Similar formulas apply for Interest, Gender, Usefulness, and Professionalism.

## Program Functionality

The program performs the following tasks:
1. **Input Parsing**:
   - Reads survey questions, Likert scale options, and participant responses from stdin.
   - Ignores lines starting with `#` (comments).
2. **Data Storage**:
   - Dynamically stores questions, Likert scale options, and responses.
3. **Statistics Computation**:
   - Computes the relative frequency (percentage) of agreement levels for each question.
   - Calculates category scores and average scores per respondent.
4. **Output**:
   - Displays results to stdout for easy redirection or comparison.

### Sample Execution

To run the program:
```bash
% cat tests/in01.txt | ./survey

/*
 * SEAS - Smart Examination Anti-Cheat System
 * Author: Momo Maurel Dorian Patsa
 * Country: Cameroon
 * Purpose: Grading MCQ exams and flag suspiciouscanswer patterns pairs
            for human investigation. My program does NOT accuse students
            of examination malpractice!
    
 "Integrity is doing the right thing, even when no one is watching."
                                                       ~ C.S. Lewis
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef _WIN32
    #include <windows.h> // for colours on windows
    //changes line color on windows console (Newly learnt)
    void setColor(int color){
	    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
#else
    void setColor(int color){
        printf("\033[%dm", color == 7 ? 0 : (color == 10 ? 32 : color == 14 ? 33 : 36));
    }
#endif
#define MAX_NAME 30


// Checks whether two students are seated close
int areSeatsClose(int r1, int c1, int r2, int c2);
// Function to print seating plan in table frm
void printSeatingGrid(int students, char names[][MAX_NAME], int row[], int col[]);

int main() {
    int i, j, k, m, n, students, questions, matchCount;
    float similarity, suspicionScore;
	
    printf("Enter number of students: ");
    scanf("%d", &students);

    printf("Enter number of questions: ");
    scanf("%d", &questions);

    if (students <= 0 || questions <= 0) {
        printf("Invalid number of students or questions.\n");
        return 1;
    }

    /* Use dynamic allocation because MSVC doesn't support VLA */
    int *mrk = (int*)malloc(students * sizeof(int));
    if (!mrk) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    for(n=0; n<students; n++){
        mrk[n] = 0;
    }

    char (*names)[MAX_NAME] = malloc(students * MAX_NAME * sizeof(char));
    if (!names) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    char *answers = malloc(students * questions * sizeof(char)); /* flat 2D: answers[i*questions + j] */
    if (!answers) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    int *row = malloc(students * sizeof(int));
    if (!row) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    int *col = malloc(students * sizeof(int));
    if (!col) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    char *markingguide = malloc(questions * sizeof(char));
    if (!markingguide) {
        printf("Memory allocation failed.\n");
        return 1;
    }

	//marking guide
	
	printf("Input marking guide: \n");
    for(m = 0; m < questions; m++){
		printf("Answer %d:", m+1);
        do {
            scanf(" %c", &markingguide[m]);
            markingguide[m] = toupper(markingguide[m]);
        } while(markingguide[m] < 'A' || markingguide[m] > 'E');
	}
	
    /* Input student details */
    for(i = 0; i < students; i++) {
        printf("\nEnter student %ds name: ", i + 1);
        scanf("%29s", names[i]);

        int valid;
        do {
            valid = 1;
            printf("Seat row for %s: ", names[i]);
            scanf("%d", &row[i]);

            printf("Seat column for %s: ", names[i]);
            scanf("%d", &col[i]);

            for(j = 0; j < i; j++) {
                if(row[i] == row[j] && col[i] == col[j]) {
                    printf("Error: Seat already occupied by %s. Please re-enter.\n", names[j]);
                    valid = 0;
                }
            }
        } while(!valid);
		//input answers
        printf("Enter answers for %s:\n", names[i]);
        for(j = 0; j < questions; j++) {
            printf("Answer %d:", j+1);
            do {
                scanf(" %c", &answers[i*questions + j]);
                answers[i*questions + j] = toupper(answers[i*questions + j]);
            } while(answers[i*questions + j] < 'A' || answers[i*questions + j] > 'E');
        }
        //marking
        for(n = 0; n < questions; n++){
            if (markingguide[n] == answers[i*questions + n]){
                mrk[i] = mrk[i] + 1;
            }
        }
    }
    printSeatingGrid(students, names, row, col);
	
	//Results
	printf("The Results are: \n");
        for(i = 0; i < students; i++){
            setColor(10); printf("%s: %d/%d \n", names[i], mrk[i], questions);  setColor(7);
        }
	
    /* Analysis */
    setColor(14); printf("\nCHEATING DETECTION REPORT\n");  setColor(7);

    for(i = 0; i < students; i++) {
        for(j = i + 1; j < students; j++) {
            matchCount = 0;

            for(k = 0; k < questions; k++) {
                if(answers[i*questions + k] == answers[j*questions + k]) {
                    matchCount++;
                }
            }

            similarity = (float)matchCount / questions;
            suspicionScore = similarity * 100;

            int close = areSeatsClose(row[i], col[i], row[j], col[j]);
            if(close) {
                suspicionScore += 15;  /* seat proximity boost */
            }

            if(suspicionScore > 100)
                suspicionScore = 100;

            printf("\nChecking %s and %s:\n", names[i], names[j]);
            printf("Answer Similarity: %.0f%%\n", similarity * 100);
            printf("Seat Distance: %s\n", close ? "Close" : "Far");
            printf("Suspicion Score: %.0f / 100\n", suspicionScore);

            if(suspicionScore >= 70 && close == 1) {
                setColor(12); printf("Status: SUSPICIOUS!!! Investigation needed\n"); setColor(7);
            } else {
                printf("Status: Nothing suspicious\n");
            }
        }
    }

     setColor(11); printf("\nAnalysis Terminated...\n");  setColor(7);
    /* free allocated memory */
    free(mrk);
    free(names);
    free(answers);
    free(row);
    free(col);
    free(markingguide);

    return 0;
}

int areSeatsClose(int r1, int c1, int r2, int c2) {
    if(abs(r1 - r2) <= 1 && abs(c1 - c2) <= 1)
        return 1;   /* close seats */
    else
        return 0;   /* far seats */
}

void printSeatingGrid(int students, char names[][MAX_NAME], int row[], int col[]) {
    int i, r, c;
    
    // Find max row and column to determine grid size
    int maxRow = 0, maxCol = 0;
    for(i = 0; i < students; i++) {
        if(row[i] > maxRow) maxRow = row[i];
        if(col[i] > maxCol) maxCol = col[i];
    }

    printf("\nSEATING PLAN:\n\n");

    // Print column headers
    printf("        ");
    for(c = 1; c <= maxCol; c++) {
        printf("Col %d     ", c);
    }
    printf("\n");

    // Print each row
    for(r = 1; r <= maxRow; r++) {
    	if (r < 10){
    		printf("Row %d  ", r);
		} else {
			printf("Row %d ", r);
		}	

        for(c = 1; c <= maxCol; c++) {
            // Check which student is in this seat
            int found = 0;
            for(i = 0; i < students; i++) {
                if(row[i] == r && col[i] == c) {
                    printf("[%-5s]   ", names[i]);
                    found = 1;
                    break;
                }
            }
            if(!found) {
                printf("[     ]   "); // empty seat
            }
        }
        printf("\n");
    }
    printf("\n");
}

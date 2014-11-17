/** CS 2505 Summer I 2013
 *  Supplied framework for Intersecting Rectangles project.  Your task is to
 *  complete the supplied code to satisfy the posted specification for this
 *  assignment.  The occurrences of "???" in the code below indicate places
 *  where you must complete the implementation.
 *
 *  Student:   <ENTER YOUR NAME HERE>
 *  PID:       <ENTER YOUR VT EMAIL PID HERE>
 */
#include <stdio.h>      // for I/O functions
#include <stdlib.h>     // generally useful
#include <stdbool.h>    // for the bool type
#include <stdint.h>     // for exact-width integer types
#include <inttypes.h>   // for exact-width integer output

#define MAXLEN 100
#define TEST_DATA "TestData.txt"
#define RESULTS "Results.txt"

// Declaration for function Overlap():
bool Overlap(int32_t aSWx, int32_t aSWy, uint32_t aHeight, uint32_t aWidth,
             int32_t bSWx, int32_t bSWy, uint32_t bHeight, uint32_t bWidth);
static bool Intersect(int32_t x, int32_t y_start, int32_t y_end, int32_t y, int32_t x_start, int32_t x_end);
 
int main( ) {

   FILE *Data = fopen(TEST_DATA, "r");         // 1: Open the test data file
   FILE *Log  = fopen(RESULTS, "w");           // 2: Open the results log file
   
   if (Data == NULL) {
      printf("Cannot open TestData.txt file");
   } else if (Log == NULL) {
      printf("Cannot open Results.txt file");
   }
   
   int32_t aSWx, aSWy, aHeight, aWidth, // specification of 1st rectangle
           bSWx, bSWy, bHeight, bWidth; // specification of 2nd rectangle

   char Line[MAXLEN + 1];
   fgets(Line, MAXLEN, Data);      // Read header lines
   fgets(Line, MAXLEN, Data);

   fprintf(Log, "Rectangle A\t\t\tRectangle B\n"); // 3: Write data header lines to the log file
   fprintf(Log, "SW corner\tHeight\tWidth\tSW corner\tHeight\tWidth\n");
   fprintf(Log, "-------------------------------------------------------------\n");

   // 4: Read the data for first pair of rectangles:
   int32_t numRead = fscanf(Data,"%d%d%d%d%d%d%d%d", &aSWx, &aSWy, &aHeight, &aWidth, &bSWx, &bSWy, &bHeight, &bWidth);

   while (numRead != EOF) { // 5: Check if all values were read

      // 6: Write data for current rectangles to log:
      fprintf(Log, "(%5d, %5d)\t%6d\t%5d\t(%5d, %5d)\t%6d\t%5d\n", aSWx, aSWy, aHeight, aWidth, bSWx, bSWy, bHeight, bWidth);

      // 7: Test for an overlap and log the results:
      if (Overlap(aSWx, aSWy, aHeight, aWidth, bSWx, bSWy, bHeight, bWidth)) {
         fprintf(Log, "     overlap detected\n");
      } 
      else {
         fprintf(Log, "     no overlap detected\n");
      }

      // 8: Read data for next pair of rectangles:
      numRead = fscanf(Data,"%d%d%d%d%d%d%d%d", &aSWx, &aSWy, &aHeight, &aWidth, &bSWx, &bSWy, &bHeight, &bWidth);

   }

   // 9: Write the marker for the end of the table:
   fprintf(Log, "-------------------------------------------------------------");

   fclose(Data);    // 10: Close the input and output files
   fclose(Log);
   return 0;
}

/**  Determines whether two rectangles, A and B, intersect.
 *
 *   Pre:
 *         aSWx and aSWy specify the SW (lower, left) corner of A
 *         aHeight specifies the vertical dimension of A
 *         aWidth specifies the horizontal dimension of A
 *         bSWx and bSWy specify the SW (lower, left) corner of B
 *         bHeight specifies the vertical dimension of B
 *         bWidth specifies the horizontal dimension of B
 *       
 *   Returns:
 *         true if A and B share at least one point; false otherwise
 */
bool Overlap(int32_t aSWx, int32_t aSWy, uint32_t aHeight, uint32_t aWidth,
             int32_t bSWx, int32_t bSWy, uint32_t bHeight, uint32_t bWidth) {

   int32_t ax1 = aSWx;		// South west corner (Of the Wayne mansion)
   int32_t ay1 = aSWy;
   int32_t ax2 = aSWx + aWidth;	// South east corner
   int32_t ay2 = aSWy;
   int32_t ax3 = aSWx;		// North west corner
   int32_t ay3 = aSWy + aHeight;
   int32_t ax4 = aSWx + aWidth;	// North east corner
   int32_t ay4 = aSWy + aHeight;
   int32_t a[4][3] = {{ax1, ay1, ay3}, {ax2, ay2, ay4}, {ay1, ax1, ax2}, {ay3, ax3, ax4}};
   int32_t a2[8] = {ax1, ay1, ax2, ay2, ax3, ay3, ax4, ay4};

   int32_t bx1 = bSWx;		// South west corner
   int32_t by1 = bSWy;
   int32_t bx2 = bSWx + bWidth;	// South east corner
   int32_t by2 = bSWy;
   int32_t bx3 = bSWx;		// North west corner
   int32_t by3 = bSWy + bHeight;
   int32_t bx4 = bSWx + bWidth;	// North east corner
   int32_t by4 = bSWy + bHeight;
   int32_t b[4][3] = {{bx1, by1, by3}, {bx2, by2, by4}, {by1, bx1, bx2}, {by3, bx3, bx4}};
   int32_t b2[8] = {bx1, by1, bx2, by2, bx3, by3, bx4, by4};

   // Determine if any vertical lines from rectangle A
   // intersect with any horizontal lines from rectangle B
   for (int i = 0; i < 2; i++) {
      for (int j = 2; j < 4; j++) {
         if (Intersect(a[i][0], a[i][1], a[i][2], b[j][0], b[j][1], b[j][2])) {
            return true;
         }
      }
   }

   // Determine if any horizontal lines from rectangle A
   // intersect with any vertical lines from rectangle B
   for (int i = 2; i < 4; i++) {
      for (int j = 0; j < 2; j++) {
         if (Intersect(b[j][0], b[j][1], b[j][2], a[i][0], a[i][1], a[i][2])) {
            return true;
         }
      }
   }

   bool inside_v = false;
   bool inside_h = false;
   // Determine if rectangle A is inside rectangle B
   for (int i = 0; i < 8; i += 2) {
      int32_t ax = a2[i];
      int32_t ay = a2[i + 1];
      // Test if corner is inside vertical limits
      if (bx1 <= ax && ax <= bx2) {
         inside_v = true;
      }
      // Test if corner is inside horizontal limits
      if (by1 <= ay && ay < by3) {
         inside_h = true;
      }
   }

   if (inside_v && inside_h) {
      return true;
   }
   else {
      inside_v = false;
      inside_h = false;
   }

   // Determine if rectangle B is inside rectangle A
   for (int i = 0; i < 8; i += 2) {
      int32_t bx = b2[i];
      int32_t by = b2[i + 1];
      // Test if corner is inside vertical limits
      if (ax1 <= bx && bx <= ax2) {
         inside_v = true;
      }
      if (ay1 <= by && by <= ay3) {
         inside_h = true;
      }
   } 

   if (inside_v && inside_h) {
      return true;
   }
   return false;
}

/**
 * Determines if the two specified lines intersect at the specified ranges.
 *
 * return:
 * 	True if the the two lines intersect at the specified ranges. Returns false otherwise.
 */
static bool Intersect(int32_t x, int32_t y_start, int32_t y_end, int32_t y, int32_t x_start, int32_t x_end) {
   bool horizontal_intercept = false;
   bool vertical_intercept = false;
   // Determine if the Horizontal line (Y) intercepts the Vertical line (X) at a valid range.
   if (y_start  <= y && y <= y_end) {
      horizontal_intercept = true;
   }
   // Determine if the Vertical line (X) intercepts the Horizontal line (Y) at a valid range.
   if (x_start <= x && x <= x_end) {
      vertical_intercept = true;
   }
   if (horizontal_intercept && vertical_intercept) {
      return true;
   }
   else {
      return false;
   }
}

// On my honor: 
// 
// - I have not discussed the C language code in my program with 
// anyone other than my instructor or the teaching assistants 
// assigned to this course. 
// 
// - I have not used C language code obtained from another student, 
// or any other unauthorized source, either modified or unmodified. 
// 
// - If any C language code or documentation used in my program 
// was obtained from another source, such as a text book or course 
// notes, that has been clearly noted with a proper citation in 
// the comments of my program. 
// 
// - I have not designed this program in such a way as to defeat or 
// interfere with the normal operation of the Curator System. 
// 
// Elmer Landaverde

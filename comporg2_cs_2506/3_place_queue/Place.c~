//	On my honor:
//	- I have not discussed the C language code in my program with
//	anyone other than my instructor or the teaching assistants
//	assigned to this course.
//	- I have not used C language code obtained from another student,
//	or any other unauthorized source, either modified or unmodified.
//	- If any C language code or documentation used in my program
//	was obtained from another source, such as a text book or course
//	notes, that has been clearly noted with a proper citation in
//	the comments of my program.
//
//	Elmer Landaverde

#include "Place.h"
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <stdbool.h>

void string_copy(char* dest, char* src, uint32_t len);

/**  Place_Set() initializes a new Place object.
 *   Pre:      pPlace points to a Place object
 *             FID has been initialized
 *             Name points to a zero-terminated ASCII string
 *             Lat points to a zero-terminated ASCII string
 *             Long points to a zero-terminated ASCII string
 *   Post:     pPlace->FID == FID
 *             pPlace->Name is a copy of Name (i.e., pPlace->Name != Name)
 *             pPlace->Latitude is a copy of Lat (i.e., pPlace->Latitude != Lat)
 *             pPlace->Longitude is a copy of Long (i.e., pPlace->Longitude != Long)
 *   Returns:  false if the object could not be properly initialized; true otherwise
 *  Called by: unknown client code that creates Place objects
 */
bool Place_Set(Place* const pPlace, uint32_t FID, char* Name, char* Lat, char* Long) {
	// Check input.
	if (pPlace == NULL || Name == NULL || Lat == NULL || Long == NULL) {
		return false;
	}
	
	// Set FID
	pPlace->FID = FID;
	
	// Get string lengths.
	uint32_t name_size = strlen(Name) + 1;
	uint32_t lat_size = strlen(Lat) + 1;
	uint32_t long_size = strlen(Long) + 1;
	
	// Initialize character arrays.
	char* name_array = malloc(sizeof(char) * name_size);
	char* lat_array = malloc(sizeof(char) * lat_size);
	char* long_array = malloc(sizeof(char) * long_size);
	
	// Check memory allocation.
	if(name_array == NULL || lat_array == NULL || long_array == NULL) {
		return false;
	}
	
	// Copy the source arrays to the destination arrays.
	string_copy(name_array, Name, name_size);
	string_copy(lat_array, Lat, lat_size);
	string_copy(long_array, Long, long_size);

	// Copy source arrays to place struc.
	pPlace->Name = name_array;
	pPlace->Latitude = lat_array;
	pPlace->Longitude = long_array;

	return true;
}

void string_copy(char* const dest, char* const src, uint32_t len) {
	for (int i = 0; i < len; i++) {
		dest[i] = src[i];
	}
}

/**  Place_Equals() indicates whether two Place objects have equal data members.
 *   Pre:      pLeft and pRight point to proper Place objects
 *   Post:     *pLeft and *pRight are unchanged
 *   Returns:  true if each member of *pLeft equals the corresponding member of *pRight;
 *             false otherwise
 *  Called by: PlaceQueue_Contains()
 */
bool  Place_Equals(const Place* const pLeft, const Place* const pRight) {
	// Check input.
	if(pLeft == NULL || pRight == NULL) {
		return false;
	}
	
	// Check FID.
	if(pLeft->FID != pRight->FID) {
		return false;
	}
	
	// Check Name.
	if (strcmp(pLeft->Name, pRight->Name) != 0) {
		return false;
	}
	// Check Latitude.
	if (strcmp(pLeft->Latitude, pRight->Latitude) != 0) {
		return false;
	}
	// Check Longitude.
	if (strcmp(pLeft->Longitude, pRight->Longitude) != 0) {
		return false;
	}
	
	// Both Place structs are equal.
	return true;
}




#ifndef PLACE_H
#define PLACE_H
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define MAXNAMELENGTH  100   // maximum number of characters in a place name
#define LATITUDELENGTH   7   // maximum number of characters in a latitude string
#define LONGITUDELENGTH  8   // maximum number of characters in a longitude string

/**  Every properly initialized Place object must satisfy the following conditions:
 *
 *    1.  Each array is a zero-terminated ASCII strings.
 *    2.  Each array is of the minimum dimension to store its contents. 
 *    3.  FID is initialized
 */
struct _Place {
   uint32_t FID;
   char*    Name;
   char*    Latitude;
   char*    Longitude;
};

typedef struct _Place Place;

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
bool Place_Set(Place* const pPlace, uint32_t FID, char* Name, char* Lat, char* Long);

/**  Place_Equals() indicates whether two Place objects have equal data members.
 *   Pre:      pLeft and pRight point to proper Place objects
 *   Post:     *pLeft and *pRight are unchanged
 *   Returns:  true if each member of *pLeft equals the corresponding member of *pRight;
 *             false otherwise
 *  Called by: PlaceQueue_Contains()
 */
bool  Place_Equals(const Place* const pLeft, const Place* const pRight);

#endif

#ifndef PLACEQUEUE_H
#define PLACEQUEUE_H
#include <stdio.h>
#include <stdbool.h>
#include "Place.h"
#include "Queue.h"

//////////////////////////////////////////////////////////// PlaceDT type
//  PlaceDT provides a "wrapper" to attach a user data object of type
//  Place to a queue node.
// 
//  Warning: the PlaceQueue implementation receives pointers to user
//  data (Place) objects, and it does not make copies of those objects.
//  Therefore, the user of this code must ensure that the Place objects
//  that are enqueued are not destroyed before they are removed from
//  the queue.  Failure to do this may result in runtime errors.
//
struct _PlaceDT {
   Place* pItem;     // points to the "payload", a Place object supplied
                     //    when PlaceQueue_Schedule() is called
   QNode  Node;      // Queue node to provide list structure
};

typedef struct _PlaceDT PlaceDT;


/**  PlaceDT_Set() initializes a new PlaceDT object.
 *   Pre:      pPlaceDT points to a PlaceDT object
 *             pPlace points to a properly-initialized Place object
 *   Post:     pPlaceDT->pItem == pPlace
 *             The pointers in pPlaceDT->Node are NULL
 *   Returns:  false if the object could not be properly initialized; 
 *             true otherwise
 *   Note:     Whether *pPlace was allocated dynamically or statically
 *             is unknown.  *pPlace is owned by the client who uses
 *             the PlaceQueue implementation, and deallocation of it
 *             is the responsibility of that client.
 *  Called by: PlaceQueue_Schedule()
 */
bool PlaceDT_Set(PlaceDT* const pPlaceDT, Place* pPlace);


//////////////////////////////////////////////////////////// PlaceQueue type
//  PlaceQueue provides the means to create and manipulate a queue of Place
//  objects (e.g., for implementing a "bucket list").
// 
//  Warning: the PlaceQueue implementation receives pointers to user
//  data (Place) objects, and it does not make copies of those objects.
//  Therefore, the user of this code must ensure that the Place objects
//  that are enqueued are not destroyed before they are removed from
//  the queue.  Failure to do this may result in runtime errors.
//
//  Uses:  Queue type implemented in Queue.h and Queue.c.
//
struct _PlaceQueue {
   Queue Q;           // a Queue object to provide the connections
};

typedef struct _PlaceQueue PlaceQueue;


/**  PlaceQueue_Init() initializes a new PlaceQueue object.
 *   Pre:      pPQ points to a PlaceQueue object
 *   Post:     pPQ->Q has been initialized to an empty state
 *   Returns:  false if the object could not be properly initialized; true otherwise
 *  Called by: unknown client code that uses a PlaceQueue object
 */
bool PlaceQueue_Init(PlaceQueue* const pPQ);


/**  PlaceQueue_Schedule() inserts a Place object into the queue.
 *   Pre:      pPQ points to a proper PlaceQueue object
 *             pPlace points to a properly-initialized Place object
 *   Post:     *pPlace has been inserted at the rear of the queue
 *   Returns:  false if the insertion could not be performed; true otherwise
 *  Called by: unknown client code that uses a PlaceQueue object
 */
bool   PlaceQueue_Schedule(PlaceQueue* const pPQ, Place* const pPlace);


/**  PlaceQueue_Visit() pops the front element from the queue and returns it.
 *   Pre:      pPQ points to a proper PlaceQueue object
 *   Post:     pPQ points to a proper PlaceQueue object, with one less element
 *                 (unless the queue was empty)
 *   Returns:  pointer to the removed object; NULL if no object could be removed
 *  Called by: unknown client code that uses a PlaceQueue object
 *   Note:     PlaceDT objects are created and owned by a PlaceQueue object;
 *             therefore, it is the responsibility of the owning PlaceQueue
 *             object to properly deallocate them.
 */
Place* PlaceQueue_Visit(PlaceQueue* const pPQ);


/**  PlaceQueue_Contains() indiates whether the queue contains a specific
 *   Place object.
 *   Pre:      pPQ points to a proper PlaceQueue object
 *             pPlace points to a proper Place object
 *   Post:     *pPQ and *pPlace are unchanged
 *   Returns:  true if the queue contains a Place object that matches 
 *             *pPlace, according to Place_Equals();
 *             false otherwise
 *  Called by: unknown client code that uses a PlaceQueue object
 */
bool   PlaceQueue_Contains(const PlaceQueue* const pPQ, const Place* const pPlace);


/**  PlaceQueue_Empty() indicates whether the queue contains elements.
 *   Pre:      pPQ points to a proper PlaceQueue object
 *   Post:     pPQ points to an unchanged PlaceQueue object
 *   Returns:  true iff pPQ->Q is empty
 *  Called by: unknown client code that uses a PlaceQueue object
 *   Note:     PlaceDT objects are created and owned by a PlaceQueue object;
 *             therefore, it is the responsibility of the owning PlaceQueue
 *             object to properly deallocate them.
 */
bool   PlaceQueue_Empty(const PlaceQueue* const pPQ);

#endif

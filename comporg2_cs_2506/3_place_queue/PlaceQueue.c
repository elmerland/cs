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
#include "Queue.h"
#include "PlaceQueue.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
bool PlaceDT_Set(PlaceDT* const pPlaceDT, Place* pPlace) {
	// Check input.
	if (pPlaceDT == NULL || pPlace == NULL) {
		return false;
	}
	
	// Set the Place item.
	pPlaceDT->pItem = pPlace;
	
	// Initialize the Node.
	QNode_Init(&pPlaceDT->Node);
	
	return true;
}

/**  PlaceQueue_Init() initializes a new PlaceQueue object.
 *   Pre:      pPQ points to a PlaceQueue object
 *   Post:     pPQ->Q has been initialized to an empty state
 *   Returns:  false if the object could not be properly initialized; true otherwise
 *  Called by: unknown client code that uses a PlaceQueue object
 */
bool PlaceQueue_Init(PlaceQueue* const pPQ) {
	// Check the input.
	if(pPQ == NULL) {
		return false;
	}
	
	// Initialize the Queue.
	Queue_Init(&pPQ->Q);
	
	return true;
}


/**  PlaceQueue_Schedule() inserts a Place object into the queue.
 *   Pre:      pPQ points to a proper PlaceQueue object
 *             pPlace points to a properly-initialized Place object
 *   Post:     *pPlace has been inserted at the rear of the queue
 *   Returns:  false if the insertion could not be performed; true otherwise
 *  Called by: unknown client code that uses a PlaceQueue object
 */
bool   PlaceQueue_Schedule(PlaceQueue* const pPQ, Place* const pPlace) {
	// Check input.
	if(pPQ == NULL || pPlace == NULL) {
		return false;
	}
	
	// Allocte memory for Place Duct Tape.
	PlaceDT *pdt = malloc(sizeof(PlaceDT));
	
	// Check if malloc was successful.
	if(pdt == NULL) {
		return false;
	}
	
	// Initialize the Place Duct Tape.
	PlaceDT_Set(pdt, pPlace);
	
	// Enqueue the Node.
	Queue_Push(&pPQ->Q, &pdt->Node);
	
	return true;
}


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
Place* PlaceQueue_Visit(PlaceQueue* const pPQ) {
	// Check input.
	if(pPQ == NULL) {
		return NULL;
	}
	
	// Check if Queue is empty.
	if (PlaceQueue_Empty(pPQ)) {
		return NULL;
	}
	
	// Pop front Node.
	QNode *node = Queue_Pop(&pPQ->Q);
	
	// Get Place Duct Tape struc.
	PlaceDT *pdt = Queue_Entry(node, PlaceDT, Node);
	
	// Get the pointer to Place struc.
	Place *place_pointer = pdt->pItem;
	
	// Deallocate memory for Place Duct Tape struc.
	free(pdt);
	
	return place_pointer;
}


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
bool   PlaceQueue_Contains(const PlaceQueue* const pPQ, const Place* const pPlace) {
	// Check input.
	if (pPQ == NULL || pPlace == NULL) {
		return false;
	}
	
	// Check if Queue is empty.
	if(PlaceQueue_Empty(pPQ)) {
		return false;
	}
	
	// Get the rear guard.
	const QNode *const guard = Queue_End(&pPQ->Q);
	
	// Get the first internal node.
	QNode *node = Queue_Front(&pPQ->Q);
	
	// Traverse Queue.
	while(node->next != guard) {
		// Get Place Duct Tape struc.
		PlaceDT *pdt = Queue_Entry(node, PlaceDT, Node);
		// Get Place struc.
		Place *p = pdt->pItem;
		// Compare the two Place strucs.
		if(Place_Equals(p, pPlace)) {
			// Places are a match.
			return true;
		}
		// Get next node.
		node = node->next;
	}
	
	// No two equal Place strucs where found.
	return false;
}


/**  PlaceQueue_Empty() indicates whether the queue contains elements.
 *   Pre:      pPQ points to a proper PlaceQueue object
 *   Post:     pPQ points to an unchanged PlaceQueue object
 *   Returns:  true iff pPQ->Q is empty
 *  Called by: unknown client code that uses a PlaceQueue object
 *   Note:     PlaceDT objects are created and owned by a PlaceQueue object;
 *             therefore, it is the responsibility of the owning PlaceQueue
 *             object to properly deallocate them.
 */
bool   PlaceQueue_Empty(const PlaceQueue* const pPQ) {
	// Determine if the Queue is empty.
	return Queue_Empty(&pPQ->Q);
}

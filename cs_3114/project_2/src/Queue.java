/**
 * Source code example for "A Practical Introduction to Data Structures and
 * Algorithm Analysis, 3rd Edition (Java)" by Clifford A. Shaffer Copyright
 * 2008-2011 by Clifford A. Shaffer
 */

/**
 * Queue ADT.
 * 
 * @author Virginia tech
 * @version Sep 24, 2013
 * @param <E>
 *            Generic type
 */
public interface Queue<E>
{
	
	/**
	 * Reinitialize the queue. The user is responsible for reclaiming the
	 * storage used by the queue elements.
	 */
	public void clear();
	
	
	/**
	 * Remove and return element at the front of the queue.
	 * 
	 * @return The element at the front of the queue.
	 */
	public E dequeue();
	
	
	/**
	 * Place an element at the rear of the queue.
	 * 
	 * @param it
	 *            The element being enqueued.
	 */
	public void enqueue(E it);
	
	
	/**
	 * Gets the front element of the Queue.
	 * 
	 * @return The front element.
	 */
	public E frontValue();
	
	
	/**
	 * Gets the number of elements in the queue.
	 * 
	 * @return The number of elements in the queue.
	 */
	public int length();
}

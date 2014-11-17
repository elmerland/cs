/**
 * Source code example for "A Practical Introduction to Data Structures and
 * Algorithm Analysis, 3rd Edition (Java)" by Clifford A. Shaffer Copyright
 * 2008-2011 by Clifford A. Shaffer
 */

/**
 * Linked queue implementation
 * 
 * @param <E>
 *            The generic type
 * @author CS 3114
 * @version Sep 10, 2013
 */
public class LQueue<E>
	implements Queue<E>
{
	
	/**
	 * Pointer to the front of the queue.
	 */
	private Link<E>	front;
	
	/**
	 * Pointer to the rear of the queue.
	 */
	private Link<E>	rear;
	
	/**
	 * Number of elements in the queue.
	 */
	private int		size;
	
	
	/**
	 * Create a new LQueue object.
	 */
	public LQueue()
	{
		init();
	}
	
	
	/**
	 * Create a new LQueue object.
	 * 
	 * @param size
	 *            The size of the array. This parameter gets ignored.
	 */
	public LQueue(int size)
	{
		init();
	}
	
	
	/**
	 * Reinitialize queue
	 */
	public void clear()
	{
		init();
	}
	
	
	/**
	 * Remove and return element from front
	 * 
	 * @return Returns the Object that was removed.
	 */
	public E dequeue()
	{
		if (size == 0)
		{
			return null;
		}
		
		E it = front.next().element(); // Store dequeued value
		front.setNext(front.next().next()); // Advance front
		
		if (front.next() == null)
		{
			rear = front; // Last Object
		}
		size--;
		return it; // Return Object
	}
	
	
	/**
	 * Put element on rear
	 * 
	 * @param it
	 *            The new object to add to the queue.
	 */
	public void enqueue(E it)
	{
		rear.setNext(new Link<E>(it, null));
		rear = rear.next();
		size++;
	}
	
	
	/**
	 * Gets the element at the front of the queue.
	 * 
	 * @return Front element
	 */
	public E frontValue()
	{
		if (size == 0)
		{
			return null;
		}
		return front.next().element();
	}
	
	
	/**
	 * Gets the length of the queue.
	 * 
	 * @return Queue size
	 */
	public int length()
	{
		return size;
	}
	
	
	/**
	 * Generate a human-readable representation of this queue's contents that
	 * looks something like this: < 1 2 3 >. This method uses toString() on the
	 * individual elements.
	 * 
	 * @return The string representation of this queue
	 */
	public String toString()
	{
		StringBuffer out = new StringBuffer((length() + 1) * 4);
		out.append("< ");
		for (Link<E> i = front.next(); i != null; i = i.next())
		{
			out.append(i.element());
			out.append(" ");
		}
		out.append(">");
		return out.toString();
	}
	
	
	/**
	 * Initialize queue
	 */
	private void init()
	{
		rear = new Link<E>(null);
		front = rear;
		size = 0;
	}
}

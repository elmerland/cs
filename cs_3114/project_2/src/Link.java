/**
 * Source code example for "A Practical Introduction to Data Structures and
 * Algorithm Analysis, 3rd Edition (Java)" by Clifford A. Shaffer Copyright
 * 2008-2011 by Clifford A. Shaffer
 */

/**
 * Implementation of a singly linked list.
 * 
 * @param <E>
 *            The generic type.
 * @author CS 3114
 * @version Sep 10, 2013
 */
public class Link<E>
{
	
	/**
	 * The values for this node.
	 */
	private E		element;
	
	/**
	 * Pointer to the next node in the list.
	 */
	private Link<E>	next;
	
	
	/**
	 * Create a new Link object.
	 * 
	 * @param it
	 *            The object to be stored.
	 * @param nextval
	 *            The pointer to the next Link.
	 */
	public Link(E it, Link<E> nextval)
	{
		element = it;
		next = nextval;
	}
	
	
	/**
	 * Create a new Link object.
	 * 
	 * @param nextval
	 *            The pointer to the next Link.
	 */
	public Link(Link<E> nextval)
	{
		next = nextval;
	}
	
	
	/**
	 * Gets the element stored in this Link.
	 * 
	 * @return The element of this Link.
	 */
	public E element()
	{
		return element;
	}
	
	
	/**
	 * Returns the object to the next Link.
	 * 
	 * @return The next Link.
	 */
	public Link<E> next()
	{
		return next;
	}
	
	
	/**
	 * Updates the element stored in this Link.
	 * 
	 * @param it
	 *            The new element to be stored in this Link.
	 * @return The new element that is store in this Link.
	 */
	public E setElement(E it)
	{
		element = it;
		return element;
	}
	
	
	/**
	 * Sets the pointer to the next Link to the one indicated.
	 * 
	 * @param nextval
	 *            The new next Link.
	 * @return The new next Link.
	 */
	public Link<E> setNext(Link<E> nextval)
	{
		next = nextval;
		return next;
	}
}

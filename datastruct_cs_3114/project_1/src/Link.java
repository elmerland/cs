/**
 * Source code example for "A Practical Introduction to Data Structures and
 * Algorithm Analysis, 3rd Edition (Java)" by Clifford A. Shaffer Copyright
 * 2008-2011 by Clifford A. Shaffer
 */

/**
 * Singly linked list.
 *
 * @param <E>
 *            The generic type.
 * @author CS 3114
 * @version Sep 10, 2013
 */
public class Link<E>
{
    private E       element; // Value for this node
    private Link<E> next;   // Pointer to next node in list


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
     * Returns the object to the next Link.
     *
     * @return The next Link.
     */
    public Link<E> next()
    {
        return next;
    } // Return next field


    /**
     * Sets the pointer to the next Link to the one indicated.
     *
     * @param nextval
     *            The new next Link.
     * @return The new next Link.
     */
    public Link<E> setNext(Link<E> nextval) // Set next field
    {
        next = nextval;
        return next;
    } // Return element field


    /**
     * Gets the element stored in this Link.
     *
     * @return The element of this Link.
     */
    public E element()
    {
        return element;
    } // Set element field


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
}

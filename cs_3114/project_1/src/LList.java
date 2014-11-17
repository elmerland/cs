/**
 * Source code example for "A Practical Introduction to Data Structures and
 * Algorithm Analysis, 3rd Edition (Java)" by Clifford A. Shaffer Copyright
 * 2008-2011 by Clifford A. Shaffer
 */

/**
 * Linked list implementation.
 *
 * @param <E>
 *            The generic type
 * @author CS 3114
 * @version Sep 10, 2013
 */
public class LList<E>
    implements List<E>
{
    /**
     * Pointer to list header
     */
    private Link<E>   head;
    /**
     * Pointer to last element
     */
    private Link<E>   tail;
    /**
     * Access to current element.
     */
    protected Link<E> curr;
    /**
     * Size of the list.
     */
    int               cnt;


    /**
     * Create a new LList object
     *
     * @param size
     *            The size of the linked list (This gets ignored)
     */
    public LList(int size)
    {
        this();
    } // Constructor -- Ignore size


    /**
     * Create a new LList object.
     */
    public LList()
    {
        head = new Link<E>(null); // Create header
        tail = head;
        curr = tail;
        cnt = 0;
    }


    /**
     * Remove all elements
     */
    public void clear()
    {
        head.setNext(null); // Drop access to links
        head = new Link<E>(null); // Create header
        tail = head;
        curr = tail;
        cnt = 0;
    }


    /**
     * Insert "it" at current position
     *
     * @param it
     *            The Object to insert to the current position.
     */
    public void insert(E it)
    {
        curr.setNext(new Link<E>(it, curr.next()));
        if (tail == curr)
        {
            tail = curr.next(); // New tail
        }
        cnt++;
    }


    /**
     * Append "it" to list
     *
     * @param it
     *            The new Object to append to the list.
     */
    public void append(E it)
    {
        tail = tail.setNext(new Link<E>(it, null));
        cnt++;
    }


    /**
     * Remove and return current element
     *
     * @return The object that was removed.
     */
    public E remove()
    {
        if (curr.next() == null)
        {
            return null; // Nothing to remove
        }
        E it = curr.next().element(); // Remember value
        if (tail == curr.next())
        {
            tail = curr; // Removed last
        }
        curr.setNext(curr.next().next()); // Remove from list
        cnt--; // Decrement count
        return it; // Return value
    }


    /**
     * Set curr at list start
     */
    public void moveToStart()
    {
        curr = head;
    }


    /**
     * Set curr at list end
     */
    public void moveToEnd()
    {
        curr = tail;
    }


    /**
     * Move curr one step left; no change if now at front
     */
    public void prev()
    {
        if (curr == head) {
            return; // No previous element
        }
        Link<E> temp = head;
        // March down list until we find the previous element
        while (temp.next() != curr)
        {
            temp = temp.next();
        }
        curr = temp;
    }


    /**
     * Move curr one step right; no change if now at end
     */
    public void next()
    {
        if (curr != tail)
        {
            curr = curr.next();
        }
    }


    /**
     * Get the length of the list.
     *
     * @return List length
     */
    public int length()
    {
        return cnt;
    }


    /**
     * Gets the position of the current element.
     *
     * @return The position of the current element
     */
    public int currPos()
    {
        Link<E> temp = head;
        int i;
        for (i = 0; curr != temp; i++)
        {
            temp = temp.next();
        }
        return i;
    }


    /**
     * Move down list to "pos" position
     *
     * @param pos
     *            The position to move to.
     */
    public void moveToPos(int pos)
    {
        if (pos >= 0 && pos <= cnt) {
            curr = head;
            for (int i = 0; i < pos; i++)
            {
                curr = curr.next();
            }
        }
    }


    /**
     * Gets the value of current object
     *
     * @return Current element value
     */
    public E getValue()
    {
        if (curr.next() == null)
        {
            return null;
        }
        return curr.next().element();
    }


// Extra stuff not printed in the book.

    /**
     * Generate a human-readable representation of this list's contents that
     * looks something like this: < 1 2 3 | 4 5 6 >. The vertical bar represents
     * the current location of the fence. This method uses toString() on the
     * individual elements.
     *
     * @return The string representation of this list
     */
    public String toString()
    {
        // Save the current position of the list
        int oldPos = currPos();
        int length = length();
        StringBuffer out = new StringBuffer((length() + 1) * 4);

        moveToStart();
        out.append("< ");
        for (int i = 0; i < oldPos; i++)
        {
            out.append(getValue());
            out.append(" ");
            next();
        }
        out.append("| ");
        for (int i = oldPos; i < length; i++)
        {
            out.append(getValue());
            out.append(" ");
            next();
        }
        out.append(">");
        moveToPos(oldPos); // Reset the fence to its original position
        return out.toString();
    }
}

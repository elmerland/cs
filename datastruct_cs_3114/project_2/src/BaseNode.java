/**
 * Source code example for "A Practical Introduction to Data Structures and
 * Algorithm Analysis, 3rd Edition (Java)" by Clifford A. Shaffer Copyright
 * 2008-2011 by Clifford A. Shaffer
 */

/**
 * ADT for binary tree nodes
 * 
 * @param <E>
 *            The generic type.
 */
public interface BaseNode<E>
{
	
	/**
	 * Get the element value
	 * 
	 * @return The element value of this node.
	 */
	public E element();
	
	
	/**
	 * Determines if this node is a leaf node.
	 * 
	 * @return True if a leaf node, false otherwise
	 */
	public boolean isLeaf();
	
	
	/**
	 * Gets the left child of this node.
	 * 
	 * @return The left child
	 */
	public BaseNode<E> left();
	
	
	/**
	 * Gets the right child of this node.
	 * 
	 * @return The right child
	 */
	public BaseNode<E> right();
	
	
	/**
	 * Sets the element value for this node.
	 * 
	 * @param v
	 *            The new element value.
	 */
	public void setElement(E v);
}

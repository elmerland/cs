/**
 * Source code example for "A Practical Introduction to Data Structures and
 * Algorithm Analysis, 3rd Edition (Java)" by Clifford A. Shaffer Copyright
 * 2008-2011 by Clifford A. Shaffer
 */

/**
 * ADT for binary tree nodes
 */
public interface BaseNode
{

	/**
	 * Get the element value
	 *
	 * @return The element value of this node.
	 */
	public WatcherUser element();


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
	 * @throws Exception
	 *             Could not get left child.
	 */
	public BaseNode left()
		throws Exception;


	/**
	 * Gets the right child of this node.
	 *
	 * @return The right child
	 * @throws Exception
	 *             Could not get right child.
	 */
	public BaseNode right()
		throws Exception;


	/**
	 * Sets the element value for this node.
	 *
	 * @param v
	 *            The new element value.
	 */
	public void setElement(WatcherUser v);
}

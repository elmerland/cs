/**
 * Source code example for "A Practical Introduction to Data Structures and
 * Algorithm Analysis, 3rd Edition (Java)" by Clifford A. Shaffer Copyright
 * 2008-2011 by Clifford A. Shaffer
 */

/**
 * Binary tree node implementation: Pointers to children
 * 
 * @param <Key>
 *            The associated key for the record.
 * @param <E>
 *            The data element
 */
class BSTNode<Key, E>
	implements BaseNode<E>
{
	
	private Key				key;		// Key for this node
										
	private E				element;	// Element for this node
										
	private BSTNode<Key, E>	left;		// Pointer to left child
										
	private BSTNode<Key, E>	right;		// Pointer to right child
										
	
	/**
	 * Create a new BSTNode object.
	 * 
	 * @param k
	 *            The associated key for the record
	 * @param val
	 *            The data element.
	 */
	public BSTNode(Key k, E val)
	{
		left = null;
		right = null;
		key = k;
		element = val;
	}
	
	
	/**
	 * Create a new BSTNode object.
	 * 
	 * @param k
	 *            The associated key for the record
	 * @param val
	 *            The data element
	 * @param l
	 *            The left child node.
	 * @param r
	 *            The right child node.
	 */
	public BSTNode(Key k, E val, BSTNode<Key, E> l, BSTNode<Key, E> r)
	{
		left = l;
		right = r;
		key = k;
		element = val;
	}
	
	
	/**
	 * Get the element value
	 * 
	 * @return Gets the element value of this BSTNode object.
	 */
	public E element()
	{
		return element;
	}
	
	
	/**
	 * Determines if this node is a leaf node.
	 * 
	 * @return True if a leaf node, false otherwise
	 */
	public boolean isLeaf()
	{
		return (left == null) && (right == null);
	}
	
	
	/**
	 * Get the key value
	 * 
	 * @return The key value that was set for this BSTNode object.
	 */
	public Key key()
	{
		return key;
	}
	
	
	/**
	 * Get the left child
	 * 
	 * @return The left child node of this record.
	 */
	public BSTNode<Key, E> left()
	{
		return left;
	}
	
	
	/**
	 * Get the right child
	 * 
	 * @return The right child node of this record.
	 */
	public BSTNode<Key, E> right()
	{
		return right;
	}
	
	
	/**
	 * Sets a new element value for this BSTNode object.
	 * 
	 * @param v
	 *            The new element value for this record.
	 */
	public void setElement(E v)
	{
		element = v;
	}
	
	
	/**
	 * Set a new key value for this BSTNode object
	 * 
	 * @param k
	 *            The associated key for the record.
	 */
	public void setKey(Key k)
	{
		key = k;
	}
	
	
	/**
	 * Sets the left child for this BSTNode object.
	 * 
	 * @param p
	 *            The new left child node of this record.
	 */
	public void setLeft(BSTNode<Key, E> p)
	{
		left = p;
	}
	
	
	/**
	 * Sets the right child for this BSTNode object.
	 * 
	 * @param p
	 *            The new right child node for this record.
	 */
	public void setRight(BSTNode<Key, E> p)
	{
		right = p;
	}
}

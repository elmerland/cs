/**
 * Source code example for "A Practical Introduction to Data Structures and
 * Algorithm Analysis, 3rd Edition (Java)" by Clifford A. Shaffer Copyright
 * 2008-2011 by Clifford A. Shaffer
 */

/**
 * Binary Search Tree implementation for Dictionary ADT
 * 
 * @param <Key>
 *            The associated key for the record.
 * @param <E>
 *            The data element.
 */
class BST<Key extends Comparable<? super Key>, E>
	implements Dictionary<Key, E>
{
	
	/**
	 * The number of nodes contained by the binary tree.
	 */
	private int				nodecount;
	
	/**
	 * The string buffer to contain the string representation of the binary
	 * search tree.
	 */
	private String			out;
	
	/**
	 * The node that represents the root of the binary tree.
	 */
	private BSTNode<Key, E>	root;
	
	
	/**
	 * Create a new BST object.
	 */
	BST()
	{
		root = null;
		nodecount = 0;
	}
	
	
	/**
	 * Reinitialize the tree.
	 */
	public void clear()
	{
		root = null;
		nodecount = 0;
	}
	
	
	/**
	 * Finds the value elements that corresponds to the given key object.
	 * 
	 * @param k
	 *            The key value to find.
	 * @return Record with key value k, null if none exist.
	 */
	public E find(Key k)
	{
		return findhelp(root, k);
	}
	
	
	/**
	 * Insert a record into the tree.
	 * 
	 * @param k
	 *            Key value of the record.
	 * @param e
	 *            The record to insert.
	 */
	public void insert(Key k, E e)
	{
		root = inserthelp(root, k, e);
		nodecount++;
	}
	
	
	/**
	 * Remove a record from the tree.
	 * 
	 * @param k
	 *            Key value of record to remove.
	 * @return The record removed, null if there is none.
	 */
	public E remove(Key k)
	{
		E temp = findhelp(root, k); // First find it
		if (temp != null)
		{
			root = removehelp(root, k); // Now remove it
			nodecount--;
		}
		return temp;
	}
	
	
	/**
	 * Remove and return the root node from the dictionary.
	 * 
	 * @return The record removed, null if tree is empty.
	 */
	public E removeAny()
	{
		if (root == null)
		{
			return null;
		}
		E temp = root.element();
		root = removehelp(root, root.key());
		nodecount--;
		return temp;
	}
	
	
	/**
	 * Gets the number of records in the dictionary.
	 * 
	 * @return The number of records in the dictionary.
	 */
	public int size()
	{
		return nodecount;
	}
	
	
	/**
	 * Prints a string representation of this binary search tree.
	 */
	public String toString()
	{
		out = "";
		printhelp(root, 0);
		return out.toString();
	}
	
	
	/**
	 * Delete the record with the highest key value.
	 * 
	 * @param rt
	 *            The root of the tree.
	 * @return The root of the tree that was originally given with the highest
	 *         key record removed.
	 */
	private BSTNode<Key, E> deleteMax(BSTNode<Key, E> rt)
	{
		if (rt.right() == null)
		{
			return rt.left();
		}
		rt.setRight(deleteMax(rt.right()));
		return rt;
	}
	
	
	/**
	 * Helper method that finds the value object that has a matching key element
	 * with the one provided.
	 * 
	 * @param rt
	 *            The root node of the tree.
	 * @param k
	 *            The associated key value.
	 * @return The data element associated with the given key. Returns null if
	 *         no such data elements exists. if there are more than one
	 *         associated data elements it returns an arbitrary one.
	 */
	private E findhelp(BSTNode<Key, E> rt, Key k)
	{
		if (rt == null || k == null)
		{
			return null;
		}
		if (rt.key().compareTo(k) > 0)
		{
			return findhelp(rt.left(), k);
		}
		else if (rt.key().compareTo(k) == 0)
		{
			return rt.element();
		}
		else
		{
			return findhelp(rt.right(), k);
		}
	}
	
	
	/**
	 * Gets the record with the highest key value.
	 * 
	 * @param rt
	 *            The root of the tree.
	 * @return The record with the highest key value.
	 */
	private BSTNode<Key, E> getMax(BSTNode<Key, E> rt)
	{
		// Get the maximum valued element in a subtree
		if (rt.right() == null)
		{
			return rt;
		}
		return getMax(rt.right());
	}
	
	
	/**
	 * Helper method that inserts a new record in the tree at the appropriate
	 * location.
	 * 
	 * @param rt
	 *            The root node of the binary tree where the new node will be
	 *            inserted.
	 * @param k
	 *            The key associated with the record to be inserted.
	 * @param e
	 *            The data element of the record to be inserted.
	 * @return The current subtree, modified to contain the new item.
	 */
	private BSTNode<Key, E> inserthelp(BSTNode<Key, E> rt, Key k, E e)
	{
		if (rt == null)
		{
			return new BSTNode<Key, E>(k, e);
		}
		if (rt.key().compareTo(k) >= 0)
		{
			rt.setLeft(inserthelp(rt.left(), k, e));
		}
		else
		{
			rt.setRight(inserthelp(rt.right(), k, e));
		}
		return rt;
	}
	
	
	/**
	 * Helper method that prints a string representation of the binary search
	 * tree.
	 * 
	 * @param rt
	 *            The root node of the binary search tree to print.
	 */
	private void printhelp(BSTNode<Key, E> rt, int depth)
	{
		if (rt == null)
		{
			return;
		}
		printhelp(rt.left(), depth + 1);
		printVisit(rt.key(), depth);
		printhelp(rt.right(), depth + 1);
	}
	
	
	/**
	 * Prints a string representation of a data element.
	 * 
	 * @param it
	 *            The data element to be printed.
	 */
	private void printVisit(Key k, int depth)
	{
		for (int i = 0; i < depth; i++)
		{
			out = out + "..";
		}
		out = out + (find(k).toString()) + "\n";
	}
	
	
	/**
	 * Remove a node with key value k.
	 * 
	 * @param rt
	 *            The root node of the tree where the node will be removed.
	 * @param k
	 *            The key associated with the record to be removed.
	 * @return The tree with the node removed.
	 */
	private BSTNode<Key, E> removehelp(BSTNode<Key, E> rt, Key k)
	{
		if (rt == null)
		{
			return null;
		}
		if (rt.key().compareTo(k) > 0)
		{
			rt.setLeft(removehelp(rt.left(), k));
		}
		else if (rt.key().compareTo(k) < 0)
		{
			rt.setRight(removehelp(rt.right(), k));
		}
		else
		{
			// Found it
			if (rt.left() == null)
			{
				return rt.right();
			}
			else if (rt.right() == null)
			{
				return rt.left();
			}
			else
			{
				// Two children
				BSTNode<Key, E> temp = getMax(rt.left());
				rt.setElement(temp.element());
				rt.setKey(temp.key());
				rt.setLeft(deleteMax(rt.left()));
			}
		}
		return rt;
	}
	
}

/**
 * Implementation for internal node of a bin tree.
 * 
 * @author Elmer Landaverde
 * @version Sep 30, 2013
 * @param <E>
 *            The generic type.
 */
public class BInternalNode<E extends Visitable>
	implements BinTreeNode<E>
{
	
	/**
	 * The left child node.
	 */
	private BinTreeNode<E>	left;
	
	/**
	 * The right child node.
	 */
	private BinTreeNode<E>	right;
	
	
	/**
	 * Create a new InternalNode object.
	 * 
	 * @param l
	 *            The left child node.
	 * @param r
	 *            The right child node.
	 */
	public BInternalNode(BinTreeNode<E> l, BinTreeNode<E> r)
	{
		left = l;
		right = r;
	}
	
	
	@Override
	public E element()
	{
		return null;
	}
	
	
	/**
	 * When this internal node has only one leaf node and one empty node, it
	 * returns the leaf node.
	 * 
	 * @return The only leaf node of this internal node.
	 */
	public BinTreeNode<E> getNonEmptyChild()
	{
		if (!hasOneLeaf())
		{
			return null;
		}
		else
		{
			if (left instanceof BLeafNode)
			{
				return left;
			}
			else
			{
				return right;
			}
		}
	}
	
	
	/**
	 * Determines if this internal node has one leaf node and one empty node.
	 * This method is meant to determine if this internal node can be converted
	 * to a leaf node when removing node from the tree.
	 * 
	 * @return True if this internal node contains only one leaf node and one
	 *         empty node.
	 */
	public boolean hasOneLeaf()
	{
		boolean hasOnlyRightLeaf =
			left instanceof BEmptyNode && right instanceof BLeafNode;
		boolean hasOnlyLeftLeaf =
			left instanceof BLeafNode && right instanceof BEmptyNode;
		if (hasOnlyRightLeaf || hasOnlyLeftLeaf)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	
	@Override
	public boolean isLeaf()
	{
		return false;
	}
	
	
	@Override
	public BinTreeNode<E> left()
	{
		return left;
	}
	
	
	@Override
	public BinTreeNode<E> right()
	{
		return right;
	}
	
	
	@Override
	public void setElement(E v)
	{
		// Do nothing.
	}
	
	
	/**
	 * Set the left child node.
	 * 
	 * @param n
	 *            The new left child node.
	 */
	public void setLeft(BinTreeNode<E> n)
	{
		left = n;
	}
	
	
	/**
	 * Set the right child node.
	 * 
	 * @param n
	 *            The new right child node.
	 */
	public void setRight(BinTreeNode<E> n)
	{
		right = n;
	}
	
}

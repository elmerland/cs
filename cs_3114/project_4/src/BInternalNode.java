/**
 * Implementation for internal node of a bin tree.
 *
 * @author Elmer Landaverde
 * @version Sep 30, 2013
 */
public class BInternalNode
	extends BinTreeNode
{

	/**
	 * The memory handle to the left child node.
	 */
	private MemoryHandle	left;

	/**
	 * The left child node object.
	 */
	private BinTreeNode		leftBinTreeNode		= null;

	/**
	 * The memory handle to the right child node.
	 */
	private MemoryHandle	right;

	/**
	 * The right child node object.
	 */
	private BinTreeNode		rightBinTreeNode	= null;


	/**
	 * Create a new InternalNode object.
	 *
	 * @param l
	 *            The left child node.
	 * @param r
	 *            The right child node.
	 */
	public BInternalNode(MemoryHandle l, MemoryHandle r)
	{
		left = l;
		right = r;
	}


	@Override
	public WatcherUser element()
	{
		return null;
	}


	/**
	 * When this internal node has only one leaf node and one empty node, it
	 * returns the leaf node. This method should be called, only after a call to
	 * the hasOneLeaf method returned true.
	 *
	 * @return The only leaf node of this internal node.
	 * @throws Exception
	 *             Could not get child.
	 */
	public MemoryHandle getNonEmptyChild()
		throws Exception
	{
		BinTreeNode leftNode = left();
		if (leftNode instanceof BLeafNode)
		{
			return left;
		}
		else
		{
			return right;
		}
	}


	/**
	 * Determines if this internal node has one leaf node and one empty node.
	 * This method is meant to determine if this internal node can be converted
	 * to a leaf node when removing node from the tree.
	 *
	 * @return True if this internal node contains only one leaf node and one
	 *         empty node.
	 * @throws Exception
	 *             Could not get child
	 */
	public boolean hasOneLeaf()
		throws Exception
	{
		BinTreeNode leftNode = left();
		BinTreeNode rightNode = right();
		boolean hasOnlyRightLeaf =
			leftNode instanceof BEmptyNode && rightNode instanceof BLeafNode;
		boolean hasOnlyLeftLeaf =
			leftNode instanceof BLeafNode && rightNode instanceof BEmptyNode;
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
	public BinTreeNode left()
		throws Exception
	{
		if (leftBinTreeNode == null)
		{
			leftBinTreeNode = Serialize.getNodeFromDisk(left);
		}
		return leftBinTreeNode;
	}


	@Override
	public MemoryHandle leftHandle()
	{
		return left;
	}


	@Override
	public BinTreeNode right()
		throws Exception
	{
		if (rightBinTreeNode == null)
		{
			rightBinTreeNode = Serialize.getNodeFromDisk(right);
		}
		return rightBinTreeNode;
	}


	@Override
	public MemoryHandle rightHandle()
	{
		return right;
	}


	@Override
	public void setElement(WatcherUser v)
	{
		// Do nothing.
	}


	/**
	 * Set the left child node.
	 *
	 * @param n
	 *            The new left child node.
	 */
	public void setLeft(MemoryHandle n)
	{
		left = n;
	}


	/**
	 * Set the right child node.
	 *
	 * @param n
	 *            The new right child node.
	 */
	public void setRight(MemoryHandle n)
	{
		right = n;
	}

}

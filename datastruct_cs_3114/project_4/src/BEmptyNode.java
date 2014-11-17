/**
 * The implementation for an empty bin node. Objects of BEmptyNode contain no
 * information, and are meant to use as a flyweight for the bin tree.
 *
 * @author Elmer landaverde
 * @version Sep 30, 2013
 */
public class BEmptyNode
	extends BinTreeNode
{

	/**
	 * Create a new BEmptyNode object.
	 */
	public BEmptyNode()
	{
		// Empty constructor.
	}


	@Override
	public WatcherUser element()
	{
		return null;
	}


	@Override
	public boolean isLeaf()
	{
		return true;
	}


	@Override
	public BinTreeNode left()
	{
		return null;
	}


	@Override
	public MemoryHandle leftHandle()
	{
		return null;
	}


	@Override
	public BinTreeNode right()
	{
		return null;
	}


	@Override
	public MemoryHandle rightHandle()
	{
		return null;
	}


	@Override
	public void setElement(WatcherUser v)
	{
		// Empty method.
	}

}

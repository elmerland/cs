/**
 * The implementation for an empty bin node. Objects of BEmptyNode contain no
 * information, and are meant to use as a flyweight for the bin tree.
 * 
 * @param <E>
 *            The generic type.
 * @author Elmer landaverde
 * @version Sep 30, 2013
 */
public class BEmptyNode<E extends Visitable>
	implements BinTreeNode<E>
{
	
	/**
	 * Create a new BEmptyNode object.
	 */
	public BEmptyNode()
	{
		// Empty constructor.
	}
	
	
	@Override
	public E element()
	{
		return null;
	}
	
	
	@Override
	public boolean isLeaf()
	{
		return true;
	}
	
	
	@Override
	public BinTreeNode<E> left()
	{
		return null;
	}
	
	
	@Override
	public BinTreeNode<E> right()
	{
		return null;
	}
	
	
	@Override
	public void setElement(E v)
	{
		// Empty method.
	}
	
}

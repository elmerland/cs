/**
 * Interface for bin tree node. This interface is meant to provide a common
 * implementation for all type of node for a bin tree.
 * 
 * @author Elmer Landaverde
 * @version Sep 30, 2013
 * @param <E>
 *            the generic type.
 */
interface BinTreeNode<E extends Visitable>
	extends BaseNode<E>
{
	
	/**
	 * Gets the left child of this node.
	 * 
	 * @return The left child
	 */
	public abstract BinTreeNode<E> left();
	
	
	/**
	 * Gets the right child of this node.
	 * 
	 * @return The right child
	 */
	public abstract BinTreeNode<E> right();
}

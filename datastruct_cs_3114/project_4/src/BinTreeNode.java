/**
 * Interface for bin tree node. This interface is meant to provide a common
 * implementation for all type of node for a bin tree.
 *
 * @author Elmer Landaverde
 * @version Sep 30, 2013
 */
abstract class BinTreeNode
	implements BaseNode
{

	/**
	 * Gets the left child of this node.
	 *
	 * @return The left child
	 */
	public abstract BinTreeNode left()  throws Exception;


	/**
	 * Gets the left memory handle that points to the left child of this node.
	 *
	 * @return The left memory handle.
	 */
	public abstract MemoryHandle leftHandle();


	/**
	 * Gets the right child of this node.
	 *
	 * @return The right child
	 */
	public abstract BinTreeNode right() throws Exception;


	/**
	 * Gets the right memory handle that points to the right child of this node.
	 *
	 * @return The right memory handle.
	 */
	public abstract MemoryHandle rightHandle();

}

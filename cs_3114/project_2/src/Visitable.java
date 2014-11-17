/**
 * This interface is used to make sure that an object has the property of being
 * visitable. This interface is used for bin tree nodes, which facilitates the
 * visiting of all node when a traversal is occurring.
 * 
 * @author Elmer Landaverde
 * @version Oct 8, 2013
 */
public interface Visitable
{
	
	/**
	 * Visits the data element of a node.
	 */
	public void visit();
}

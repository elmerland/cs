/**
 * Implementation for a non-empty binary tree leaf node.
 * 
 * @param <E>
 *            The generic type.
 * @author Elmer Landaverde
 * @version Sep 30, 2013
 */
public class BLeafNode<E extends Visitable>
	implements BinTreeNode<E>
{
	
	/**
	 * The x coordinate of this leaf node.
	 */
	private double	x;
	
	/**
	 * The y coordinate of this leaf node.
	 */
	private double	y;
	
	/**
	 * The data element of this leaf node.
	 */
	private E		cargo;
	
	
	/**
	 * Create a new LeafNode object.
	 * 
	 * @param element
	 *            The data element of this leaf node.
	 * @param xCoordinate
	 *            The x coordinate of the data element.
	 * @param yCoordinate
	 *            The y coordinate of the data element.
	 */
	public BLeafNode(E element, double xCoordinate, double yCoordinate)
	{
		setX(xCoordinate);
		setY(yCoordinate);
		cargo = element;
	}
	
	
	@Override
	public E element()
	{
		return cargo;
	}
	
	
	/**
	 * Get the x coordinate of the data element.
	 * 
	 * @return the x
	 */
	public double getX()
	{
		return x;
	}
	
	
	/**
	 * Get the y coordinate of the data element.
	 * 
	 * @return the y
	 */
	public double getY()
	{
		return y;
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
		cargo = v;
	}
	
	
	/**
	 * Set the x coordinate of the data element.
	 * 
	 * @param x
	 *            the x to set
	 */
	public void setX(double x)
	{
		this.x = x;
	}
	
	
	/**
	 * Set the y coordinate of the data element.
	 * 
	 * @param y
	 *            the y to set
	 */
	public void setY(double y)
	{
		this.y = y;
	}
	
	
	/**
	 * Visits the data element of this node.
	 */
	public void visit()
	{
		// Visit data element.
		cargo.visit();
	}
}

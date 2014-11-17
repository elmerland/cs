/**
 * Implementation for a non-empty binary tree leaf node.
 *
 * @author Elmer Landaverde
 * @version Sep 30, 2013
 */
public class BLeafNode
	extends BinTreeNode
{

	/**
	 * The x coordinate of this leaf node.
	 */
	private double		x;

	/**
	 * The y coordinate of this leaf node.
	 */
	private double		y;

	/**
	 * The data element of this leaf node.
	 */
	private WatcherUser	cargo;


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
	public BLeafNode(WatcherUser element, double xCoordinate, double yCoordinate)
	{
		setX(xCoordinate);
		setY(yCoordinate);
		cargo = element;
	}


	@Override
	public WatcherUser element()
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
	 *
	 * @param k
	 *            The x and y coordinates of the search.
	 * @param radius
	 *            The radius of the search.
	 */
	public void visit(double k[], double radius)
	{
		// Visit data element.
		cargo.visit(k[0], k[1], radius);
	}
}

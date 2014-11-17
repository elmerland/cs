import java.awt.geom.Point2D;

/**
 * Represents the boundary space occupied by a bin tree node.
 *
 * @author Elmer Landaverde
 * @version Oct 7, 2013
 */
public class Rectangle
{

	/**
	 * The x coordinate of the upper left corner.
	 */
	public double	x;

	/**
	 * The y coordinate of the upper left corner.
	 */
	public double	y;

	/**
	 * The width of the rectangle.
	 */
	public double	width;

	/**
	 * The height of the rectangle.
	 */
	public double	height;


	/**
	 * Create a new Rectangle object.
	 *
	 * @param x
	 *            The x coordinate of the upper left corner.
	 * @param y
	 *            The y coordinate of the upper left corner.
	 * @param width
	 *            The width of the rectangle.
	 * @param height
	 *            The height of the rectangle.
	 */
	public Rectangle(double x, double y, double width, double height)
	{
		this.x = x;
		this.y = y;
		this.width = width;
		this.height = height;
	}


	/**
	 * Determines if the given point is contained within this rectangle.
	 *
	 * @param xPoint
	 *            The x coordinate of the point.
	 * @param yPoint
	 *            The y coordinate of the point.
	 * @return True if the point is contained within this rectangle without
	 *         touching the right or top boundaries. False otherwise.
	 */
	public boolean contains(double xPoint, double yPoint)
	{
		if (xPoint >= x && xPoint < (x + width))
		{
			if (yPoint < y && yPoint >= (y - height))
			{
				return true;
			}
		}
		return false;
	}


	/**
	 * Determines if this Rectangle is valid.
	 *
	 * @return Returns false if the height or width of the rectangle is less
	 *         than 0.5, otherwise it returns true.
	 */
	public boolean isValid()
	{
		if (width < 0.5 || height < 0.5)
		{
			return false;
		}
		else
		{
			return true;
		}
	}


	/**
	 * Determines if the given rectangle <i>rec</i> overlaps with this
	 * rectangle.
	 *
	 * @param rec
	 *            The rectangle to consider.
	 * @return True if <i>rec</i> overlaps with this rectangle. False otherwise.
	 */
	public boolean overlaps(Rectangle rec)
	{
		Point2D.Double p1 = new Point2D.Double(x, y);
		Point2D.Double p2 = new Point2D.Double(x + width, y - height);
		Point2D.Double p3 = new Point2D.Double(rec.x, rec.y);
		Point2D.Double p4 = new Point2D.Double(rec.x + rec.width, rec.y - rec.height);
		if (p2.y < p3.y && p1.y > p4.y && p2.x > p3.x && p1.x < p4.x)
		{
			return true;
		}
		return false;
	}


	/**
	 * Gets the left/right or bottom/top Rectangles from the r.
	 *
	 * @param r
	 *            The Rectangle from which to get the other two
	 *            Rectangles.
	 * @param discriminator
	 *            Determines if an horizontal split or a vertical split is
	 *            required.
	 * @return The left/right or bottom/top Rectangles inside of r.
	 */
	public static Rectangle[] getRectangles(Rectangle r, int discriminator)
	{
		Rectangle[] recs;
		// Determine what orientation the split is going to be in.
		if (discriminator == BinTree.xSplit)
		{
			// Split horizontally.
			recs = Rectangle.splitHorizontal(r);
		}
		else
		{
			// Split vertically.
			recs = Rectangle.splitVertical(r);
		}

		// Determine if the resulting rectangles are valid. Only the first
		// rectangle needs to be tested, if this rectangle is not valid then the
		// other will also not be valid and vice versa.
		if (!recs[0].isValid())
		{
			throw new NumberFormatException(
				"Rectangles generated are of invalid width or height:\n\t"
					+ printRec(recs[0]) + "\n\t" + printRec(recs[1]));
		}

		return recs;
	}


	/**
	 * Gets a string representation of a Rectangle object.
	 *
	 * @param rec
	 *            The Rectangle object.
	 * @return A string representation of the Rectangle object.
	 */
	public static String printRec(Rectangle rec)
	{
		return "(" + rec.x + ", " + rec.y + ") <" + rec.width + ", " + rec.height + ">";
	}


	/**
	 * Generates a new Rectangle that lies within r.
	 *
	 * @param r
	 *            The given Rectangle.
	 * @param horizontal
	 *            Determine if the Rectangle will be split horizontally or
	 *            vertically.
	 * @param leftBottom
	 *            Determines if the resulting Rectangle is the one on the
	 *            left/bottom, or right/top.
	 * @return A Rectangle the lies within r and is either the top/bottom
	 *         half, or left/right half.
	 */
	private static Rectangle getInnerRectangle(
		Rectangle r,
		boolean horizontal,
		boolean leftBottom)
	{
		// Instantiate the x and y coordinates, and the height and width of the
		// rectangle.
		double x, y, width, height;

		// Determine the orientation of the split.
		if (horizontal)
		{
			// Horizontal split.
			y = r.y;
			height = r.height;

			// Assign coordinates for x and width.
			if (leftBottom)
			{
				// Left split.
				x = r.x;
				width = r.width / 2;
			}
			else
			{
				// Right split.
				x = r.x + (r.width / 2);
				width = r.width - (r.width / 2);
			}
		}
		else
		{
			// Vertical split.
			x = r.x;
			width = r.width;

			// Assign coordinate for y and height.
			if (leftBottom)
			{
				// Bottom split.
				y = r.y - (r.height / 2);
				height = r.height - (r.height / 2);
			}
			else
			{
				// Top split.
				y = r.y;
				height = r.height / 2;
			}
		}

		return new Rectangle(x, y, width, height);
	}


	/**
	 * Generates two Rectangle object the occupy the entire space of r, with
	 * the first Rectangle occupying the left half, and the second
	 * Rectangle occupying the right half.
	 *
	 * @param r
	 *            The Rectangle to split.
	 * @return Two Rectangles, one occupying the left half, and the other the
	 *         right half of r.
	 */
	private static Rectangle[] splitHorizontal(Rectangle r)
	{
		Rectangle left = getInnerRectangle(r, true, true);
		Rectangle right = getInnerRectangle(r, true, false);
		Rectangle[] result = { left, right };
		return result;
	}


	/**
	 * Generates two Rectangle object the occupy the entire space of r, with
	 * the first Rectangle occupying the bottom half, and the second
	 * Rectangle occupying the top half.
	 *
	 * @param r
	 *            The Rectangle to split.
	 * @return Two Rectangles, one occupying the bottom half, and the other
	 *         the top half of r.
	 */
	private static Rectangle[] splitVertical(Rectangle r)
	{
		Rectangle bottom = getInnerRectangle(r, false, true);
		Rectangle top = getInnerRectangle(r, false, false);
		Rectangle[] result = { bottom, top };
		return result;
	}

}

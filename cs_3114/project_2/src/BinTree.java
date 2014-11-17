/**
 * Implementation of a bin tree.
 *
 * @author Elmer Landaverde
 * @version Sep 30, 2013
 * @param <E>
 *            The generic type.
 */
public class BinTree<E extends Visitable>
	implements SpatialDictionary<E>
{

	/**
	 * The bounding box for the entire bin tree.
	 */
	private Rectangle		worldBox;

	/**
	 * The root node for the bin tree.
	 */
	private BinTreeNode<E>	root;

	/**
	 * The number of non-empty nodes inside the bin tree.
	 */
	private int				nodeCount;

	/**
	 * The pointer to the flyweigth for the empty leaf node.
	 */
	private BEmptyNode<E>	empty;

	/**
	 * Variables used to keep track of all the node visited during the insert,
	 * remove, find, or regionSearch methods. This variables should be set to
	 * zero before each of the methods execute.
	 */
	private int				visitCount	= 0;

	/**
	 * Discriminator for horizontal split.
	 */
	public static final int	xSplit		= 0;


	/**
	 * Create a new BinTree object.
	 *
	 * @param box
	 *            The bounding box for the entire bin tree.
	 */
	public BinTree(Rectangle box)
	{
		// Set bounding box.
		worldBox = box;

		// Set root node.
		empty = new BEmptyNode<E>();
		root = empty;
	}


	@Override
	public void clear()
	{
		empty = new BEmptyNode<E>();
		root = empty;
	}


	@Override
	public E find(double[] k)
	{
		// Validate input
		if (k == null || k.length != 2)
		{
			throw new IllegalArgumentException("Point coordinate is invalid.");
		}
		// Check if point is valid.
		if (!worldBox.contains(k[0], k[1]))
		{
			throw new IllegalArgumentException(
				"Point lies outside of the bin tree boundaries. Box: "
					+ Rectangle.printRec(worldBox) + ", Point: (" + k[0] + ", " + k[1]
					+ ")");
		}
		// Find record.
		visitCount = 0;
		BinTreeNode<E> result = findHelper(root, worldBox, k, 0);
		if (result == null)
		{
			return null;
		}
		else
		{
			return result.element();
		}
	}


	/**
	 * Gets the number of nodes that were visited.
	 *
	 * @return The number of visited nodes.
	 */
	public int getNumberOfVisitedNodes()
	{
		return visitCount;
	}


	/**
	 * Gets the root node for this bin tree.
	 *
	 * @return The root node of this bin tree.
	 */
	public BinTreeNode<E> getRoot()
	{
		return root;
	}


	@Override
	public void insert(double[] k, E e)
	{
		// Check if coordinates are valid.
		if (k == null || k.length != 2)
		{
			throw new IllegalArgumentException("Point coordinate is invalid.");
		}
		// Check if point is valid.
		if (!worldBox.contains(k[0], k[1]))
		{
			throw new IllegalArgumentException(
				"Point lies outside of the bin tree boundaries. Box: "
					+ Rectangle.printRec(worldBox) + ", Point: (" + k[0] + ", " + k[1]
					+ ")");
		}
		if (find(k) != null)
		{
			return;
		}
		// Insert node.
		visitCount = 0;
		root = insertHelper(root, worldBox, k, 0, e);
		nodeCount++;
	}


	@Override
	public void regionSearch(double[] k, double rad)
	{

		// Get smallest rectangle that fits outside of circle.
		double xCoor = k[0] - rad;
		double yCoor = k[1] + rad;
		double width = rad * 2;
		double height = rad * 2;
		Rectangle region = new Rectangle(xCoor, yCoor, width, height);
		// Check if rectangle overlaps with world box.
		if (!worldBox.overlaps(region))
		{
			throw new IllegalArgumentException(
				"Region search is outside of the bounding box for the world.\nWorld: "
					+ Rectangle.printRec(worldBox) + "\nSearch region: "
					+ Rectangle.printRec(region));
		}
		visitCount = 0;
		regionSearchHelper(root, worldBox, region, 0);
	}


	@Override
	public E remove(double[] k)
	{
		// First find the node in the position.
		E element = find(k);
		if (element == null)
		{
			// Desired element is not in the tree.
			return null;
		}
		else
		{
			// Desired element is in the tree. Remove the matching node and
			// return it.
			root = removeHelper(root, worldBox, k, 0);
			nodeCount--;
			return element;
		}
	}


	@Override
	public E removeAny()
	{
		// TODO Auto-generated method stub
		return null;
	}


	@Override
	public int size()
	{
		return nodeCount;
	}


	@Override
	public String toString()
	{
		String result = printNode(root, worldBox, 0);
		if (!result.endsWith("\n"))
		{
			result += "\n";
		}
		if (result.startsWith("\n"))
		{
			result = result.substring(1);
		}
		result = result.replaceAll("\n\n", "\n");
		return result;
	}


	/**
	 * Gets a detailed string representation of the bin tree.
	 *
	 * @return A string representation of the bin tree.
	 */
	public String toStringDetailed()
	{
		return printNodeDetailed(root, worldBox, 0, 0);
	}


	/**
	 * This method finds a node in a the binary tree that has the same position
	 * as the one specified by <i>point</i>. If no such node exists it returns
	 * null.
	 *
	 * @param rt
	 *            The root node of the tree.
	 * @param box
	 *            The bounding rectangle of the root node.
	 * @param point
	 *            The position of the desired node.
	 * @param disc
	 *            The discriminator of the depth level.
	 * @return A binary tree node object corresponding to a node that has the
	 *         same position as <i>point</i>. Returns null if no such node was
	 *         found.
	 */
	private BinTreeNode<E> findHelper(
		BinTreeNode<E> rt,
		Rectangle box,
		double[] point,
		int disc)
	{
		if (rt instanceof BEmptyNode)
		{
			// Root node is empty.
			return null;
		}
		else if (rt instanceof BLeafNode)
		{
			// Root node is a leaf node.
			// Check if leaf node key is a match.
			double x = ((BLeafNode<E>)rt).getX();
			double y = ((BLeafNode<E>)rt).getY();
			if (x == point[0] && y == point[1])
			{
				// Found it!
				return rt;
			}
			else
			{
				// No match.
				return null;
			}
		}
		else
		{
			// Root node is an internal node.
			// Determine which child node contains the point.
			Rectangle[] recs = Rectangle.getRectangles(box, disc);
			int newDisc = DSutil.incDisc(disc);
			if (recs[0].contains(point[0], point[1]))
			{
				// Point falls inside left child. Look for point in left child.
				return findHelper(rt.left(), recs[0], point, newDisc);
			}
			else
			{
				// Point falls inside right child. Look for point in right
				// child.
				return findHelper(rt.right(), recs[1], point, newDisc);
			}
		}
	}


	/**
	 * This method inserts a new node to the bin tree.
	 *
	 * @param rt
	 *            The root of the bin tree where the new node is to be inserted.
	 * @param box
	 *            The bounding box of the root node.
	 * @param point
	 *            The position of the new node.
	 * @param disc
	 *            The discriminator of the tree depth.
	 * @param element
	 *            The data element of the new node.
	 * @return The root node of the original binary tree with the newly added
	 *         node.
	 */
	private BinTreeNode<E> insertHelper(
		BinTreeNode<E> rt,
		Rectangle box,
		double[] point,
		int disc,
		E element)
	{
		if (rt instanceof BEmptyNode)
		{
			// Root node is empty node.
			return insertToEmptyNode(box, point, element);
		}
		else if (rt instanceof BLeafNode)
		{
			// Root node is a leaf node.
			return insertToLeafNode((BLeafNode<E>)rt, box, point, disc, element);
		}
		else
		{
			// Root node is an internal node.
			return insertToInternalNode((BInternalNode<E>)rt, box, point, disc, element);
		}
	}


	/**
	 * Inserts a new node to an empty node. This method replaces the existing
	 * empty node and replaces it with a new leaf node.
	 *
	 * @param element
	 *            The data element of the new node.
	 * @param point
	 *            The position of the new node.
	 * @param box
	 *            The bounding box of the root empty node.
	 * @return A new leaf node containing the data element and position of the
	 *         new node.
	 */
	private BinTreeNode<E> insertToEmptyNode(Rectangle box, double[] point, E element)
	{
		// Root is an empty node.
		// Create new leaf node and return.
		return new BLeafNode<E>(element, point[0], point[1]);
	}


	/**
	 * Inserts a new node to an existing internal node. This method first
	 * determines in which of the two children of the internal node would the
	 * new node fall in. It then call on the insertHelp method to insert the new
	 * node to the appropriate child.
	 *
	 * @param internalRoot
	 *            The root bin internal node.
	 * @param box
	 *            The bounding box of the root node.
	 * @param point
	 *            The position of the new node.
	 * @param disc
	 *            The discriminator of the tree depth.
	 * @param element
	 *            The data element of the new node.
	 * @return The original internal node with the newly added node.
	 */
	private BinTreeNode<E> insertToInternalNode(
		BInternalNode<E> internalRoot,
		Rectangle box,
		double[] point,
		int disc,
		E element)
	{
		// Get Rectangles for child nodes.
		Rectangle[] recs = Rectangle.getRectangles(box, disc);
		// Determine where the new point falls.
		int newDiscriminator = DSutil.incDisc(disc);
		if (recs[0].contains(point[0], point[1]))
		{
			// Add new point to left child.
			internalRoot.setLeft(insertHelper(
				internalRoot.left(),
				recs[0],
				point,
				newDiscriminator,
				element));
		}
		else
		{
			// Add new point to right child.
			internalRoot.setRight(insertHelper(
				internalRoot.right(),
				recs[1],
				point,
				newDiscriminator,
				element));
		}
		return internalRoot;
	}


	/**
	 * Inserts a new node to an existing leaf node. This method replaces the
	 * original leaf node with an internal node. It then assigns to one of the
	 * children of the new internal node with the data from the original leaf
	 * node. Once this is done it determines in which child does the new node to
	 * be inserted would fall. It then calls on the insertHelp method to the
	 * insert the new node to either of the internal node's children.
	 *
	 * @param rt
	 *            The root bin leaf node.
	 * @param element
	 *            The data element of the new node.
	 * @param point
	 *            The position of the new node.
	 * @param box
	 *            The bounding box of the root leaf node.
	 * @param disc
	 *            The discriminator of the tree depth.
	 * @return An internal node containing the original leaf node and the newly
	 *         inserted node.
	 */
	private BinTreeNode<E> insertToLeafNode(
		BLeafNode<E> leafRoot,
		Rectangle box,
		double[] point,
		int disc,
		E element)
	{
		// Create new root
		BInternalNode<E> newInternal = null;
		// Create the two empty children of the new internal node.
		BinTreeNode<E> left = empty;
		BinTreeNode<E> right = empty;
		// Determine which rectangle the root point falls in.
		double rootX = leafRoot.getX();
		double rootY = leafRoot.getY();
		BLeafNode<E> newLeaf = new BLeafNode<E>(leafRoot.element(), rootX, rootY);
		Rectangle[] recs = Rectangle.getRectangles(box, disc);
		if (recs[0].contains(rootX, rootY))
		{
			// Root falls inside left child.
			left = newLeaf;
		}
		else
		{
			// Root falls inside right child.
			right = newLeaf;
		}
		// Initialize the new internal node.
		newInternal = new BInternalNode<E>(left, right);
		// Determine which rectangle the new point falls in.
		int newDisc = DSutil.incDisc(disc);
		if (recs[0].contains(point[0], point[1]))
		{
			// Add new node to the left child.
			newInternal.setLeft(insertHelper(
				newInternal.left(),
				recs[0],
				point,
				newDisc,
				element));
		}
		else
		{
			// Add new node to the right child.
			newInternal.setRight(insertHelper(
				newInternal.right(),
				recs[1],
				point,
				newDisc,
				element));
		}

		return newInternal;
	}


	/**
	 * Gets a less detailed string representation of a bin tree.
	 *
	 * @param rt
	 *            The root node of the bin tree.
	 * @param box
	 *            The bounding box of the root node.
	 * @param disc
	 *            The discriminator value for the tree depth.
	 * @return A string represenation of a bin tree.
	 */
	private String printNode(BinTreeNode<E> rt, Rectangle box, int disc)
	{
		if (rt instanceof BInternalNode)
		{
			// Root is internal node.
			// Get rectangles
			Rectangle[] recs = Rectangle.getRectangles(box, disc);
			int newDisc = DSutil.incDisc(disc);
			// Get string representation of left and right nodes.
			String left = printNode(rt.left(), recs[0], newDisc);
			String right = printNode(rt.right(), recs[1], newDisc);
			return "I\n" + left + right;
		}
		else if (rt instanceof BLeafNode)
		{
			// Root is leaf node.
			String elementInfo = rt.element().toString();
			return elementInfo + "\n";
		}
		else
		{
			// Root is empty node.
			return "E\n";
		}
	}


	/**
	 * Helper method that recursively prints every node in the tree when given
	 * the root node.
	 *
	 * @param node
	 *            The root node.
	 * @param box
	 *            The bounding box of the root node.
	 * @param depth
	 *            The depth of the root node.
	 * @param disc
	 *            The discriminator for the tree depth.
	 * @return A string representation of the bin tree.
	 */
	private String printNodeDetailed(
		BinTreeNode<E> node,
		Rectangle box,
		int depth,
		int disc)
	{
		// Get new discriminator for next depth level.
		int newDisc = DSutil.incDisc(disc);
		// Generate number of dashes to represent tree structure.
		String result = "";
		String dash = "";
		for (int i = 0; i < depth; i++)
		{
			dash += "--";
		}
		// Get message for each type of node.
		if (node instanceof BEmptyNode)
		{
			// Node is empty node.
			// Message for empty node.
			result += dash + "Empty: " + Rectangle.printRec(box) + "\n";
		}
		else if (node instanceof BLeafNode)
		{
			// Node is leaf node.
			// Message for leaf node.
			String nodePoint = DSutil.printPoint((BLeafNode<E>)node);
			String nodeElement = ((BLeafNode<E>)node).element().toString();
			result +=
				dash + "Leaf: " + Rectangle.printRec(box) + ", Point: " + nodePoint
					+ ", Element: " + nodeElement + "\n";
		}
		else
		{
			// Node is internal node.
			// Get rectangles of the two child nodes.
			Rectangle[] recs = Rectangle.getRectangles(box, disc);
			// Message for internal node.
			String temp = dash + "Internal: " + Rectangle.printRec(box) + "\n";
			// Message for left child node.
			String leftNode = printNodeDetailed(node.left(), recs[0], depth + 1, newDisc);
			// Message for right child node.
			String rightNode =
				printNodeDetailed(node.right(), recs[1], depth + 1, newDisc);

			result += rightNode + temp + leftNode;
		}
		return result;
	}


	/**
	 * Given the search region it visits any leaf node with a bounding box that
	 * overlaps with the search region bounding box.
	 *
	 * @param rt
	 *            The root node.
	 * @param box
	 *            The bounding box for the root node.
	 * @param region
	 *            The bounding box for the search region.
	 * @param disc
	 *            The discriminator for the tree depth.
	 */
	private void regionSearchHelper(
		BinTreeNode<E> rt,
		Rectangle box,
		Rectangle region,
		int disc)
	{
		if (rt instanceof BEmptyNode)
		{
			// Root node is an empty node.
			visitCount++;
			return;
		}
		else if (rt instanceof BLeafNode)
		{
			// Root node is a leaf node.
			visitCount++;
			((BLeafNode<E>)rt).visit();
			return;
		}
		else
		{
			// Root node is an internal node.
			// Get rectangles
			visitCount++;
			Rectangle[] recs = Rectangle.getRectangles(box, disc);
			int newDisc = DSutil.incDisc(disc);
			if (recs[0].overlaps(region))
			{
				// Overlaps with left child.
				regionSearchHelper(rt.left(), recs[0], region, newDisc);
			}
			if (recs[1].overlaps(region))
			{
				// Overlaps with right child.
				regionSearchHelper(rt.right(), recs[1], region, newDisc);
			}
		}
	}


	/**
	 * Removes a node with the same position as <i>point</i>. This method
	 * iterates through the entire tree until a matching node is found. When a
	 * node is found the leaf node is replaced by an empty node and any empty
	 * internal nodes are also replaced with empty nodes. The root of the tree
	 * with the deleted nodes is returned. If a matching node is not found then
	 * the original tree is returned.
	 *
	 * @param rt
	 *            The root node of the tree.
	 * @param box
	 *            The bounding box of the root node.
	 * @param point
	 *            The position of the node to find.
	 * @param disc
	 *            The discriminator for the tree depth.
	 * @return The root node of a bin tree with the node matching the position
	 *         <i>point</i> removed. If not such node is found, the original
	 *         tree is returned.
	 */
	private BinTreeNode<E> removeHelper(
		BinTreeNode<E> rt,
		Rectangle box,
		double[] point,
		int disc)
	{
		if (rt instanceof BLeafNode)
		{
			// Root node is a leaf node.
			return empty;
		}
		else
		{
			// Root node is an internal node.
			BInternalNode<E> internal = (BInternalNode<E>)rt;
			// Determine which child node contains the point.
			Rectangle[] recs = Rectangle.getRectangles(box, disc);
			int newDisc = DSutil.incDisc(disc);
			if (recs[0].contains(point[0], point[1]))
			{
				// Point falls inside left child. Look for point in left child.
				internal.setLeft(removeHelper(rt.left(), recs[0], point, newDisc));
			}
			else
			{
				// Point falls inside right child. Look for point in right
				// child.
				internal.setRight(removeHelper(rt.right(), recs[1], point, newDisc));
			}
			// Determine if internal node has only one node.
			if (internal.hasOneLeaf())
			{
				// If the internal node has only one leaf node, return the leaf
				// node.
				return internal.getNonEmptyChild();
			}
			else
			{
				// Return the internal node, if the internal node is not empty.
				return rt;
			}
		}
	}

}

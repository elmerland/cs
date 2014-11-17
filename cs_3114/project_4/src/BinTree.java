/**
 * Implementation of a bin tree.
 *
 * @author Elmer Landaverde
 * @version Sep 30, 2013
 */
public class BinTree
	implements SpatialDictionary
{

	/**
	 * The bounding box for the entire bin tree.
	 */
	private Rectangle				worldBox;

	/**
	 * The root node for the bin tree.
	 */
	private MemoryHandle			root;

	/**
	 * The number of non-empty nodes inside the bin tree.
	 */
	private int						nodeCount;

	/**
	 * The pointer to the flyweigth for the empty leaf node.
	 */
	public static MemoryHandle		empty;

	/**
	 * Empty node flyweigth.
	 */
	public static final BEmptyNode	emptyNode	= new BEmptyNode();

	/**
	 * Variables used to keep track of all the node visited during the insert,
	 * remove, find, or regionSearch methods. This variables should be set to
	 * zero before each of the methods execute.
	 */
	private int						visitCount	= 0;

	/**
	 * Discriminator for horizontal split.
	 */
	public static final int			xSplit		= 0;


	/**
	 * Create a new BinTree object.
	 *
	 * @param box
	 *            The bounding box for the entire bin tree.
	 * @throws Exception
	 *             Could not access disk.
	 */
	public BinTree(Rectangle box)
		throws Exception
	{
		// Set bounding box.
		worldBox = box;

		// Set empty node.
		empty = Serialize.storeNodeToDisk(emptyNode);

		// Set root node.
		root = empty;
	}


	@Override
	public void clear()
	{
		// Empty method.
	}


	@Override
	public WatcherUser find(double[] k)
		throws Exception
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
		MemoryHandle result = findHelper(root, worldBox, k, 0);
		if (result == null)
		{
			return null;
		}
		else
		{
			BLeafNode resultNode = (BLeafNode)Serialize.getNodeFromDisk(result);
			return resultNode.element();
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
	public MemoryHandle getRoot()
	{
		return root;
	}


	@Override
	public boolean insert(double[] k, WatcherUser e)
		throws Exception
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
			return false;
		}
		// Insert node.
		visitCount = 0;
		root = insertHelper(root, worldBox, k, 0, e);
		nodeCount++;
		return true;
	}


	@Override
	public void regionSearch(double[] k, double rad)
		throws Exception
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
		regionSearchHelper(root, worldBox, region, 0, k, rad);
	}


	@Override
	public WatcherUser remove(double[] k)
		throws Exception
	{
		// First find the node in the position.
		WatcherUser element = find(k);
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
	public WatcherUser removeAny()
	{
		return null;
	}


	@Override
	public int size()
	{
		return nodeCount;
	}


	/**
	 * Prints a string representation of this bin tree.
	 *
	 * @return The string representation of this bin tree.
	 * @throws Exception
	 *             Could not generate string.
	 */
	public String debugString()
		throws Exception
	{
		String result;
		result = printNode(root, worldBox, 0);
		result = result.replaceAll("\n\n", "\n");
		return result;
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
	private MemoryHandle findHelper(
		MemoryHandle rt,
		Rectangle box,
		double[] point,
		int disc)
		throws Exception
	{
		BinTreeNode rootNode = Serialize.getNodeFromDisk(rt);
		if (rootNode instanceof BEmptyNode)
		{
			// Root node is empty.
			return null;
		}
		else if (rootNode instanceof BLeafNode)
		{
			// Root node is a leaf node.
			// Check if leaf node key is a match.
			double x = ((BLeafNode)rootNode).getX();
			double y = ((BLeafNode)rootNode).getY();
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
				return findHelper(rootNode.leftHandle(), recs[0], point, newDisc);
			}
			else
			{
				// Point falls inside right child. Look for point in right
				// child.
				return findHelper(rootNode.rightHandle(), recs[1], point, newDisc);
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
	private MemoryHandle insertHelper(
		MemoryHandle rt,
		Rectangle box,
		double[] point,
		int disc,
		WatcherUser element)
		throws Exception
	{
		BinTreeNode rootNode = Serialize.getNodeFromDisk(rt);
		if (rootNode instanceof BEmptyNode)
		{
			// Root node is empty node.
			return insertToEmptyNode(box, point, element);
		}
		else if (rootNode instanceof BLeafNode)
		{
			// Root node is a leaf node.
			return insertToLeafNode(rt, box, point, disc, element);
		}
		else
		{
			// Root node is an internal node.
			return insertToInternalNode(rt, box, point, disc, element);
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
	private MemoryHandle insertToEmptyNode(
		Rectangle box,
		double[] point,
		WatcherUser element)
		throws Exception
	{
		// Root is an empty node.
		// Create new leaf node and return.
		MemoryHandle handle =
			Serialize.storeNodeToDisk(new BLeafNode(element, point[0], point[1]));
		return handle;
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
	private MemoryHandle insertToInternalNode(
		MemoryHandle internalRoot,
		Rectangle box,
		double[] point,
		int disc,
		WatcherUser element)
		throws Exception
	{
		// Get Rectangles for child nodes.
		Rectangle[] recs = Rectangle.getRectangles(box, disc);
		// Determine where the new point falls.
		int newDiscriminator = DSutil.incDisc(disc);
		BInternalNode internalRootNode =
			(BInternalNode)Serialize.getNodeFromDisk(internalRoot);
		if (recs[0].contains(point[0], point[1]))
		{
			// Add new point to left child.
			internalRootNode.setLeft(insertHelper(
				internalRootNode.leftHandle(),
				recs[0],
				point,
				newDiscriminator,
				element));
		}
		else
		{
			// Add new point to right child.
			internalRootNode.setRight(insertHelper(
				internalRootNode.rightHandle(),
				recs[1],
				point,
				newDiscriminator,
				element));
		}

		// Release original internal node.
		Serialize.freeNode(internalRoot);
		// Write new internal node to disk.
		MemoryHandle resultingInternalNode = Serialize.storeNodeToDisk(internalRootNode);
		return resultingInternalNode;
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
	private MemoryHandle insertToLeafNode(
		MemoryHandle leafRoot,
		Rectangle box,
		double[] point,
		int disc,
		WatcherUser element)
		throws Exception
	{
		// Create new root
		BInternalNode newInternal = null;
		// Create the two empty children of the new internal node.
		MemoryHandle left = empty;
		MemoryHandle right = empty;
		// Determine which rectangle the root point falls in.
		BLeafNode leafRootNode = (BLeafNode)Serialize.getNodeFromDisk(leafRoot);
		double rootX = leafRootNode.getX();
		double rootY = leafRootNode.getY();
		Rectangle[] recs = Rectangle.getRectangles(box, disc);
		if (recs[0].contains(rootX, rootY))
		{
			// Root falls inside left child.
			left = leafRoot;
		}
		else
		{
			// Root falls inside right child.
			right = leafRoot;
		}
		// Initialize the new internal node.
		newInternal = new BInternalNode(left, right);
		// Determine which rectangle the new point falls in.
		int newDisc = DSutil.incDisc(disc);
		if (recs[0].contains(point[0], point[1]))
		{
			// Add new node to the left child.
			newInternal.setLeft(insertHelper(
				newInternal.leftHandle(),
				recs[0],
				point,
				newDisc,
				element));
		}
		else
		{
			// Add new node to the right child.
			newInternal.setRight(insertHelper(
				newInternal.rightHandle(),
				recs[1],
				point,
				newDisc,
				element));
		}
		MemoryHandle newInternalHandle = Serialize.storeNodeToDisk(newInternal);
		return newInternalHandle;
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
	private String printNode(MemoryHandle rt, Rectangle box, int disc)
		throws Exception
	{
		BinTreeNode rootNode = Serialize.getNodeFromDisk(rt);
		if (rootNode instanceof BInternalNode)
		{
			// Root is internal node.
			// Get rectangles
			Rectangle[] recs = Rectangle.getRectangles(box, disc);
			int newDisc = DSutil.incDisc(disc);
			// Get string representation of left and right nodes.
			String left = printNode(rootNode.leftHandle(), recs[0], newDisc);
			String right = printNode(rootNode.rightHandle(), recs[1], newDisc);
			return "I\n" + left + right;
		}
		else if (rootNode instanceof BLeafNode)
		{
			// Root is leaf node.
			String elementInfo = rootNode.element().toString();
			return elementInfo + "\n";
		}
		else
		{
			// Root is empty node.
			return "E\n";
		}
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
		MemoryHandle rt,
		Rectangle box,
		Rectangle region,
		int disc,
		double[] k,
		double rad)
		throws Exception
	{
		BinTreeNode rootNode = Serialize.getNodeFromDisk(rt);
		if (rootNode instanceof BEmptyNode)
		{
			// Root node is an empty node.
			visitCount++;
			return;
		}
		else if (rootNode instanceof BLeafNode)
		{
			// Root node is a leaf node.
			visitCount++;
			((BLeafNode)rootNode).visit(k, rad);
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
				regionSearchHelper(
					rootNode.leftHandle(),
					recs[0],
					region,
					newDisc,
					k,
					rad);
			}
			if (recs[1].overlaps(region))
			{
				// Overlaps with right child.
				regionSearchHelper(
					rootNode.rightHandle(),
					recs[1],
					region,
					newDisc,
					k,
					rad);
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
	private MemoryHandle removeHelper(
		MemoryHandle rt,
		Rectangle box,
		double[] point,
		int disc)
		throws Exception
	{
		BinTreeNode rootNode = Serialize.getNodeFromDisk(rt);
		if (rootNode instanceof BLeafNode)
		{
			// Root node is a leaf node.
			// Release leaf node.
			Serialize.freeNode(rt);
			return empty;
		}
		else
		{
			// Root node is an internal node.
			BInternalNode internal = (BInternalNode)rootNode;
			// Determine which child node contains the point.
			Rectangle[] recs = Rectangle.getRectangles(box, disc);
			int newDisc = DSutil.incDisc(disc);
			if (recs[0].contains(point[0], point[1]))
			{
				// Point falls inside left child. Look for point in left child.
				internal.setLeft(removeHelper(
					rootNode.leftHandle(),
					recs[0],
					point,
					newDisc));
			}
			else
			{
				// Point falls inside right child. Look for point in right
				// child.
				internal.setRight(removeHelper(
					rootNode.rightHandle(),
					recs[1],
					point,
					newDisc));
			}
			// Determine if internal node has only one node.
			if (internal.hasOneLeaf())
			{
				// Get the non empty child.
				MemoryHandle child = internal.getNonEmptyChild();
				// Release internal node.
				Serialize.freeNode(rt);
				// Return memory handle to non empty child.
				return child;
			}
			else
			{
				// Release original internal node.
				Serialize.freeNode(rt);
				// Write new internal node to disk.
				MemoryHandle newInternal = Serialize.storeNodeToDisk(internal);
				// Return the internal node, if the internal node is not empty.
				return newInternal;
			}
		}
	}

}

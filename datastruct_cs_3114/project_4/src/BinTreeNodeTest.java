import junit.framework.TestCase;

/**
 * Test cases for the bin tree node classes.
 *
 * @author Elmer Landaverde
 * @version Dec 6, 2013
 */
public class BinTreeNodeTest
	extends TestCase
{

	/**
	 * Test for the BEmptyNode class.
	 */
	public void testEmpty()
	{
		BEmptyNode en = new BEmptyNode();
		en.setElement(null);
		assertNull(en.element());
		assertNull(en.left());
		assertNull(en.right());
		assertTrue(en.isLeaf());
		assertNull(en.leftHandle());
		assertNull(en.rightHandle());
	}


	/**
	 * Test for the BInternalNode class.
	 */
	public void testInternal()
	{
		BInternalNode in = new BInternalNode(null, null);
		assertFalse(in.isLeaf());
		in.setElement(null);
		assertNull(in.element());
	}


	/**
	 * Test for the BLeafNode class
	 */
	public void testLeaf()
	{
		BLeafNode ln = new BLeafNode(null, 0, 0);
		assertNull(ln.left());
		assertNull(ln.leftHandle());
		assertNull(ln.right());
		assertNull(ln.rightHandle());
		assertTrue(ln.isLeaf());
		ln.setElement(null);
		assertNull(ln.element());
	}

}

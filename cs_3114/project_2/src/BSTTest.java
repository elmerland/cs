/**
 * Source code example for "A Practical Introduction to Data Structures and
 * Algorithm Analysis, 3rd Edition (Java)" by Clifford A. Shaffer Copyright
 * 2008-2011 by Clifford A. Shaffer
 */

/**
 * A JUnit test class for the BST.
 * 
 * @author Virginia Tech
 * @version Sep 28, 2013
 */
public class BSTTest
	extends junit.framework.TestCase
{
	
	private Dictionary<Integer, Integer>	D1;
	
	
	public void setUp()
	{
		D1 = new BST<Integer, Integer>();
	}
	
	
	/**
	 * Test case for a Binary Search Tree.
	 */
	public void testBST1()
	{
		assertEquals(D1.size(), 0);
		D1.insert(50, 50);
		assertEquals(D1.size(), 1);
		D1.remove(20);
		assertEquals(D1.size(), 1);
		D1.remove(50);
		assertEquals(D1.size(), 0);
		D1.clear();
		assertEquals(D1.size(), 0);
		D1.insert(40, 40);
		D1.insert(20, 20);
		D1.insert(30, 30);
		assertEquals(D1.size(), 3);
		D1.clear();
		assertEquals(D1.size(), 0);
	}
	
	
	/**
	 * Test case for a Binary Search Tree.
	 */
	public void testBST2()
	{
		D1.insert(70, 70);
		D1.insert(35, 35);
		D1.insert(20, 20);
		D1.insert(17, 17);
		D1.insert(15, 15);
		D1.insert(19, 19);
		D1.insert(100, 100);
		D1.insert(90, 90);
		D1.insert(95, 95);
		D1.insert(1, 1);
		assertEquals(D1.size(), 10);
		D1.insert(1, 1);
		assertNotNull(D1.toString());
		assertEquals(D1.size(), 11);
		assertEquals(D1.find(99), null);
		assertEquals(D1.find(100), new Integer(100));
		assertEquals(D1.find(15), new Integer(15));
		assertEquals(D1.find(70), new Integer(70));
		assertEquals(D1.remove(15), new Integer(15));
		assertEquals(D1.size(), 10);
		assertEquals(D1.remove(33), null);
		assertEquals(D1.size(), 10);
		assertEquals(D1.remove(70), new Integer(70));
		assertEquals(D1.size(), 9);
		D1.clear();
		assertEquals(D1.size(), 0);
	}
	
	
	/**
	 * More tests for binary tree.
	 */
	public void testBST3()
	{
		D1.insert(70, 70);
		D1.insert(35, 35);
		D1.insert(20, 20);
		D1.insert(17, 17);
		D1.insert(15, 15);
		D1.insert(19, 19);
		D1.insert(100, 100);
		D1.insert(90, 90);
		D1.insert(95, 95);
		D1.insert(1, 1);
		D1.insert(1, 1);
		
		assertNull(D1.find(null));
		
		while (D1.size() > 0)
		{
			assertNotNull(D1.removeAny());
		}
		
		assertNull(D1.find(1));
		
		D1.insert(70, 70);
		D1.insert(35, 35);
		D1.insert(20, 20);
		D1.insert(17, 17);
		D1.insert(15, 15);
		D1.insert(19, 19);
		D1.insert(100, 100);
		D1.insert(90, 90);
		D1.insert(95, 95);
		D1.insert(1, 1);
		D1.insert(1, 1);
		
		BSTNode<Integer, Integer> n = new BSTNode<Integer, Integer>(10, null, null, null);
		assertTrue(n.isLeaf());
		n.setLeft(n);
		assertFalse(n.isLeaf());
		n.setLeft(null);
		n.setRight(n);
		assertFalse(n.isLeaf());
	}
}

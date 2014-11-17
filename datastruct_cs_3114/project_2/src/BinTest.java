import junit.framework.TestCase;

/**
 * Test case for the all the classes related to the bin tree.
 * 
 * @author Elmer Landaverde
 * @version Oct 10, 2013
 */
public class BinTest
	extends TestCase
{
	
	private WatcherUser				wu;
	
	private BEmptyNode<WatcherUser>	empty;
	
	private BLeafNode<WatcherUser>	leaf;
	
	private Rectangle				world;
	
	private double[]				outOfWorld;
	
	private double[]				inWorld;
	
	private double[]				inWorld2;
	
	private WatcherUser[]			users;
	
	private String[]				names		= { "Riley", "Taylor", "Nevaeh",
		"Dominic", "Taylor", "Tristan", "Sean", "Alexa", "John", "Penny" };
	
	private double[][]				coordinates	= { { -105, -24 }, { 21, -38 },
		{ -11, 63 }, { -79, 37 }, { -125, -38 }, { -117, 5 }, { -105, -24 }, { -50, 88 },
		{ 10, -80 }, { 55, -80 }				};
	
	
	/*
	 * add -105 -24 Riley add 21 -38 Taylor add -11 63 Nevaeh add -79 37 Dominic
	 * add -125 -38 Taylor add -117 5 Tristan add -105 -24 Sean add -50 88 Alexa
	 * add 10 -80 John add 55 -80 Penny
	 */
	
	/**
	 * Test case for BEmptyNode class.
	 */
	public void testBEmptyNode()
	{
		BEmptyNode<WatcherUser> e = new BEmptyNode<WatcherUser>();
		assertNull(e.element());
		assertNull(e.left());
		assertNull(e.right());
		e.setElement(wu);
		assertNull(e.element());
		assertTrue(e.isLeaf());
	}
	
	
	/**
	 * Test case for BInternalNode.
	 */
	public void testBInternalNode()
	{
		BInternalNode<WatcherUser> i = new BInternalNode<WatcherUser>(empty, empty);
		i.setElement(wu);
		assertNull(i.element());
		assertEquals(i.left(), empty);
		assertEquals(i.right(), empty);
		assertFalse(i.isLeaf());
		assertNull(i.getNonEmptyChild());
		assertFalse(i.hasOneLeaf());
		i.setLeft(leaf);
		i.setRight(empty);
		assertTrue(i.hasOneLeaf());
		assertNotNull(i.getNonEmptyChild());
		i.setLeft(empty);
		i.setRight(leaf);
		assertTrue(i.hasOneLeaf());
		assertNotNull(i.getNonEmptyChild());
	}
	
	
	/**
	 * Test case for BinTree.
	 */
	public void testBinTree()
	{
		BinTree<WatcherUser> bt = new BinTree<WatcherUser>(world);
		bt.clear();
		assertTrue(bt.getRoot() instanceof BEmptyNode);
		assertEquals(bt.toString(), "E\n");
		assertNotNull(bt.toStringDetailed());
		
		testInsert(bt);
		
		testFind(bt);
		
		testRegionSearch(bt);
		
		testRemove(bt);
	}
	
	
	/**
	 * Test case for BLeafNode.
	 */
	public void testBLeafNode()
	{
		BLeafNode<WatcherUser> l = new BLeafNode<WatcherUser>(null, 10.0, 11.0);
		assertEquals(l.getX(), 10.0);
		assertEquals(l.getY(), 11.0);
		assertNull(l.element());
		assertTrue(l.isLeaf());
		assertNull(l.left());
		assertNull(l.right());
		
		l.setElement(wu);
		assertEquals(l.element(), wu);
		l.visit();
	}
	
	
	private void testFind(BinTree<WatcherUser> bt)
	{
		boolean pass = true;
		try
		{
			bt.find(null);
			pass = false;
		}
		catch (Exception e)
		{
			assertNotNull(e);
		}
		try
		{
			double[] k = { 10, 10, 10 };
			bt.find(k);
			pass = false;
		}
		catch (Exception e)
		{
			assertNotNull(e);
		}
		try
		{
			bt.find(outOfWorld);
			pass = false;
		}
		catch (Exception e)
		{
			assertNotNull(e);
		}
		
		assertTrue(pass);
		
		BinTree<WatcherUser> bt2 = new BinTree<WatcherUser>(world);
		bt2.insert(wu.getCoordinates(), wu);
		double[] k = { 10 + 180, 10 + 90 };
		assertNull(bt2.find(k));
		
		k[0] = 23.0 + 180;
		assertNull(bt2.find(k));
		
		k[0] = 10 + 180;
		k[1] = 36.0 + 90;
		assertNull(bt2.find(k));
	}
	
	
	private void testInsert(BinTree<WatcherUser> bt)
	{
		boolean pass = true;
		try
		{
			// Null element.
			bt.insert(outOfWorld, null);
			pass = false;
		}
		catch (Exception e)
		{
			assertNotNull(e);
		}
		try
		{
			// Null point.
			bt.insert(null, null);
			pass = false;
		}
		catch (Exception e)
		{
			assertNotNull(e);
		}
		try
		{
			// Wrong length.
			double[] k = { 10, 20, 40 };
			bt.insert(k, null);
			pass = false;
		}
		catch (Exception e)
		{
			assertNotNull(e);
		}
		assertTrue(pass);
		for (int i = 0; i < users.length; i++)
		{
			double[] k = { users[i].getLongitude(), users[i].getLatitude() };
			bt.insert(k, users[i]);
		}
		assertEquals(bt.size(), 9);
		assertNotNull(bt.toString());
		assertNotNull(bt.toStringDetailed());
	}
	
	
	private void testRegionSearch(BinTree<WatcherUser> bt)
	{
		boolean pass = true;
		try
		{
			double[] k = { 1000, 1000 };
			double radius = 10;
			bt.regionSearch(k, radius);
			pass = false;
		}
		catch (Exception e)
		{
			assertNotNull(e);
		}
		
		double[] k = { 180, 90 };
		bt.regionSearch(k, 180);
		assertTrue(bt.getNumberOfVisitedNodes() > 0);
		
		assertTrue(pass);
	}
	
	
	private void testRemove(BinTree<WatcherUser> bt)
	{
		boolean pass = true;
		try
		{
			bt.remove(null);
			pass = false;
		}
		catch (Exception e)
		{
			assertNotNull(e);
		}
		assertTrue(pass);
		
		System.out.println(bt.toStringDetailed());
		
		for (int i = 0; i < users.length; i++)
		{
			if (i == 6)
			{
				continue;
			}
			assertNotNull(bt.remove(users[i].getCoordinates()));
		}
		
		System.out.println(bt.toStringDetailed());
		
		assertNull(bt.removeAny());
	}
	
	
	/**
	 * Set up variables for global use.
	 */
	protected void setUp()
		throws Exception
	{
		super.setUp();
		wu = new WatcherUser("John", 23.0, 36.0);
		empty = new BEmptyNode<WatcherUser>();
		leaf = new BLeafNode<WatcherUser>(wu, 23.0, 36.0);
		world = new Rectangle(0, 180, 360, 180);
		outOfWorld = new double[2];
		outOfWorld[0] = 300.0;
		outOfWorld[1] = 300.0;
		inWorld = new double[2];
		inWorld[0] = 45.0;
		inWorld[1] = 45.0;
		inWorld2 = new double[2];
		inWorld2[0] = 15.0;
		inWorld2[1] = 15.0;
		users = new WatcherUser[names.length];
		for (int i = 0; i < users.length; i++)
		{
			users[i] = new WatcherUser(names[i], coordinates[i][0], coordinates[i][1]);
		}
	}
}

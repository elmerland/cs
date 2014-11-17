import junit.framework.TestCase;

/**
 * Test case for WatcherManager class.
 * 
 * @author Elmer Landaverde
 * @version Oct 10, 2013
 */
public class WatcherManagerTest
	extends TestCase
{
	
	private String[]	names		= { "Riley", "Taylor", "Nevaeh", "Dominic", "Taylor",
		"Tristan", "Sean", "Alexa", "John", "Penny", "Elmer" };
	
	private double[][]	coordinates	= { { -105, -24 }, { 21, -38 }, { -11, 63 },
		{ -79, 37 }, { -125, -38 }, { -117, 5 }, { -105, -24 }, { -50, 88 }, { 10, -80 },
		{ 55, -80 }, { 10, 101 }	};
	
	
	/*
	 * add -105 -24 Riley add 21 -38 Taylor add -11 63 Nevaeh add -79 37 Dominic
	 * add -125 -38 Taylor add -117 5 Tristan add -105 -24 Sean add -50 88 Alexa
	 * add 10 -80 John add 55 -80 Penny
	 */
	
	/**
	 * Test case for addUser method.
	 */
	public void testAddUser()
	{
		EqSpatial.createWatcherHandlerForTesting();
		WatcherManager wm = EqSpatial.getWatcherManager();
		
		for (int i = 0; i < names.length; i++)
		{
			wm.addUser(names[i], coordinates[i][0], coordinates[i][1]);
		}
		
		assertEquals(wm.getUserCount(), 8);
	}
	
	
	/**
	 * Set up variables for global use.
	 */
	protected void setUp()
		throws Exception
	{
		super.setUp();
	}
	
}

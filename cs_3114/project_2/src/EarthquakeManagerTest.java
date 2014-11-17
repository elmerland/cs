import junit.framework.TestCase;

/**
 * Test case for the EarthquakeManager class.
 * 
 * @author Elmer Landaverde
 * @version Oct 11, 2013
 */
public class EarthquakeManagerTest
	extends TestCase
{
	
	/**
	 * Test case for the queryEarthquakes method.
	 */
	public void testQueryEarthquakes()
	{
		String[] args = { "watcher.txt", "violent.earthquakes.json" };
		try
		{
			EqSpatial.main(args);
		}
		catch (Exception e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		assertTrue(true);
	}
	
	
	protected void setUp()
		throws Exception
	{
		super.setUp();
	}
	
}

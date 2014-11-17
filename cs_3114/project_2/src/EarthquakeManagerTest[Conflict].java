import junit.framework.TestCase;



/**
 *  Test case for the EarthquakeManager class.
 *
 *  @author Elmer Landaverde
 *  @version Oct 11, 2013
 */
public class EarthquakeManagerTest
	extends TestCase
{

	private EarthquakeManager em;

	protected void setUp()
		throws Exception
	{
		super.setUp();
		em = new EarthquakeManager();
	}

	/**
	 * Test case for the queryEarthquakes method.
	 */
	public void testQueryEarthquakes() {
		em.queryEarthquakes();
		assertNotNull(systemOut().getHistory());
	}

}

import junit.framework.TestCase;

/**
 * Test case for EqSpatial class.
 * 
 * @author Elmer Landaverde
 * @version Oct 10, 2013
 */
public class EqSpatialTest
	extends TestCase
{
	
	/**
	 * Test case for the constructor.
	 */
	public void testConstructor()
	{
		EqSpatial qs = new EqSpatial();
		assertNotNull(qs);
	}
	
	
	/**
	 * Test case for main method.
	 */
	public void testMain()
	{
		
		// Test invalid inputs
		try
		{
			EqSpatial.createWatcherHandlerForTesting();
			assertNotNull(EqSpatial.getWatcherManager());
			
			EqSpatial.testingFlag = true;
			EqSpatial.main(null);
			assertTrue(EqSpatial.inputError);
			
			String[] lonStr = { "Hello", "World", "World" };
			EqSpatial.testingFlag = true;
			EqSpatial.main(lonStr);
			assertTrue(EqSpatial.inputError);
			
			String[] inputStr = { "unknown", "unknown" };
			EqSpatial.testingFlag = true;
			EqSpatial.main(inputStr);
			assertTrue(EqSpatial.inputError);
			EqSpatial.inputError = false;
			
			inputStr[0] = "Wsmall2.txt";
			EqSpatial.main(inputStr);
			assertTrue(EqSpatial.inputError);
			EqSpatial.inputError = false;
			
			inputStr[0] = "unknown";
			inputStr[1] = "EQsmallP2.json";
			EqSpatial.main(inputStr);
			assertTrue(EqSpatial.inputError);
			EqSpatial.inputError = false;
			
			inputStr[0] = "unknown";
			inputStr[1] = "live";
			EqSpatial.main(inputStr);
			assertTrue(EqSpatial.inputError);
			EqSpatial.inputError = false;
			
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
		
		// Test valid inputs
		try
		{
			String[] inputStr = { "Wsmall2.txt", "EQsmallP2.json" };
			EqSpatial.testingFlag = true;
			EqSpatial.main(inputStr);
			assertFalse(EqSpatial.inputError);
			assertTrue(EqSpatial.getOffLineFlag());
			assertEquals(EqSpatial.getEarthquakesFile(), "EQsmallP2.json");
			assertEquals(EqSpatial.getWatchersFile(), "Wsmall2.txt");
			assertNotNull(EqSpatial.getEarthquakeManager());
			assertNotNull(EqSpatial.getWatchersFile());
			EqSpatial.inputError = false;
			
			inputStr[1] = "live";
			EqSpatial.testingFlag = true;
			EqSpatial.main(inputStr);
			assertFalse(EqSpatial.inputError);
			assertFalse(EqSpatial.getOffLineFlag());
			assertNull(EqSpatial.getEarthquakesFile());
			assertEquals(EqSpatial.getWatchersFile(), "Wsmall2.txt");
			assertNotNull(EqSpatial.getEarthquakeManager());
			assertNotNull(EqSpatial.getWatchersFile());
			EqSpatial.inputError = false;
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
	}
	
}

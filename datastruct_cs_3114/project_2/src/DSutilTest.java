import junit.framework.TestCase;

/**
 * Test case for DSutil class.
 * 
 * @author Elmer Landaverde
 * @version Oct 11, 2013
 */
public class DSutilTest
	extends TestCase
{
	
	/**
	 * Test case for the getRectangles method.
	 */
	public void testGetRectangles()
	{
		new DSutil<String>();
		
		boolean pass = true;
		try
		{
			Rectangle r = new Rectangle(0, 0, .9, .99);
			Rectangle.getRectangles(r, 0);
			pass = false;
		}
		catch (Exception e)
		{
			assertNotNull(e);
		}
		try
		{
			Rectangle r = new Rectangle(0, 0, 1, .9);
			Rectangle.getRectangles(r, 1);
			pass = false;
		}
		catch (Exception e)
		{
			assertNotNull(e);
		}
		try
		{
			Rectangle r = new Rectangle(0, 0, 1, 1);
			Rectangle.getRectangles(r, 0);
			assertTrue(pass);
		}
		catch (Exception e)
		{
			pass = false;
		}
		assertTrue(pass);
		
	}
	
	
	/**
	 * Test data printing methods.
	 */
	public void testPrint()
	{
		String date = DSutil.printDate(System.currentTimeMillis());
		assertNotNull(date);
		date = DSutil.printHours(System.currentTimeMillis());
		assertNotNull(date);
	}
	
	
	protected void setUp()
		throws Exception
	{
		super.setUp();
	}
	
}

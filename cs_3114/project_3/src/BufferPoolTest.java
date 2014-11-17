import junit.framework.TestCase;

/**
 * Test case for the BufferPool class.
 *
 * @author Elmer Landaverde
 * @version Nov 6, 2013
 */
public class BufferPoolTest
	extends TestCase
{

	private BufferPool	bp;


	protected void setUp()
		throws Exception
	{
		super.setUp();
		bp = new BufferPool(Util.aFile, 4);
	}


	/**
	 * Test case for the get and set methods.
	 */
	public void testBufferPool()
	{
		try
		{
			byte[] b1 = { -1, -1, -1, -1 };
			byte[] b2 = { 100, 100, 100, 100 };
			byte[] b3 = { 0, 0, 0, 0 };

			// Test valid input.
			assertTrue(bp.getBytes(b1, 0));
			assertNotSame(b1[0], -1);
			assertNotSame(b1[1], -1);
			assertNotSame(b1[2], -1);
			assertNotSame(b1[3], -1);

			assertTrue(bp.setBytes(b2, 0));
			assertTrue(bp.getBytes(b3, 0));
			assertEquals(b2[0], b3[0]);
			assertEquals(b2[1], b3[1]);
			assertEquals(b2[2], b3[2]);
			assertEquals(b2[3], b3[3]);

			// set bytes to initial state.
			assertTrue(bp.setBytes(b1, 0));

			// Test invalid inputs.
			assertFalse(bp.getBytes(b1, BufferPool.BUFFER_SIZE - 1));
			assertFalse(bp.setBytes(b1, BufferPool.BUFFER_SIZE - 1));

			assertFalse(bp.getBytes(b1, BufferPool.BUFFER_SIZE * 10 - 1));
			assertFalse(bp.setBytes(b1, BufferPool.BUFFER_SIZE * 10 - 1));

			assertEquals(bp.getNumberOfActiveBuffers(), 1);

			assertEquals(bp.getFileSize(), BufferPool.BUFFER_SIZE * 10);
		}
		catch (Exception e)
		{
			e.printStackTrace();
			fail();
		}

		try
		{
			bp.closeBufferPool();
		}
		catch (Exception e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
			fail();
		}
		assertEquals(bp.getNumberOfActiveBuffers(), 0);
	}


	protected void tearDown()
		throws Exception
	{
		super.tearDown();
	}

}

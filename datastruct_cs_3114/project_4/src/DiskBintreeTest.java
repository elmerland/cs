import junit.framework.TestCase;

/**
 * Test case for the main method of the program.
 *
 * @author Elmer Landaverde
 * @version Dec 6, 2013
 */
public class DiskBintreeTest
	extends TestCase
{

	/**
	 * Test for main method.
	 *
	 * @throws Exception
	 *             Error in testing.
	 */
	public void test()
		throws Exception
	{
		new DiskBintree();
		new DSutil();
		new Serialize();
		new StreamManager();
		DiskBintree.testing = true;
		String[] args = { "Comprehensive.txt", "10", "10" };
		DiskBintree.main(args);
		DiskBintree.getWatcherManager().getBinTree().clear();
		assertNotNull(DiskBintree.getWatcherManager().getBinTree().getRoot());
		assertNull(DiskBintree.getWatcherManager().getBinTree().removeAny());
		assertTrue(DiskBintree.getWatcherManager().getBinTree().size() > 0);
		assertFalse(DiskBintree.getWatcherManager().removeUser(10, 10));
		try
		{
			DiskBintree.getBufferPool().incFileSize(-1);
			fail();
		}
		catch (Exception e)
		{
			// Do nothing.
		}
		try
		{
			byte[] b = { 0, 0, 0 };
			DiskBintree.getBufferPool().setBytes(b, -1);
			fail();
		}
		catch (Exception e)
		{
			// Do nothing.
		}
		try
		{
			byte[] b = new byte[0];
			DiskBintree.getBufferPool().setBytes(b, 0);
			fail();
		}
		catch (Exception e)
		{
			// Do nothing.
		}
		try
		{
			byte[] b = { 0, 0, 0 };
			long pos = DiskBintree.getBufferPool().getFileSize();
			DiskBintree.getBufferPool().setBytes(b, pos);
			fail();
		}
		catch (Exception e)
		{
			// Do nothing.
		}
		try
		{
			byte[] b = { 0, 0, 0 };
			DiskBintree.getBufferPool().getBytes(b, -1);
			fail();
		}
		catch (Exception e)
		{
			// Do nothing.
		}
		try
		{
			DiskBintree.getWatcherManager().getBinTree().find(null);
			fail();
		}
		catch (Exception e)
		{
			// Do nothing.
		}
		try
		{
			double[] d = { 1, 2, 3 };
			DiskBintree.getWatcherManager().getBinTree().find(d);
			fail();
		}
		catch (Exception e)
		{
			// Do nothing.
		}
		try
		{
			double[] d = { 1000, 1000 };
			DiskBintree.getWatcherManager().getBinTree().find(d);
			fail();
		}
		catch (Exception e)
		{
			// Do nothing.
		}
		try
		{
			DiskBintree.getWatcherManager().getBinTree().insert(null, null);
			fail();
		}
		catch (Exception e)
		{
			// Do nothing.
		}
		try
		{
			double[] d = { 1, 2, 3 };
			DiskBintree.getWatcherManager().getBinTree().insert(d, null);
			fail();
		}
		catch (Exception e)
		{
			// Do nothing.
		}
		try
		{
			double[] d = { 1000, 1000 };
			DiskBintree.getWatcherManager().getBinTree().regionSearch(d, 10);
			;
			fail();
		}
		catch (Exception e)
		{
			// Do nothing.
		}
		DiskBintree.getBufferPool().closeBufferPool();

		// Test arguments
		DiskBintree.testing = false;
		String[] args2 = { "input.txt", "10", "10" };
		DiskBintree.main(args2);

		// Invalid arguments
		DiskBintree.main(null);

		String[] args3 = { "x", "x", "x", "x" };
		DiskBintree.main(args3);

		args2[0] = null;
		DiskBintree.main(args2);

		args2[0] = "x";
		args2[1] = "x";
		DiskBintree.main(args2);

		args2[1] = "10";
		DiskBintree.main(args2);
	}
}

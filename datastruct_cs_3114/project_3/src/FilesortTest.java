import java.io.File;
import junit.framework.TestCase;

/**
 * Test case for the FileSort class.
 *
 * @author Elmer Landaverde
 * @version Nov 6, 2013
 */
public class FilesortTest
	extends TestCase
{

	/**
	 * Testing binary file
	 */
	private String	bTempFile	= "sorted_10_blocks_b.dat";

	/**
	 * Testing text file.
	 */
	private String	aTempFile	= "sorted_10_blocks_a.dat";


	/**
	 * Set up the variables for the testing methods.
	 */
	protected void setUp()
		throws Exception
	{
		super.setUp();
		// Make copy of test files.
		File bf = new File(Util.bFile);
		File btf = new File(bTempFile);
		btf.createNewFile();
		Util.copyFile(bf, btf);

		File af = new File(Util.aFile);
		File atf = new File(aTempFile);
		atf.createNewFile();
		Util.copyFile(af, atf);
	}


	/**
	 * Test case for the main function of the FileSort class.
	 */
	public void testMain()
	{
		new Util();
		new Filesort();
		// Test invlid arguments
		try
		{
			Filesort.main(null);
			assertFalse(Filesort.success);

			String args1[] = new String[0];
			Filesort.main(args1);
			assertFalse(Filesort.success);

			String args2[] = new String[4];
			Filesort.main(args2);
			assertFalse(Filesort.success);

			String args3[] = { null, null };
			Filesort.main(args3);
			assertFalse(Filesort.success);

			String args4[] = { "unknow", "10" };
			Filesort.main(args4);
			assertFalse(Filesort.success);

			String args5[] = { "src", "10" };
			Filesort.main(args5);
			assertFalse(Filesort.success);

			String args6[] = { Util.aFile, null };
			Filesort.main(args6);
			assertFalse(Filesort.success);

			String args7[] = { Util.aFile, "hello" };
			Filesort.main(args7);
			assertFalse(Filesort.success);

			String args8[] = { Util.aFile, "0" };
			Filesort.main(args8);
			assertFalse(Filesort.success);

		}
		catch (Exception e)
		{
			fail();
		}

		// Sort first file.
		try
		{
			String args[] = { bTempFile, "4" };
			Filesort.main(args);
			assertTrue(Filesort.success);
		}
		catch (Exception e)
		{
			e.printStackTrace();
			fail();
		}

		// Sort second file.
		try
		{
			String args[] = { aTempFile, "4" };
			Filesort.main(args);
			assertTrue(Filesort.success);
		}
		catch (Exception e)
		{
			fail();
		}
	}


	/**
	 * Closed the resources.
	 */
	protected void tearDown()
		throws Exception
	{
		super.tearDown();
		// Delete copy of test files.
		File btf = new File(bTempFile);
		btf.delete();
		File atf = new File(aTempFile);
		atf.delete();
	}

}

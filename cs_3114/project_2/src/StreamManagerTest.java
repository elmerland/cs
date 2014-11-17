import java.util.ArrayList;
import junit.framework.TestCase;

/**
 * Test case for the StreamManager class.
 * 
 * @author Elmer Landaverde
 * @version Oct 11, 2013
 */
public class StreamManagerTest
	extends TestCase
{
	
	/**
	 * Test case for the StreamManager class.
	 */
	public void testStreamManager()
	{
		new StreamManager();
		
		assertNull(StreamManager.getEarthquakeData(true, "unknwon"));
		StreamManager.parseWatcherCmds(null);
		StreamManager.printWatcherNotFound("Doe");
		
		ArrayList<String> cmds = new ArrayList<String>();
		cmds.add("");
		StreamManager.parseWatcherCmds(cmds);
		
		cmds = new ArrayList<String>();
		cmds.add(null);
		StreamManager.parseWatcherCmds(cmds);
		
		cmds = new ArrayList<String>();
		cmds.add("add helo hello hello");
		StreamManager.parseWatcherCmds(cmds);
		
		cmds = new ArrayList<String>();
		cmds.add("asjuhg alsdjhg aisug a");
		StreamManager.parseWatcherCmds(cmds);
		
	}
	
	
	protected void setUp()
		throws Exception
	{
		super.setUp();
	}
	
}

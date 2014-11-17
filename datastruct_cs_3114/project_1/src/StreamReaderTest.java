import java.util.ArrayList;
import realtimeweb.earthquakeservice.domain.Report;
import junit.framework.TestCase;

/**
 * Test case for StreamReader class.
 *
 * @author Elmer Landaverde, Harjas Singh
 * @version Sep 10, 2013
 */
public class StreamReaderTest
    extends TestCase
{
    /**
     * Test case for the getEarthquakeData method in the StreamReader class.
     */
    public void testGetEarthquakeData()
    {
        // Check correct input with on line data.
        Report r = StreamReader.getEarthquakeData(false, null);
        assertNotNull(r);

        // Check correct input with off line data.
        r = StreamReader.getEarthquakeData(true, "normal.earthquakes.json");
        assertNotNull(r);

        // Check incorrect input with off line data
        r = StreamReader.getEarthquakeData(true, "unknown_file");
        assertNull(r);
    }


    /**
     * Test case for the getWatcherData method in the StreamReader class.
     */
    public void testGetWatcherData()
    {
        // Check correct input.
        ArrayList<String> newCmds;
        try
        {
            newCmds = StreamReader.getWatcherData("watcher.txt");
            assertNotNull(newCmds);
        }
        catch (Exception e)
        {
            assertTrue(false);
        }

        // Check incorrect input.
        try {
            newCmds = StreamReader.getWatcherData("unknown.txt");
        }
        catch (Exception e) {
            assertTrue(true);
        }
    }

}

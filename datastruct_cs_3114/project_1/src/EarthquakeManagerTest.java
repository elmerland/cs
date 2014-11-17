import realtimeweb.earthquakeservice.domain.Earthquake;
import junit.framework.TestCase;

/**
 * Test case for the EarthquakeManager class
 *
 * @author Elmer Landaverde, Harjas Singh
 * @version Sep 10, 2013
 */
public class EarthquakeManagerTest
    extends TestCase
{

    /**
     * Test case for printDate method
     */
    public void testPrintDate()
    {
        assertNotNull(DSutil.printDate(System.currentTimeMillis()));
    }


    /**
     * Test case for print hours method
     */
    public void testPrintHours()
    {
        assertNotNull(DSutil.printHours(System.currentTimeMillis()));
    }


    /**
     * Test case for swapIndex method
     */
    public void testSwapIndex()
    {
        Earthquake eq =
            new Earthquake(
                null,
                10.0,
                null,
                0,
                null,
                0,
                0,
                0,
                null,
                null,
                0,
                null,
                0,
                0,
                0);
        EarthquakeManager.QueueLog ql1 = new EarthquakeManager.QueueLog(eq, 10);

        EarthquakeManager.QueueLog ql2 = new EarthquakeManager.QueueLog(eq, 11);

        DSutil.swapIndexables(ql1, ql2);

        assertEquals(ql1.getIndex(), 11);
        assertEquals(ql2.getIndex(), 10);
    }

}

import realtimeweb.earthquakeservice.domain.Earthquake;
import junit.framework.TestCase;

/**
 * Write a one-sentence summary of your class here. Follow it with additional
 * details about its purpose, what abstraction it represents, and how to use it.
 *
 * @author Elmer
 * @version Sep 10, 2013
 */
public class EqSimpleTest
    extends TestCase
{

    /**
     * Test case for the main method in the EqSimple class.
     *
     * @throws InterruptedException
     *             Error happened
     */
    public void testMain()
        throws InterruptedException
    {
        try
        {
            EqSimple qs = new EqSimple();
            assertNotNull(qs.toString());
            // Check correct input for normal.earthquakes.json files
            String[] inputs =
                { "--all", "watcher.txt", "normal.earthquakes.json" };

            EqSimple.main(inputs);
            assertTrue(EqSimple.getVerboseFlag());
            assertTrue(EqSimple.getOffLineFlag());
            assertEquals(EqSimple.getWatchersFile(), inputs[1]);
            assertNotNull(EqSimple.getEarthquakesFile());
            assertFalse(EqSimple.inputError);

            // Check correct input with off line status and --all flag
            inputs[2] = "normal.earthquakes.json";
            EqSimple.main(inputs);
            assertTrue(EqSimple.getVerboseFlag());
            assertTrue(EqSimple.getOffLineFlag());
            assertEquals(EqSimple.getWatchersFile(), inputs[1]);
            assertEquals(EqSimple.getEarthquakesFile(), inputs[2]);
            assertFalse(EqSimple.inputError);

            inputs = new String[2];
            inputs[0] = "watcher.txt";
            inputs[1] = "live";

            // Check correct input with on line status
            /*
             * EqSimple.main(inputs); assertFalse(EqSimple.getAllFlag());
             * assertFalse(EqSimple.getOffLineFlag());
             * assertEquals(EqSimple.getWatchersFile(), inputs[0]);
             * assertNull(EqSimple.getEarthquakesFile());
             * assertFalse(EqSimple.inputError);
             */

            // Check correct input with off line status
            inputs[0] = "watcher.txt";
            inputs[1] = "normal.earthquakes.json";
            EqSimple.main(inputs);
            assertFalse(EqSimple.getVerboseFlag());
            assertTrue(EqSimple.getOffLineFlag());
            assertEquals(EqSimple.getWatchersFile(), inputs[0]);
            assertEquals(EqSimple.getEarthquakesFile(), inputs[1]);
            assertFalse(EqSimple.inputError);

            StreamOutput.printEarthquakeAdded(new Earthquake(
                null,
                0,
                "place 1",
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
                0));

            // Check incorrect input with null input
            EqSimple.main(null);
            assertTrue(EqSimple.inputError);

            // Check incorrect input with all flag.
            inputs = new String[3];
            inputs[0] = "--Unknown";
            EqSimple.main(inputs);
            assertTrue(EqSimple.inputError);

            // Check incorrect input with watchers file.
            inputs[0] = "--all";
            inputs[1] = "unknown_file.txt";
            EqSimple.main(inputs);
            assertTrue(EqSimple.inputError);

            // Check incorrect
            inputs[1] = "watcher.txt";
            inputs[2] = "unknown_state";
            EqSimple.main(inputs);
            assertTrue(EqSimple.inputError);

            inputs = new String[1];
            EqSimple.main(inputs);
            assertTrue(EqSimple.inputError);

            EqSimple.testingFlag = true;
            String[] inputs2 = { "--all", "watcher.txt", "live" };
            EqSimple.main(inputs2);
            assertFalse(EqSimple.getOffLineFlag());
        }
        catch (Exception e)
        {
            assertTrue(false);
        }
    }

}

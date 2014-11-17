import realtimeweb.earthquakeservice.domain.Coordinate;
import realtimeweb.earthquakeservice.domain.Earthquake;
import junit.framework.TestCase;

/**
 *  Test case for the WatcherManager class
 *
 *  @author Elmer Landaverde, Harjas Singh
 *  @version Sep 10, 2013
 */
public class WatcherManagerTest
    extends TestCase
{
    /**
     * Test case for the addUser method of the Watchers class.
     */
    public void testAddUser()
    {
        // Initialize watchers object
        WatcherManager w = new WatcherManager();
        // Add users
        assertTrue(w.addUser("Me", 10, 10));
        assertTrue(w.addUser("You", 20, 10));
        assertTrue(w.addUser("Him", 30, 10));
        assertTrue(w.addUser("Her", 40, 10));
        assertTrue(w.addUser("She", 50, 10));
        // Check the number of users.
        assertEquals(w.getUserCount(), 5);
        // Try to add a user with existing name
        assertFalse(w.addUser("She", 60, 10));
        // Test that user was not added.
        assertEquals(w.getUserCount(), 5);
        // Try to add user with null name,
        assertFalse(w.addUser(null, 10, 10));
        // Test that user was not added.
        assertEquals(w.getUserCount(), 5);
    }


    /**
     * Test case for the removeUser of the Watchers class.
     */
    public void testRemoveUser()
    {
        WatcherManager w = new WatcherManager();
        w.addUser("Me", 10, 10);
        w.addUser("You", 20, 10);
        w.addUser("Him", 30, 10);
        w.addUser("Her", 40, 10);
        w.addUser("She", 50, 10);
        assertEquals(w.getUserCount(), 5);
        assertTrue(w.removeUser("Him"));
        assertEquals(w.getUserCount(), 4);
        assertFalse(w.removeUser("Unknown"));
        assertEquals(w.getUserCount(), 4);
    }


    /**
     * Test case for the reportEarthquake method in the Watchers class.
     */
    public void testReportEarthquake()
    {
        WatcherManager w = new WatcherManager();
        w.addUser("Elmer", 10, 10);
        Coordinate c = new Coordinate(15, 15, 10);
        Earthquake earth =
            new Earthquake(
                c,
                5.0,
                "a place near this place",
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

        assertTrue(w.reportEarthquake(earth));

        earth =
            new Earthquake(
                c,
                0.0,
                "a place near this place",
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
        assertFalse(w.reportEarthquake(earth));

        assertFalse(w.reportEarthquake(null));
    }

}

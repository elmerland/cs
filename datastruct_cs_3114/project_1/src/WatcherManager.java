import realtimeweb.earthquakeservice.domain.Coordinate;
import realtimeweb.earthquakeservice.domain.Earthquake;

// -------------------------------------------------------------------------
/**
 * The Watchers class is in charge of keeping track of all the current users
 * that are watching for earthquakes. It also handles any queries related to
 * watchers.
 *
 * @author Elmer Landaverde, Harjas Singh
 * @version Sep 3, 2013
 */
public class WatcherManager
{

    /**
     * The WatcherUser class is an inner class used to store all the information
     * related to individual watcher users.
     *
     * @author Elmer Landaverde, Harjas Singh
     * @version Sep 3, 2013
     */
    private class WatcherUser
    {
        /**
         * Hold the user name of the watcher user.
         */
        public String name;

        /**
         * Holds the latitude coordinate of the watcher user.
         */
        public int    latitude;

        /**
         * Holds the longitude coordinate of the watcher user.
         */
        public int    longitude;


        /**
         * Creates a new WatcherUser object.
         *
         * @param userName
         *            The user name of the watcher user.
         * @param lat
         *            The latitude coordinate of the watcher user.
         * @param longi
         *            The longitude coordinate of the watcher user.
         */
        public WatcherUser(String userName, int lat, int longi)
        {
            name = userName;
            latitude = lat;
            longitude = longi;
        }
    }

    /**
     * Linked list that holds all the watcher users.
     */
    private LList<WatcherUser> userList;


    /**
     * Create a new WatcherManager object
     */
    public WatcherManager()
    {
        userList = new LList<WatcherUser>();
    }


    /**
     * Adds a new user to the list of watchers. A new user will not be added if
     * another user with the same name already exists or if one of the
     * parameters is invalid.
     *
     * @param name
     *            The name of the new user.
     * @param lat
     *            The latitude coordinate of the new user.
     * @param lon
     *            The longitude coordinate of the new user.
     * @return True if the user was added, false if the user was not added.
     */
    public boolean addUser(String name, int lat, int lon)
    {
        if (name == null || hasUser(name))
        {
            return false;
        }
        userList.append(new WatcherUser(name, lat, lon));
        StreamOutput.printWatcherAdded(name);
        return true;
    }


    /**
     * Determines if there exists a user with the specified user name in the
     * list.
     *
     * @return True if a user with the same name exists, false if it doesn't.
     */
    private boolean hasUser(String name)
    {
        userList.moveToStart();
        while (userList.currPos() < userList.length())
        {
            String temp = userList.getValue().name;
            if (temp.equals(name))
            {
                return true;
            }
            userList.next();
        }
        return false;
    }


    /**
     * Removes the user with the specified name from the list.
     *
     * @param name
     *            The name of the user to be removed
     * @return True if the user was removed, false if no such use was found.
     */
    public boolean removeUser(String name)
    {
        userList.moveToStart();
        while (userList.currPos() < userList.length())
        {
            String temp = userList.getValue().name;
            if (temp.equals(name))
            {
                userList.remove();
                StreamOutput.printWatcherRemoved(name);
                return true;
            }
            userList.next();
        }
        return false;
    }


    /**
     * Gets the number of watcher users currently in the list.
     *
     * @return The number of watcher users in the current list.
     */
    public int getUserCount()
    {
        return userList.length();
    }


    /**
     * This method looks at all the watchers and determines if one or more of
     * the watchers is close enough to the given earthquake and needs to be
     * notified.
     *
     * @param earthquake
     *            The new earthquake to be reported.
     * @return Returns true if one or more watchers where notified of an
     *         earthquake. Returns false if no watchers were notified.
     */
    public boolean reportEarthquake(Earthquake earthquake)
    {
        if (earthquake == null)
        {
            return false;
        }
        boolean result = false;
        userList.moveToStart();
        while (userList.currPos() < userList.length())
        {
            WatcherUser user = userList.getValue();

            if (isCloseEnough(earthquake, user.longitude, user.latitude))
            {
                result = true;
                StreamOutput.printEarthquakeNotification(earthquake, user.name);
            }
            userList.next();
        }

        return result;
    }


    /**
     * Determines if the distance between the two given points is small enough
     * so that the watcher needs to be notified.
     *
     * @param earthquake
     *            The Earthquake object of the new earthquake.
     * @param x1
     *            The x coordinate of the watcher.
     * @param y1
     *            The y coordinate of the watcher.
     * @return Returns true if the distance between the two points is close
     *         enough. False if it is not close enough.
     */
    private boolean isCloseEnough(Earthquake earthquake, int x1, int y1)
    {
        Coordinate coordinate = earthquake.getLocation();
        double distance =
            distance(
                x1,
                y1,
                coordinate.getLongitude(),
                coordinate.getLatitude());
        double magnitude = earthquake.getMagnitude();
        double maxDistance = 2 * Math.pow(magnitude, 3);

        return distance < maxDistance;
    }


    /**
     * This method determines the distance between two points in a 2D plane.
     *
     * @param x1
     *            The x coordinate of the first point.
     * @param y1
     *            The y coordinate of the first point.
     * @param x2
     *            The x coordinate of the second point.
     * @param y2
     *            The y coordinate of the second point.
     * @return The distance between the two points.
     */
    private double distance(int x1, int y1, double x2, double y2)
    {
        double temp1 = Math.pow(x2 - x1, 2.0); // (x2 - x1)^2
        double temp2 = Math.pow(y2 - y1, 2.0); // (y2 - y1)^2

        double distance = Math.sqrt(temp1 + temp2);

        return distance;
    }
}

import realtimeweb.earthquakeservice.domain.Coordinate;
import realtimeweb.earthquakeservice.domain.Earthquake;

/**
 * The Watchers class is in charge of keeping track of all the current users
 * that are watching for earthquakes. It also handles any queries related to
 * watchers.
 * 
 * @author Elmer Landaverde
 * @version Sep 3, 2013
 */
public class WatcherManager
{
	
	/**
	 * The binary search tree used to search watcher users by name.
	 */
	private BST<String, WatcherUser>	bst;
	
	/**
	 * The bin tree used to search watcher users by coordinates.
	 */
	private BinTree<WatcherUser>		bint;
	
	/**
	 * A rectangle object that represents the size of the world.
	 */
	private Rectangle					world	= new Rectangle(0, 180, 360, 180);
	
	/**
	 * Variable the stores the current earthquake that was reported to the
	 * WatcherManager by the EarthquakeManager.
	 */
	public static Earthquake			currentEarthquake;
	
	
	/**
	 * Create a new WatcherManager object
	 */
	public WatcherManager()
	{
		bst = new BST<String, WatcherUser>();
		bint = new BinTree<WatcherUser>(world);
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
	public boolean addUser(String name, double lon, double lat)
	{
		// Check if user name is duplicate
		WatcherUser temp = bst.find(name);
		if (temp != null)
		{
			// If user name already exists, then print user duplicate message.
			StreamManager.printWatcherDuplicate(temp, true);
			return false;
		}
		// Add user to BST
		WatcherUser newWatcher = new WatcherUser(name, lon, lat);
		bst.insert(name, newWatcher);
		StreamManager.printWatcherAdded(newWatcher, true);
		
		// Check if coordinates are duplicate
		boolean addToBinTree = true;
		try
		{
			temp = bint.find(newWatcher.getCoordinates());
		}
		catch (Exception e)
		{
			// Catch any exception in the case that the coordinates for the
			// watcher user are invalid.
			addToBinTree = false;
		}
		if (temp != null || !addToBinTree)
		{
			// Watcher user cannot be added to bin tree. Print appropriate
			// message.
			StreamManager.printWatcherDuplicate(newWatcher, false);
			// Remove user from BST
			bst.remove(name);
			// Print remove from BST message.
			StreamManager.printWatcherRemoved(newWatcher, false, true);
			return false;
		}
		
		// Add user to bin tree
		bint.insert(newWatcher.getCoordinates(), newWatcher);
		StreamManager.printWatcherAdded(newWatcher, false);
		return true;
		
	}
	
	
	/**
	 * Gets the bin tree of WatcherUser objects.
	 * 
	 * @return The bintree.
	 */
	public BinTree<WatcherUser> getBinTree()
	{
		return bint;
	}
	
	
	/**
	 * Gets the binary search tree of WatcherUser objects.
	 * 
	 * @return The BST.
	 */
	public BST<String, WatcherUser> getBST()
	{
		return bst;
	}
	
	
	/**
	 * Gets the number of watcher users currently in the list.
	 * 
	 * @return The number of watcher users in the current list.
	 */
	public int getUserCount()
	{
		return bst.size();
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
		// Attempt to remove user from BST
		WatcherUser removed = bst.remove(name);
		if (removed == null)
		{
			// Watcher user was not found, print watcher not found message.
			StreamManager.printWatcherNotFound(name);
			return false;
		}
		else
		{
			// Print watcher removed message.
			StreamManager.printWatcherRemoved(removed, true, true);
			// Remove user from bin tree.
			bint.remove(removed.getCoordinates());
			StreamManager.printWatcherRemoved(removed, true, false);
			return true;
		}
	}
	
	
	/**
	 * This method looks at all the watchers and determines if one or more of
	 * the watchers is close enough to the given earthquake and needs to be
	 * notified.
	 * 
	 * @param earthquake
	 *            The new earthquake to be reported.
	 */
	public void reportEarthquake(Earthquake earthquake)
	{
		// Print header
		StreamManager.printWatcherNotificationHeader(earthquake);
		
		// Update current earthquake for the WatcherUsers to use.
		currentEarthquake = earthquake;
		
		// Get coordinates of earthquake.
		double x = earthquake.getLocation().getLongitude() + 180.0;
		double y = earthquake.getLocation().getLatitude() + 90.0;
		double[] origin = { x, y };
		double radius = getNotificationRadius(earthquake);
		
		// Notified appropriate users in the bin tree.
		bint.regionSearch(origin, radius);
		
		// Print number of visited nodes.
		int visitedNodes = bint.getNumberOfVisitedNodes();
		StreamManager.printWatcherVisitedNotification(visitedNodes);
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
	public static double distance(double x1, double y1, double x2, double y2)
	{
		double temp1 = Math.pow(x2 - x1, 2.0); // (x2 - x1)^2
		double temp2 = Math.pow(y2 - y1, 2.0); // (y2 - y1)^2
		
		double distance = Math.sqrt(temp1 + temp2);
		
		return distance;
	}
	
	
	/**
	 * Gets the radius of the maximum distance from the origin of the
	 * earthquake, any watcher inside of that radius has to be notified.
	 * 
	 * @param earthquake
	 *            The Earthquake object.
	 * @return The radius of maximum distance from the origin of the earthquake.
	 */
	public static double getNotificationRadius(Earthquake earthquake)
	{
		double magnitude = earthquake.getMagnitude();
		double radius = 2 * Math.pow(magnitude, 3);
		return radius;
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
	public static boolean isCloseEnough(Earthquake earthquake, double x1, double y1)
	{
		// Get coordinates of earthquake.
		Coordinate coordinate = earthquake.getLocation();
		// Get distance between earthquake and the given point.
		double distance =
			WatcherManager.distance(
				x1,
				y1,
				coordinate.getLongitude(),
				coordinate.getLatitude());
		// Get maximum distance for notification.
		double magnitude = earthquake.getMagnitude();
		double maxDistance = 2 * Math.pow(magnitude, 3);
		
		// Determine if the given point is close enough to be notified.
		return distance < maxDistance;
	}
}

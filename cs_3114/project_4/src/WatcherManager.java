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
	 * The bin tree used to search watcher users by coordinates.
	 */
	private BinTree		bint;

	/**
	 * A rectangle object that represents the size of the world.
	 */
	private Rectangle	world	= new Rectangle(0, 180, 360, 180);


	/**
	 * Create a new WatcherManager object
	 *
	 * @throws Exception
	 *             Could not access disk.
	 */
	public WatcherManager()
		throws Exception
	{
		bint = new BinTree(world);
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
	 * @throws Exception
	 *             Could not access disk.
	 */
	public boolean addUser(String name, double lon, double lat)
		throws Exception
	{
		WatcherUser user = new WatcherUser(name, lon, lat);
		// Add user.
		if (bint.insert(user.getCoordinates(), user))
		{
			// User added successfully.
			StreamManager.printAddUser(user, true);
			return false;
		}
		else
		{
			// User has duplicate. Not added.
			StreamManager.printAddUser(user, false);
			return true;
		}
	}


	/**
	 * Gets the bin tree of WatcherUser objects.
	 *
	 * @return The bin tree.
	 */
	public BinTree getBinTree()
	{
		return bint;
	}


	/**
	 * Removes the watcher user at the specified coordinates.
	 *
	 * @param x
	 *            The x coordinate.
	 * @param y
	 *            The y coordinate.
	 * @return True if the user was removed.
	 * @throws Exception
	 *             Could not access disk.
	 */
	public boolean removeUser(double x, double y)
		throws Exception
	{
		WatcherUser user = new WatcherUser(null, x, y);
		WatcherUser deleted = bint.remove(user.getCoordinates());
		if (deleted == null)
		{
			StreamManager.printDeleteUser(user, false);
			return false;
		}
		else
		{
			StreamManager.printDeleteUser(deleted, true);
			return true;
		}
	}


	/**
	 * Searches all users that fall within the given circle.
	 *
	 * @param x
	 *            The x coordinate of the center of the circle.
	 * @param y
	 *            The y coordinate of the center of the circle.
	 * @param rad
	 *            The radius of the circle.
	 * @return True if the search was successful.
	 * @throws Exception
	 *             Could not access disk.
	 */
	public boolean searchUser(double x, double y, double rad)
		throws Exception
	{
		WatcherUser user = new WatcherUser(null, x, y);
		StreamManager.printSearchHeader(user, rad);
		bint.regionSearch(user.getCoordinates(), rad);
		StreamManager.printSearchFooter(bint.getNumberOfVisitedNodes());
		return true;
	}
}

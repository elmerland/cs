/**
 * The WatcherUser class is an inner class used to store all the information
 * related to individual watcher users.
 *
 * @author Elmer Landaverde
 * @version Sep 3, 2013
 */
public class WatcherUser
{

	/**
	 * Hold the user name of the watcher user.
	 */
	private String	name;

	/**
	 * Holds the latitude coordinate of the watcher user.
	 */
	private double	latitude;

	/**
	 * Holds the longitude coordinate of the watcher user.
	 */
	private double	longitude;


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
	public WatcherUser(String userName, double longi, double lat)
	{
		name = userName;
		longitude = longi + 180.0;
		latitude = lat + 90.0;
	}


	/**
	 * Get the coordinates for this watcher user.
	 *
	 * @return An integer array containing the longitude and latitude
	 *         coordinates.
	 */
	public double[] getCoordinates()
	{
		double[] r = { longitude, latitude };
		return r;
	}


	/**
	 * Gets the latitude of this watcher user.
	 *
	 * @return the latitude
	 */
	public double getLatitude()
	{
		return latitude;
	}


	/**
	 * Gets the longitude of this watcher user.
	 *
	 * @return the longitude
	 */
	public double getLongitude()
	{
		return longitude;
	}


	/**
	 * Gets the name of this watcher user.
	 *
	 * @return the name
	 */
	public String getName()
	{
		return name;
	}


	/**
	 * Gets a string representation of the coordinates for this WatchUser
	 * object.
	 *
	 * @return A string representation of the coordinates for this WatcherUSer
	 *         object.
	 */
	public String printCoordinates()
	{
		double longi = longitude - 180.0;
		double lat = latitude - 90.0;

		return String.format("%.1f", longi) + " " + String.format("%.1f", lat);
	}


	@Override
	public String toString()
	{
		double longi = longitude - 180.0;
		double lat = latitude - 90.0;

		return name + " " + String.format("%.1f", longi) + " "
			+ String.format("%.1f", lat);
	}


	/**
	 * This method is called when an earthquake is reported to this watcher
	 * user. This method determines if this watcher user is close enough to the
	 * earthquake. If so, it prints the appropriate message to console.
	 *
	 * @param x
	 *            The x coordinate of the earthquake origin.
	 * @param y
	 *            The y coordinate of the earthquake origin.
	 * @param radius
	 *            The radius of the earthquake,
	 */
	public void visit(double x, double y, double radius)
	{
		// Get distance
		double dist = distance(x, y, longitude, latitude);
		// Check if inside radius.
		if (dist <= radius)
		{
			StreamManager.printSearchUser(this);
		}
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
}

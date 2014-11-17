/**
 * The WatcherUser class is an inner class used to store all the information
 * related to individual watcher users.
 * 
 * @author Elmer Landaverde
 * @version Sep 3, 2013
 */
public class WatcherUser
	implements Visitable
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
		if (longi == 180.0)
		{
			longi = -180.0;
		}
		if (lat == 90.0)
		{
			lat = -90.0;
		}
		latitude = lat + 90.0;
		longitude = longi + 180.0;
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
	
	
	@Override
	public void visit()
	{
		if (WatcherManager.currentEarthquake == null)
		{
			return;
		}
		// Determine if the earthquake is close enough to this watcher user.
		if (WatcherManager.isCloseEnough(
			WatcherManager.currentEarthquake,
			longitude - 180.0,
			latitude - 90.0))
		{
			StreamManager
				.printWatcherNotification(WatcherManager.currentEarthquake, this);
		}
	}
}

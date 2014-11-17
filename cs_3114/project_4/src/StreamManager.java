import java.io.BufferedReader;
import java.io.FileReader;

/**
 * The StreamManager class is in charge of dealing with everything relating to
 * streams. This class has three responsibilities:
 * <p>
 * 1 - Get data from the EarthquakeService and WatcherService <br>
 * 2 - Interpret data obtained from the services </br> <br>
 * 3 - Output information to System.out </br>
 * </p>
 *
 * @author Elmer Landaverde
 * @version Sept 5, 2013
 */
public class StreamManager
{

	/**
	 * Token separator. The character used to split the watcher commands into
	 * individual tokens.
	 */
	private static String	delimeterCharacter	= " ";


	/**
	 * Takes in the name of a file and constructs a bin tree out of the contents
	 * of the input file.
	 *
	 * @param fileName
	 *            The name of the input file.
	 * @return True if the file was digested successfully.
	 * @throws Exception
	 *             Could not read file.
	 */
	public static boolean digestInputFile(String fileName)
		throws Exception
	{
		FileReader fr = new FileReader(fileName);
		BufferedReader reader = new BufferedReader(fr);
		String line;
		while ((line = reader.readLine()) != null)
		{
			processCmd(line);
		}
		reader.close();

		printBufferPoolMetrics();
		return true;
	}


	/**
	 * Takes one watcher command and determines what has to be done. Then it
	 * invokes the necessary class and passes the formated information.
	 *
	 * @param cmd
	 *            The command to be processed.
	 */
	private static void processCmd(String cmd)
		throws Exception
	{
		// Get type of query
		if (cmd.startsWith("add" + StreamManager.delimeterCharacter))
		{
			// Get command tokens
			String[] addCmd = cmd.split(StreamManager.delimeterCharacter, 4);

			// Extract information.
			String name = addCmd[3];
			double longitude = Double.parseDouble(addCmd[1]);
			double latitude = Double.parseDouble(addCmd[2]);
			// Add user.
			DiskBintree.getWatcherManager().addUser(name, longitude, latitude);
		}
		else if (cmd.startsWith("delete" + StreamManager.delimeterCharacter))
		{
			// Get command tokens
			String[] delCmd = cmd.split(StreamManager.delimeterCharacter, 3);
			// Get coordinates.
			double longitude = Double.parseDouble(delCmd[1]);
			double latitude = Double.parseDouble(delCmd[2]);
			// Remove user.
			DiskBintree.getWatcherManager().removeUser(longitude, latitude);
		}
		else if (cmd.startsWith("search"))
		{
			// Get command tokens
			String[] searchCmd = cmd.split(StreamManager.delimeterCharacter, 4);
			// Remove user.
			double longitude = Double.parseDouble(searchCmd[1]);
			double latitude = Double.parseDouble(searchCmd[2]);
			double radius = Double.parseDouble(searchCmd[3]);
			DiskBintree.getWatcherManager().searchUser(longitude, latitude, radius);
		}
		else
		{
			// Print bin tree information.
			System.out.print(DiskBintree.getWatcherManager().getBinTree().debugString());
			// Print buffer information.
			BufferPool bp = (BufferPool)DiskBintree.getBufferPool();
			for (int i = 0; i < DiskBintree.getNumberBuffers(); i++)
			{
				System.out.println("Block ID of buffer" + i + " is " + bp.getBufferID(i));
			}
		}
	}


	/**
	 * Prints message to console after adding a new user.
	 *
	 * @param user
	 *            The watcher user to be added.
	 * @param success
	 *            Whether the user was added successfully or not.
	 */
	public static void printAddUser(WatcherUser user, boolean success)
	{
		if (success)
		{
			System.out.println(user.toString() + " is added into the bintree");
		}
		else
		{
			System.out.println(user.toString()
				+ " duplicates a watcher already in the bintree");
		}
	}


	/**
	 * Prints a message to console after deleting a user.
	 *
	 * @param user
	 *            The user to be deleted.
	 * @param success
	 *            Whether the user was deleted successfully or not.
	 */
	public static void printDeleteUser(WatcherUser user, boolean success)
	{
		if (success)
		{
			System.out.println(user.toString() + " is removed from the bintree");
		}
		else
		{
			System.out.println("There is no record at " + user.printCoordinates()
				+ " in the bintree");
		}
	}


	/**
	 * Prints a message to console before searching for users.
	 *
	 * @param user
	 *            A watcher user object with the search coordinates.
	 * @param radius
	 *            The radius of the search
	 */
	public static void printSearchHeader(WatcherUser user, double radius)
	{
		System.out.println("Search " + user.printCoordinates() + " "
			+ String.format("%.1f", radius) + " returned the following watchers:");
	}


	/**
	 * Prints a message to console after searching for users.
	 *
	 * @param count
	 *            The number of users that were found.
	 */
	public static void printSearchFooter(int count)
	{
		System.out.println("Watcher search caused " + count
			+ " bintree nodes to be visited.");
	}


	/**
	 * Prints a message to console when this user was visited by a search
	 * command.
	 *
	 * @param user
	 *            The user visited.
	 */
	public static void printSearchUser(WatcherUser user)
	{
		System.out.println(user.toString());
	}


	/**
	 * Prints a message to console with the metrics of the buffer pool manager.
	 */
	public static void printBufferPoolMetrics()
	{
		System.out.println(DSutil.getFormatedMetrics((BufferPool)DiskBintree
			.getBufferPool()));
	}
}

import realtimeweb.earthquakeservice.domain.Earthquake;

// -------------------------------------------------------------------------
/**
 * The StreamOutput class is in charge of printing out all the messages to
 * console. Each class that triggers the messages needs to provide the
 * appropriate information and the methods in this class will format the
 * information in the appropriate manner and then print to console.
 *
 * @author Elmer Landaverde, Harjas Singh
 * @version Sep 2, 2013
 */
public class StreamOutput
{
    /**
     * Prints a message notifying the addition of a watcher user.
     *
     * @param name
     *            The name of the user who was added to the watcher list.
     */
    public static void printWatcherAdded(String name)
    {
        System.out.println(name + " is added to the watchers list");
    }


    /**
     * Prints a message notifying the removal of a watcher user.
     *
     * @param name
     *            The name of the user who was deleted from the watcher list.
     */
    public static void printWatcherRemoved(String name)
    {
        System.out.println(name + " is removed from the watchers list");
    }


    /**
     * Prints a message that notifies a watcher of an earthquake near him/her.
     *
     * @param earthquake
     *            The Earthquake object.
     * @param userName
     *            The name of the user that needs to be notified.
     */
    public static void printEarthquakeNotification(
        Earthquake earthquake,
        String userName)
    {
        System.out.println("Earthquake in "
            + earthquake.getLocationDescription() + " is close to " + userName);
    }


    /**
     * Prints a message notifying of the insertion of a new earthquake. The
     * message is only printed if the --all option was enabled.
     *
     * @param earthquake
     *            The earthquake object that was added to the heap.
     */
    public static void printEarthquakeAdded(Earthquake earthquake)
    {
        if (EqSimple.getVerboseFlag())
        {
            System.out.println("Earthquake "
                + earthquake.getLocationDescription()
                + ", is inserted into the Heap.");
        }
    }


    /**
     * Prints a message reporting the earthquake with the largest magnitude.
     *
     * @param earthquake
     *            The earthquake object
     */
    public static void printMagnitudeQuery(Earthquake earthquake)
    {
        System.out.println("Largest earthquake in past 6 hours:\nMagnitude: "
            + earthquake.getMagnitude() + " at "
            + earthquake.getLocationDescription());
    }


    /**
     * Prints a message reporting that there are no elements in the max heap.
     */
    public static void printNoQuery()
    {
        System.out.println("No record on MaxHeap");
    }

}

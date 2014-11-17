import java.util.ArrayList;
import realtimeweb.earthquakeservice.domain.Report;

/**
 * The StreamParser class is in charge of digesting the raw information obtained
 * by the StreamReader class. It processes all of the commands or earthquake
 * records, determines what needs to be done with each one, and finally calls
 * the appropriate classes and passes the formated information.
 *
 * @author Elmer Landaverde, Harjas Singh
 * @version Aug 31, 2013
 */
public class StreamParser
{

    /**
     * Token separator. The character used to split the watcher commands into individual
     * tokens.
     */
    private static String delimeterCharacter = " ";


    /**
     * Takes a list of new watcher commands and determines what has to be done
     * with each one of them. Then it invokes the necessary methods and passes
     * the formated information.
     *
     * @param cmds
     *            The list of new watcher commands.
     */
    public static void parseWatcherCmds(ArrayList<String> cmds)
    {
        if (cmds == null)
        {
            // System.out.println("null");
            return;
        }
        else if (cmds.size() == 0)
        {
            // System.out.println("Empty");
            return;
        }
        for (String cmd : cmds)
        {
            processCmd(cmd);
        }
    }


    /**
     * Takes one watcher command and determines what has to be done. Then it
     * invokes the necessary class and passes the formated information.
     *
     * @param cmd
     *            The command to be processed.
     */
    private static void processCmd(String cmd)
    {
        if (cmd == null || cmd.equals(""))
        {
            return;
        }
        // Get type of query
        if (cmd.startsWith("add" + delimeterCharacter))
        {
            // Get command tokens
            String[] addCmd = cmd.split(delimeterCharacter, 4);

            try
            {
                // Extract information.
                String name = addCmd[3];
                int longitude = Integer.parseInt(addCmd[1]);
                int latitude = Integer.parseInt(addCmd[2]);
                // Add user.
                EqSimple.getWatcherManager().addUser(name, latitude, longitude);
            }
            catch (Exception e)
            {
                return;
            }
        }
        else if (cmd.startsWith("delete" + delimeterCharacter))
        {
            // Get command tokens
            String[] delCmd = cmd.split(delimeterCharacter, 2);
            // Remove user.
            EqSimple.getWatcherManager().removeUser(delCmd[1]);
        }
        else if (cmd.equals("query"))
        {
            EqSimple.getEarthquakeManager().queryEarthquakes();
        }
        else
        {
            return;
        }
    }


    /**
     * Takes a Report object and gives the relevant information to the
     * earthquake handler so that the new earthquake events can be processed.
     *
     * @param report
     *            The Report object to be processed.
     */
    public static void parseEarthquakeReport(Report report)
    {
        EqSimple.getEarthquakeManager().addEarthquakeReport(report);
    }
}

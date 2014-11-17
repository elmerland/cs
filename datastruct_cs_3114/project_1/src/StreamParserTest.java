import java.util.ArrayList;
import junit.framework.TestCase;

/**
 *  Test case for the StreamParserTest
 *
 *  @author Elmer Landaverde, Harjas Singh
 *  @version Sep 10, 2013
 */
public class StreamParserTest
    extends TestCase
{
    /**
     * Test case for the parseWatcherCmds methods of the StreamParser class.
     */
    public void testParseWatcherCmds()
    {
        // Test correct input for adding users.
        EqSimple.createWatcherHandlerForTesting();
        ArrayList<String> cmds = new ArrayList<String>();
        cmds.add("add 100 200 Elmer");
        cmds.add("add 300 400 Harjas");

        StreamParser.parseWatcherCmds(cmds);
        assertEquals(EqSimple.getWatcherManager().getUserCount(), 2);

        // Test correct input for removing users.
        cmds.clear();
        cmds.add("delete Elmer");
        cmds.add("delete Harjas");

        StreamParser.parseWatcherCmds(cmds);
        assertEquals(EqSimple.getWatcherManager().getUserCount(), 0);

        // Test incorrect input for adding users.
        cmds.clear();
        cmds.add("add abc abc Elmer");
        cmds.add("add abc abc Harjas");

        StreamParser.parseWatcherCmds(cmds);
        assertEquals(EqSimple.getWatcherManager().getUserCount(), 0);

        // Test incorrect input for removing users.
        cmds.clear();
        cmds.add("add 100 200 Elmer");
        cmds.add("deletes Elmer");
        cmds.add("delete Emrle");

        StreamParser.parseWatcherCmds(cmds);
        assertEquals(EqSimple.getWatcherManager().getUserCount(), 1);

        StreamParser.parseWatcherCmds(null);
        cmds.clear();
        cmds.add("");
        cmds.add(null);
        StreamParser.parseWatcherCmds(cmds);
    }

}

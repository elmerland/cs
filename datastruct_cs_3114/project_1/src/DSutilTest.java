import junit.framework.TestCase;


/**
 *  Write a one-sentence summary of your class here.
 *  Follow it with additional details about its purpose, what abstraction
 *  it represents, and how to use it.
 *
 *  @author Elmer
 *  @version Sep 11, 2013
 */
public class DSutilTest
    extends TestCase
{

    /**
     * Test the constructors for DSutil and StreamOuput
     */
    public void testObject()
    {
        DSutil<String> ds = new DSutil<String>();
        assertNotNull(ds.toString());

        StreamOutput so = new StreamOutput();
        assertNotNull(so.toString());

        StreamOutput.printNoQuery();

        StreamParser sp = new StreamParser();
        assertNotNull(sp.toString());

        StreamReader sr = new StreamReader();
        assertNotNull(sr.toString());
    }

}

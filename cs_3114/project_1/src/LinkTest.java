import junit.framework.TestCase;


/**
 *  Test case for the Link class
 *
 *  @author Elmer Landaverde, Harjas Singh
 *  @version Sep 10, 2013
 */
public class LinkTest
    extends TestCase
{

    /**
     * Test case for the setElement method.
     */
    public void testSetElement()
    {
        Link<String> link = new Link<String>(null);
        link.setElement("Hello World!");
        assertEquals(link.element(), "Hello World!");
    }

}

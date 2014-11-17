import junit.framework.TestCase;



/**
 *  Test case for the LQueue class.
 *
 *  @author Elmer Landaverde, Harjas Singh
 *  @version Sep 11, 2013
 */
public class LQueueTest
    extends TestCase
{

    /**
     * Test case for the clear method.
     */
    public void testClear()
    {
        LQueue<String> lq = new LQueue<String>();
        lq.clear();
        assertTrue(lq.size == 0);
    }


    /**
     * Test case for the dequeue method
     */
    public void testDequeue()
    {
        LQueue<String> lq = new LQueue<String>();
        assertNull(lq.dequeue());
        assertTrue(lq.size == 0);
    }


    /**
     * Test case for the frontValue method
     */
    public void testFrontValue()
    {
        LQueue<String> lq = new LQueue<String>();
        assertNull(lq.frontValue());
        assertTrue(lq.size == 0);
    }

}

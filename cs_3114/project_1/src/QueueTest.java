import junit.framework.TestCase;

/**
 * Test case for the Queue class.
 *
 * @author CS 3114
 * @version Sep 10, 2013
 */
public class QueueTest
    extends TestCase
{
    private Queue<Integer> queue1;
    private Queue<Integer> queue2;


    /**
     * This method is automatically called once before each test case method, so
     * that all the variables are cleanly initialized for each test.
     */
    public void setUp()
    {
        queue1 = new LQueue<Integer>();
        queue2 = new LQueue<Integer>(15);
    }


    /**
     * Place a description of your method here.
     */
    public void testGeneral()
    {
        int temp;
        queue2.enqueue(10);
        queue2.enqueue(20);
        queue2.enqueue(15);
        assertEquals("< 10 20 15 >", queue2.toString());
        while (queue2.length() > 0)
        {
            temp = queue2.dequeue();
            queue1.enqueue(temp);
        }
        assertEquals("< >", queue2.toString());
        assertEquals("< 10 20 15 >", queue1.toString());

    }
}

import junit.framework.TestCase;


/**
 *  Write a one-sentence summary of your class here.
 *  Follow it with additional details about its purpose, what abstraction
 *  it represents, and how to use it.
 *
 *  @author Elmer
 *  @version Sep 10, 2013
 */
public class LListTest
    extends TestCase
{

    /**
     * Place a description of your method here.
     */
    public void testMoveToEnd()
    {
        LList<String> list = new LList<String>();
        list.append("Hello");
        list.append("World");
        list.append("!");
        list.moveToStart();
        list.moveToEnd();
        assertTrue(list.currPos() == list.length());
    }


    /**
     * Place a description of your method here.
     */
    public void testPrev()
    {
        LList<String> list = new LList<String>();
        list.append("Hello");
        list.append("World");
        list.append("!");
        list.moveToStart();
        int pos = list.currPos();
        list.prev();
        assertEquals(pos, list.currPos());

        list.moveToEnd();
        pos = list.currPos();
        list.prev();
        assertEquals(list.currPos(), pos - 1);
    }


    /**
     * Place a description of your method here.
     */
    public void testNext()
    {
        LList<String> list = new LList<String>();
        list.append("Hello");
        list.append("World");
        list.append("!");
        list.moveToEnd();
        int pos = list.currPos();
        list.next();
        assertEquals(list.currPos(), pos);
    }


    /**
     * Test case for the moveToPos method.
     */
    public void testMoveToPos()
    {
        LList<String> list = new LList<String>();
        list.append("Hello");
        list.append("World");
        list.append("!");
        list.moveToPos(-1);
    }


    /**
     * Test case for the getValue method.
     */
    public void testGetValue()
    {
        LList<String> list = new LList<String>();
        list.append("Hello");
        list.append("World");
        list.append("!");
        list.moveToEnd();
        assertNull(list.getValue());
    }

}

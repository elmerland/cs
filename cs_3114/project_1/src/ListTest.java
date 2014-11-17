import junit.framework.TestCase;

/**
 * Test case for List class.
 *
 * @author CS 3114
 * @version Sep 10, 2013
 */
public class ListTest
    extends TestCase
{
    private List<Integer> list1;
    private List<Integer> list2;
    private List<Object>  list3;


    /**
     * @param list
     * @param k
     * @return True if k is in list L, false otherwise
     */
    public static boolean find(List<Integer> list, int k)
    {
        for (list.moveToStart(); list.currPos() < list.length(); list.next()) {
            if (k == list.getValue()) {
                return true; // Found k
            }
        }
        return false; // k not found
    }


    /**
     * This method is automatically called once before each test case method, so
     * that all the variables are cleanly initialized for each test.
     */
    public void setUp()
    {
        list1 = new LList<Integer>();
        list2 = new LList<Integer>(15);
        list3 = new LList<Object>();
    }


    /**
     * Test case for the remove method in the LList class.
     */
    public void testRemove()
    {
        list2.append(1);
        assertEquals("< | 1 >", list2.toString());
        assertEquals(1, (int)list2.remove());
        assertEquals("< | >", list2.toString());
        assertEquals(null, list2.remove());
    }


    /**
     * Test case for the append method in the LList class.
     */
    public void testAppend()
    {
        list2.append(10);
        assertEquals("< | 10 >", list2.toString());
        list2.append(20);
        list2.append(15);
        assertEquals("< | 10 20 15 >", list2.toString());
    }


    /**
     * Test case for the find method in the LList class.
     */
    public void testFind()
    {
        list1.moveToStart();
        list1.insert(39);
        list1.next();
        list1.insert(9);
        list1.insert(5);
        list1.append(4);
        list1.append(3);
        list1.append(2);
        list1.append(1);
        assertEquals("< 39 | 5 9 4 3 2 1 >", list1.toString());
        assertEquals(7, list1.length());

        assertEquals(true, find(list1, 3));
        assertEquals(false, find(list1, 29));
        assertEquals(true, find(list1, 5));
    }


    /**
     * Test case for the listOfObjects method in the LList class.
     */
    public void testListOfObjects()
    {
        assertEquals("< | >", list3.toString());
        list3.insert(3);
        assertEquals("< | 3 >", list3.toString());
        list3.insert("Hello");
        assertEquals("< | Hello 3 >", list3.toString());
    }


    /**
     * Test case for the new method in the LList class.
     */
    public void testNext()
    {
        list2.append(10);
        list2.append(20);
        list2.append(15);
        list2.moveToStart();
        list2.next();
        assertEquals(20, (int)list2.getValue());
    }


    /**
     * Test case for miscellaneous methods of the LList class.
     */
    public void testMoveToStart()
    {
        list2.clear();
        list2.moveToStart();
        list2.insert(1);
        list2.insert(2);
        list2.moveToPos(2);
        list2.insert(3);
        list2.clear();
        assertEquals("< | >", list2.toString());
    }

}

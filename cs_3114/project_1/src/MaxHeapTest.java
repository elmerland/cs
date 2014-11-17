import junit.framework.TestCase;

/**
 * Write a one-sentence summary of your class here. Follow it with additional
 * details about its purpose, what abstraction it represents, and how to use it.
 *
 * @author Elmer
 * @version Sep 11, 2013
 */
public class MaxHeapTest
    extends TestCase
{
    /**
     * The array for the max heap.
     */
    EarthquakeManager.HeapLog[]        hl = new EarthquakeManager.HeapLog[5];

    /**
     * The max heap.
     */
    MaxHeap<EarthquakeManager.HeapLog> heapList;


    /**
     * The set up for the heap list.
     */
    protected void setUp()
        throws Exception
    {

        EarthquakeManager.QueueLog ql = new EarthquakeManager.QueueLog(null, 0);
        hl[0] = new EarthquakeManager.HeapLog(10, ql);

        ql = new EarthquakeManager.QueueLog(null, 1);
        hl[1] = new EarthquakeManager.HeapLog(11, ql);

        ql = new EarthquakeManager.QueueLog(null, 2);
        hl[2] = new EarthquakeManager.HeapLog(12, ql);

        ql = new EarthquakeManager.QueueLog(null, 3);
        hl[3] = new EarthquakeManager.HeapLog(13, ql);

        ql = new EarthquakeManager.QueueLog(null, 4);
        hl[4] = new EarthquakeManager.HeapLog(14, ql);

        heapList = new MaxHeap<EarthquakeManager.HeapLog>(hl, 5, 5);
        super.setUp();
    }


    /**
     * Test case for the isLeaf method.
     */
    public void testIsLeaf()
    {
        assertFalse(heapList.isLeaf(0));
        assertTrue(heapList.isLeaf(4));
    }


    /**
     * Test case for the leftChild method.
     */
    public void testLeftchild()
    {
        assertEquals(heapList.leftchild(0), 1);
        assertEquals(heapList.leftchild(4), -1);
    }


    /**
     * Test case for the right child method.
     */
    public void testRightchild()
    {
        assertEquals(heapList.rightchild(0), 2);
        assertEquals(heapList.rightchild(4), -1);
    }


    /**
     * Test case for the parent method.
     */
    public void testParent()
    {
        assertEquals(heapList.parent(1), 0);
        assertEquals(heapList.parent(0), -1);
    }


    /**
     * Test case for the removeMax method.
     */
    public void testRemovemax()
    {
        assertNotNull(heapList.removemax());
        assertNotNull(heapList.removemax());
        assertNotNull(heapList.removemax());
        assertNotNull(heapList.removemax());
        assertNotNull(heapList.removemax());
        assertNull(heapList.removemax());
    }


    /**
     * Test case for the remove method.
     */
    public void testRemove()
    {
        assertNull(heapList.remove(-5));
        assertNull(heapList.remove(13));
        assertNotNull(heapList.remove(1));
    }

}

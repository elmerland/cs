// -------------------------------------------------------------------------
/**
 * This interface is intended to allow MaxHeap objects to get and swap the index
 * of the QueueLog objects contained within the HeapLog objects.
 *
 * @param <E> The generic type.
 * @author Elmer Landaverde, Harjas Singh
 * @version Sep 17, 2013
 */
public interface ComparableIndexable<E>
    extends Comparable<E>
{
    @Override
    public int compareTo(E o);


    /**
     * Gets the QueueLog object that corresponds to this ComparableHeapLog
     * object.
     *
     * @return The QueueLog object that corresponds to this ComparableHeapLog
     *         object.
     */
    public Indexable getIndexable();
}

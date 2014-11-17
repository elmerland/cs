/**
 * This interface imposes an indexable quality to each class that implements it.
 * This index can refer to anything, and the setIndex and getIndex methods
 * change and obtain the index of each class.
 *
 * @author Elmer Landaverde, Harjas Singh
 * @version Sep 17, 2013
 */
public interface Indexable
{
    /**
     * Gets the index.
     *
     * @return The index
     */
    public int getIndex();


    /**
     * Set the index to the one specified.
     *
     * @param newIndex
     *            The new index.
     */
    public void setIndex(int newIndex);
}

/**
 * Source code example for "A Practical Introduction to Data Structures and
 * Algorithm Analysis, 3rd Edition (Java)" by Clifford A. Shaffer Copyright
 * 2008-2011 by Clifford A. Shaffer
 */

/**
 * The Dictionary abstract class.
 * 
 * @param <Key>
 *            The key object.
 * @param <E>
 *            The value object.
 */
public interface Dictionary<Key, E>
{
	
	/**
	 * Reinitialize dictionary
	 */
	public void clear();
	
	
	/**
	 * Finds the value object corresponding to the specified key. If there is no
	 * value that has the given key it will return null. If there are more than
	 * one value objects with the same key, one of the value objects will be
	 * chosen arbitrary and returned.
	 * 
	 * @param k
	 *            The key of the record to find.
	 * @return A record matching "k" (null if none exists). If multiple records
	 *         match, return an arbitrary one.
	 */
	public E find(Key k);
	
	
	/**
	 * Insert a record
	 * 
	 * @param k
	 *            The key for the record being inserted.
	 * @param e
	 *            The record being inserted.
	 */
	public void insert(Key k, E e);
	
	
	/**
	 * Remove and return a record.
	 * 
	 * @param k
	 *            The key of the record to be removed.
	 * @return A matching record. If multiple records match "k", remove an
	 *         arbitrary one. Return null if no record with key "k" exists.
	 */
	public E remove(Key k);
	
	
	/**
	 * Remove and return an arbitrary record from dictionary.
	 * 
	 * @return the record removed, or null if none exists.
	 */
	public E removeAny();
	
	
	/**
	 * Gets the number of records in the dictionary.
	 * 
	 * @return The number of records in the dictionary.
	 */
	public int size();
};

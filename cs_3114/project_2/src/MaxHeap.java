/**
 * Source code example for "A Practical Introduction to Data Structures and
 * Algorithm Analysis, 3rd Edition (Java)" by Clifford A. Shaffer Copyright
 * 2008-2011 by Clifford A. Shaffer
 */

/**
 * Max heap implementation.
 * 
 * @param <E>
 *            The generic type
 * @author Elmer Landaverde
 * @version Sep 11, 2013
 */
public class MaxHeap<E extends ComparableIndex<? super E>>
{
	
	/**
	 * The heap array were all the nodes of the tree are stored.
	 */
	private E[]	heap;
	
	/**
	 * The maximum size of the heap.
	 */
	private int	size;
	
	/**
	 * The number of things in the heap.
	 */
	private int	n;
	
	
	/**
	 * Constructor supporting pre-loading of heap contents
	 * 
	 * @param h
	 *            The array where the E object will be stored.
	 * @param num
	 *            The number of non-null object initially stored in h.
	 * @param max
	 *            The maximum number of objects for the heap.
	 */
	public MaxHeap(E[] h, int num, int max)
	{
		heap = h;
		n = num;
		size = max;
		buildheap();
	}
	
	
	/**
	 * Heapify contents of Heap
	 */
	public void buildheap()
	{
		for (int i = n / 2 - 1; i >= 0; i--)
		{
			siftdown(i);
		}
	}
	
	
	/**
	 * Gets the size of the heap.
	 * 
	 * @return Current size of the heap
	 */
	public int heapsize()
	{
		return n;
	}
	
	
	/**
	 * Insert val into heap
	 * 
	 * @param val
	 *            The object to be inserted to the heap.
	 */
	public void insert(E val)
	{
		assert n < size : "Heap is full";
		int curr = n++;
		heap[curr] = val; // Start at end of heap
		// Now sift up until curr's parent's key > curr's key
		while ((curr != 0) && (heap[curr].compareTo(heap[parent(curr)]) > 0))
		{
			// Swap the indexes of the QueueLog objects inside of the HeapLog
			// objects
			DSutil.swapIndexables(
				heap[curr].getIndexable(),
				heap[parent(curr)].getIndexable());
			DSutil.swap(heap, curr, parent(curr));
			curr = parent(curr);
		}
	}
	
	
	/**
	 * Determines if the given position corresponds to a leaf.
	 * 
	 * @param pos
	 *            The position of the node.
	 * @return True if pos a leaf position, false otherwise
	 */
	public boolean isLeaf(int pos)
	{
		return (pos >= n / 2) && (pos < n);
	}
	
	
	/**
	 * Gets the position of the left child
	 * 
	 * @param pos
	 *            The position of the node.
	 * @return Position for left child of pos
	 */
	public int leftchild(int pos)
	{
		if (pos >= n / 2)
		{
			return -1;
		}
		return 2 * pos + 1;
	}
	
	
	/**
	 * Gets the position of the parent
	 * 
	 * @param pos
	 *            The position of the node
	 * @return Position for parent
	 */
	public int parent(int pos)
	{
		if (pos <= 0)
		{
			return -1;
		}
		return (pos - 1) / 2;
	}
	
	
	/**
	 * Remove and return element at specified position
	 * 
	 * @param pos
	 *            The position of the element to remove.
	 * @return Return the element that was removed.
	 */
	public E remove(int pos)
	{
		if ((pos < 0) || (pos >= n))
		{
			return null;
		}
		if (pos == (n - 1))
		{
			n--; // Last element, no work to be done
		}
		else
		{
			int updatedN = --n;
			// Swap the indexes of the QueueLog objects inside of the HeapLog
			// objects
			DSutil
				.swapIndexables(heap[pos].getIndexable(), heap[updatedN].getIndexable());
			DSutil.swap(heap, pos, updatedN); // Swap with last value
			// If we just swapped in a big value, push it up
			while ((pos > 0) && (heap[pos].compareTo(heap[parent(pos)]) > 0))
			{
				// Swap the indexes of the QueueLog objects inside of the
				// HeapLog objects
				DSutil.swapIndexables(
					heap[pos].getIndexable(),
					heap[parent(pos)].getIndexable());
				DSutil.swap(heap, pos, parent(pos));
				pos = parent(pos);
			}
			if (n != 0)
			{
				siftdown(pos); // If it is little, push down
			}
		}
		return heap[n];
	}
	
	
	/**
	 * Remove and return maximum value
	 * 
	 * @return Returns the maximum value element that was just removed.
	 */
	public E removemax()
	{
		if (n <= 0)
		{
			return null;
		}
		int temp = --n;
		// Swap the indexes of the QueueLog objects inside of the HeapLog
		// objects
		DSutil.swapIndexables(heap[0].getIndexable(), heap[temp].getIndexable());
		DSutil.swap(heap, 0, temp); // Swap maximum with last value
		if (n != 0)
		{
			// Not on last element
			siftdown(0); // Put new heap root val in correct place
		}
		return heap[n];
	}
	
	
	/**
	 * Gets the position of the right child
	 * 
	 * @param pos
	 *            The position of the node
	 * @return Position for right child of pos
	 */
	public int rightchild(int pos)
	{
		if (pos >= (n - 1) / 2)
		{
			return -1;
		}
		return 2 * pos + 2;
	}
	
	
	/**
	 * Put element in its correct place
	 * 
	 * @param pos
	 *            The position of the object to sift down.
	 */
	private void siftdown(int pos)
	{
		assert (pos >= 0) && (pos < n) : "Illegal heap position";
		while (!isLeaf(pos))
		{
			int j = leftchild(pos);
			if ((j < (n - 1)) && (heap[j].compareTo(heap[j + 1]) < 0))
			{
				j++; // j is now index of child with greater value
			}
			if (heap[pos].compareTo(heap[j]) >= 0)
			{
				return;
			}
			// Swap the indexes of the QueueLog objects inside of the HeapLog
			// objects
			DSutil.swapIndexables(heap[pos].getIndexable(), heap[j].getIndexable());
			DSutil.swap(heap, pos, j);
			pos = j; // Move down
		}
	}
}

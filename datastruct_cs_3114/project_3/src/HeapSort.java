/**
 * This class takes in an array of short records. It first builds the heap by
 * organizing the array, and then the user can choose to sort the heap by
 * removing the largest element until the heap is empty.
 *
 * @author Elmer Landaverde
 * @version Nov 6, 2013
 */
public class HeapSort
{

	/**
	 * The array that stores the short record objects.
	 */
	private ShortArray	recordArray;

	/**
	 * The number of elements in the heap.
	 */
	private long		n;


	/**
	 * Create a new MaxHeap object.
	 *
	 * @param h
	 *            The ShortArray object.
	 * @param num
	 *            The number of elements in the list.
	 * @throws Exception
	 *             Problem with the buffer.
	 */
	public HeapSort(ShortArray h, long num)
		throws Exception
	{
		recordArray = h;
		n = num;
		buildheap();
	}


	/**
	 * Sorts the heap array in ascending order.
	 *
	 * @throws Exception
	 *             Problem with the buffer.
	 */
	public void sort()
		throws Exception
	{
		// The heap constructor invokes the buildheap method
		while (n != 0)
		{
			removemax();
		}
	}


	/**
	 * Builds the heap from its initial state.
	 *
	 * @throws Exception
	 *             Problem with the buffer.
	 */
	private void buildheap()
		throws Exception
	{
		for (long i = n / 2 - 1; i >= 0; i--)
		{
			siftdown(i);
		}
	}


	/**
	 * Determines if the element at the given position is a leaf.
	 *
	 * @param pos
	 *            The position.
	 * @return True if the element at the specified position is a leaf.
	 */
	private boolean isLeaf(long pos)
	{
		return (pos >= n / 2) && (pos < n);
	}


	/**
	 * Determines the index of the left child of the element at the given
	 * position.
	 *
	 * @param pos
	 *            The position.
	 * @return The index of the left child of the element at the given position.
	 */
	private long leftchild(long pos)
	{
		return 2 * pos + 1;
	}


	/**
	 * Removed the largest element in the heap.
	 *
	 * @throws Exception
	 *             Problem with the buffer.
	 */
	private void removemax()
		throws Exception
	{
		HeapSort.swap(recordArray, 0, --n); // Swap maximum with last value
		if (n != 0)
		{// Not on last element
			siftdown(0); // Put new heap root val in correct place
		}
	}


	/**
	 * Places the element at the given position in the right index of the array.
	 *
	 * @param pos
	 *            The position.
	 * @throws Exception
	 *             Problem with the buffer.
	 */
	private void siftdown(long pos)
		throws Exception
	{
		ShortRecord current = recordArray.get(pos);
		while (!isLeaf(pos))
		{
			long j = leftchild(pos);
			ShortRecord leftChild = recordArray.get(j);
			ShortRecord rightChild = null;
			ShortRecord child = leftChild;
			if (j < (n - 1))
			{
				rightChild = recordArray.get(j + 1);
				if (leftChild.compareTo(rightChild) < 0)
				{
					j++; // j is now index of child with greater value
					child = rightChild;
				}
			}
			if (current == null)
			{
				current = recordArray.get(pos);
			}
			// ShortRecord current = recordArray.get(pos);
			if (current.compareTo(child) >= 0)
			{
				return;
			}
			HeapSort.swapAlt(recordArray, current, pos, child, j);
			pos = j; // Move down
		}
	}


	/**
	 * Swap two objects in an array
	 *
	 * @param array
	 *            The array
	 * @param p1
	 *            Index of one Object in A
	 * @param p2
	 *            Index of another Object A
	 * @throws Exception
	 *             Problem with the buffer.
	 */
	private static void swap(ShortArray array, long p1, long p2)
		throws Exception
	{
		ShortRecord r2 = array.get(p2);
		ShortRecord r1 = array.get(p1);
		array.set(p2, r1);
		array.set(p1, r2);
	}


	/**
	 * Swap two object in an array.
	 *
	 * @param array
	 *            The array.
	 * @param rec1
	 *            The first element.
	 * @param p1
	 *            The position of the first element.
	 * @param rec2
	 *            The second element.
	 * @param p2
	 *            The position of the second element.
	 * @throws Exception
	 *             Could not write to file.
	 */
	private static void swapAlt(
		ShortArray array,
		ShortRecord rec1,
		long p1,
		ShortRecord rec2,
		long p2)
		throws Exception
	{
		array.set(p2, rec1);
		array.set(p1, rec2);
	}
}

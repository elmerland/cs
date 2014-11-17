/**
 * This class represents a block of memory in disk.
 *
 * @author Elmer Landaverde
 * @version Dec 3, 2013
 */
public class MemoryBlock
{

	/**
	 * The memory address of the starting position.
	 */
	private long	position;

	/**
	 * The size in bytes of this memory block.
	 */
	private long	size;


	/**
	 * Create a new MemoryBlock object.
	 *
	 * @param handle
	 *            The handle that represents the start of the memory block.
	 * @param size
	 *            The byte size of the memory block.
	 */
	public MemoryBlock(MemoryHandle handle, long size)
	{
		position = handle.getPosition();
		this.size = size;
	}


	/**
	 * Create a new MemoryBlock object.
	 *
	 * @param pos
	 *            The starting position of the memory block.
	 * @param size
	 *            The byte size of the memory block.
	 */
	public MemoryBlock(long pos, long size)
	{
		this.position = pos;
		this.size = size;
	}


	/**
	 * Sets the starting position of this memory block.
	 *
	 * @param pos
	 *            The new starting position.
	 */
	public void setPosition(long pos)
	{
		this.position = pos;
	}


	/**
	 * Sets the size of this memory block.
	 *
	 * @param size
	 *            The new block size.
	 */
	public void setSize(long size)
	{
		this.size = size;
	}


	/**
	 * Gets the starting memory position of this memory block.
	 *
	 * @return The memory position.
	 */
	public long getPosition()
	{
		return position;
	}


	/**
	 * Gets the size of this memory block.
	 *
	 * @return The size of the memory block.
	 */
	public long getSize()
	{
		return size;
	}


	/**
	 * Gets the memory position where this memory block ends. NOTE: Ending
	 * position is non-inclusive.
	 *
	 * @return The ending position of this block.
	 */
	public long getEndPosition()
	{
		return position + size;
	}

}

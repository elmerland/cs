/**
 * This class represents that starting address of a block of memory in disk.
 *
 * @author Elmer Landaverde
 * @version Nov 17, 2013
 */
public class MemoryHandle
{

	private long	memPosition;


	/**
	 * Create a new MemoryHandle object.
	 *
	 * @param pos
	 *            The memory position of this memory block.
	 */
	public MemoryHandle(long pos)
	{
		memPosition = pos;
	}


	/**
	 * Gets the starting memory position of this memory block.
	 *
	 * @return The memory position.
	 */
	public long getPosition()
	{
		return memPosition;
	}
}

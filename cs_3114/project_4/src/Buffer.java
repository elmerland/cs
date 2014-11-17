import java.nio.ByteBuffer;

/**
 * The Buffer class contains one array of bytes that represent the information
 * at a specific block in a file.
 *
 * @author Elmer Landaverde
 * @version Nov 4, 2013
 */
public class Buffer
{

	/**
	 * The byte buffer that holds the byte array for this buffer.
	 */
	private ByteBuffer	bytes;

	/**
	 * Boolean variable to determine if the information in this buffer has being
	 * modified.
	 */
	private boolean		dirty;

	/**
	 * The uniquely identifying ID for this buffer. The ID is used to determine
	 * the starting position of the block of information that is contained in
	 * this buffer.
	 */
	private int			ID;


	/**
	 * Create a new Buffer object.
	 *
	 * @param capacity
	 *            The number of bytes to be contained in this buffer.
	 */
	public Buffer(int capacity)
	{
		ID = -1;
		dirty = false;
		bytes = ByteBuffer.allocate(capacity);
	}


	/**
	 * Gets the uniquely identifying ID for this buffer.
	 *
	 * @return The ID.
	 */
	public int getID()
	{
		return ID;
	}


	/**
	 * Sets the ID of this buffer to the one specified in by 'newID'.
	 *
	 * @param newID
	 *            The new ID.
	 */
	public void setID(int newID)
	{
		ID = newID;
	}


	/**
	 * Marks this buffer as empty. Both the ID and the dirty bit are reset.
	 * NOTE: The information in this buffer is not erased, instead it just
	 * appears as available.
	 */
	public void flush()
	{
		bytes.clear();
		ID = -1;
		dirty = false;
	}


	/**
	 * Determines if the information in this buffer has being modified.
	 *
	 * @return True if the information in this buffer has being modified. False
	 *         if it hasn't.
	 */
	public boolean isDirty()
	{
		return dirty;
	}


	/**
	 * Gets the byte array that this buffer is storing. NOTE: All changes made
	 * to the array returned by the method will be reflected in the buffer.
	 * However, this change will not be recorded. This method should only be
	 * used by the BufferPool class when loading a new block of data to this
	 * buffer.
	 *
	 * @return The byte array that this buffer is storing.
	 */
	public byte[] getArray()
	{
		return bytes.array();
	}


	/**
	 * Copies the give byte array to a position in this buffer specified by
	 * 'offset'.
	 *
	 * @param src
	 *            The bytes to be copied to this buffer.
	 * @param offset
	 *            The byte offset that indicates where the new bytes will be
	 *            copied to.
	 * @return True if the bytes where copied successfully. False if they could
	 *         not be written to the buffer.
	 */
	public boolean setBytes(byte[] src, int offset)
	{
		bytes.position(offset);
		bytes.put(src);
		dirty = true;
		return true;
	}


	/**
	 * Copies the bytes in this buffer starting at the position specified by the
	 * 'offset' and copies them to the provided byte array.
	 *
	 * @param dst
	 *            The byte array to where the bytes will be copied to.
	 * @param offset
	 *            The offset that specified the starting position of the bytes
	 *            to be copied.
	 * @return True if the bytes where copied successfully. False if they were
	 *         not copied to the destination array.
	 */
	public boolean getBytes(byte[] dst, int offset)
	{
		bytes.position(offset);
		bytes.get(dst);
		return true;
	}

}

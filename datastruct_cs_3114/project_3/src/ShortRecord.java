import java.nio.ByteOrder;
import java.nio.ByteBuffer;

/**
 * This class represents a record made up of a key and a value. The key is of
 * type short and the value is also of type short. However, despite the key and
 * value being shorts, the data is represented using a 4 byte array.
 *
 * @author Elmer Landaverde
 * @version Nov 5, 2013
 */
public class ShortRecord
	implements Comparable<ShortRecord>
{

	/**
	 * The size of this record measured in bytes.
	 */
	public static final int		SIZE		= 4;

	/**
	 * The index of the key.
	 */
	private static final int	KEY_INDEX	= 0;

	/**
	 * The index of the value.
	 */
	private static final int	VAL_INDEX	= 2;

	/**
	 * A byte buffer used to convert the byte array into short variables.
	 */
	private ByteBuffer			bb			= ByteBuffer.allocate(SIZE);


	/**
	 * Create a new ShortRecord object.
	 *
	 * @param pos
	 *            The file position of this record.
	 * @param bytes
	 *            The byte array of size 'SIZE'
	 */
	public ShortRecord(long pos, byte[] bytes)
	{
		bb.clear();
		bb.order(ByteOrder.BIG_ENDIAN);
		bb.put(bytes);
	}


	/**
	 * Gets the byte array that this record represents.
	 *
	 * @return The byte array of this record.
	 */
	public byte[] getBytes()
	{
		return bb.array();
	}


	/**
	 * Gets the key for this record.
	 *
	 * @return The key.
	 */
	public short key()
	{
		return bb.getShort(KEY_INDEX);
	}


	/**
	 * Gets the value for this record.
	 *
	 * @return The value.
	 */
	public short value()
	{
		return bb.getShort(VAL_INDEX);
	}


	@Override
	public int compareTo(ShortRecord s)
	{

		return key() - s.key();
	}


	@Override
	public String toString()
	{
		return key() + "\t" + value();
	}
}

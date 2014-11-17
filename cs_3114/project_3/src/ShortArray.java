/**
 * This class represents an array of short values stored in disk. It gets and
 * writes each record using a buffer pool object. This class does not have an
 * array of record objects, instead it gets and writes record as they are need
 * from disk.
 *
 * @author Elmer Landaverde
 * @version Nov 5, 2013
 */
public class ShortArray
{

	private BufferPoolADT	bp;


	/**
	 * Create a new ShortArray object.
	 *
	 * @param buffers
	 *            The buffer pool object.
	 */
	public ShortArray(BufferPoolADT buffers)
	{
		bp = buffers;
	}


	/**
	 * Gets the size of this short record array.
	 *
	 * @return The size of this array.
	 */
	public long size()
	{
		return bp.getFileSize() / ShortRecord.SIZE;
	}


	/**
	 * Gets the short record at the specified index.
	 *
	 * @param index
	 *            The index of the record to be returned.
	 * @return The record at the specified index.
	 * @throws Exception
	 *             Could not read file.
	 */
	public ShortRecord get(long index)
		throws Exception
	{
		long pos = ShortRecord.SIZE * index;
		byte[] b = new byte[4];
		bp.getBytes(b, pos);
		ShortRecord r = new ShortRecord(pos, b);
		return r;
	}


	/**
	 * Writes the given short record at the specified index.
	 *
	 * @param index
	 *            The index.
	 * @param r
	 *            The record to be written.
	 * @throws Exception
	 *             Could not write to file.
	 */
	public void set(long index, ShortRecord r)
		throws Exception
	{
		long pos = ShortRecord.SIZE * index;
		bp.setBytes(r.getBytes(), pos);
	}


	/**
	 * Gets a string representation of this ShortArray object.
	 *
	 * @return A string representation of this object.
	 * @throws Exception
	 *             Could not read file.
	 */
	public String getBlocks()
		throws Exception
	{
		StringBuilder sb = new StringBuilder();
		long recordsPerBlock = BufferPool.BUFFER_SIZE / ShortRecord.SIZE;
		long blocks = size() / recordsPerBlock;
		ShortRecord r = null;
		for (long i = 0; i < blocks; i++)
		{
			sb.append("\t");
			r = get(i * recordsPerBlock);
			sb.append(r.toString());
			if ((i + 1) % 8 == 0)
			{
				sb.append("\n");
			}
		}
		return sb.toString();
	}

}

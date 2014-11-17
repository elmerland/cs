import java.io.File;
import java.io.RandomAccessFile;

/**
 * The buffer pool class is in charge of managing reads and writes to a specific
 * file, by permitting the reading and writing to the file in a much more
 * efficient manner.
 *
 * @author Elmer Landaverde
 * @version Nov 4, 2013
 */
public class BufferPool
	implements BufferPoolADT
{

	/**
	 * The number of bytes in each buffer.
	 */
	public static final int		BUFFER_SIZE	= 4096;

	/**
	 * The number of cache misses in this buffer pool session.
	 */
	private int					cacheMisses	= 0;

	/**
	 * The number of cache hits in this buffer pool session.
	 */
	private int					cacheHits	= 0;

	/**
	 * The number of times a block was read from disk.
	 */
	private int					diskReads	= 0;

	/**
	 * The number of times a block was written to disk.
	 */
	private int					diskWrites	= 0;

	/**
	 * The file where all the blocks are written and read from.
	 */
	private RandomAccessFile	srcFile;

	/**
	 * The access privileges for the random access file.
	 */
	private String				mode		= "rw";

	/**
	 * The path name of the file to be read and written to.
	 */
	private String				fileName;

	/**
	 * The number of buffers contained in this buffer pool.
	 */
	private int					poolSize;

	/**
	 * The array of buffers that contains all the block of information.
	 */
	private Buffer[]			buffers;

	/**
	 * The index to the bottom buffer. The bottom buffer is the last non-empty
	 * buffer in the buffer array.
	 */
	private int					bottomBuffer;

	/**
	 * The size of the source file measured in number of bytes.
	 */
	private long				fileSize;


	/**
	 * Create a new BufferPool object.
	 *
	 * @param fileName
	 *            The path name of the file to be read and written to.
	 * @param bufferNum
	 *            The number of buffers to be used.
	 * @throws Exception
	 *             Could not initialize buffer.
	 */
	public BufferPool(String fileName, int bufferNum)
		throws Exception
	{
		this.fileName = fileName;
		poolSize = bufferNum;
		initRandomAccessFile();

		initBuffers();
	}


	@Override
	public void flushBuffers()
		throws Exception
	{
		closeBuffers();
	}


	@Override
	public boolean closeBufferPool()
		throws Exception
	{
		// Flush all remaining buffers.
		closeBuffers();
		srcFile.close();
		return true;
	}


	/**
	 * Initializes the random access file object.
	 *
	 * @throws Exception
	 *             Cannot initialize random access file.
	 */
	private void initRandomAccessFile()
		throws Exception
	{
		File file = new File(fileName);

		srcFile = new RandomAccessFile(file, mode);
		fileSize = srcFile.length();
	}


	/**
	 * Initializes the array of buffers for this buffer pool.
	 */
	private void initBuffers()
	{
		// Initialize byte buffers.
		buffers = new Buffer[poolSize];
		for (int i = 0; i < poolSize; i++)
		{
			buffers[i] = new Buffer(BUFFER_SIZE);
		}

		bottomBuffer = -1;
	}


	/**
	 * Gets the number of non-empty buffers that are contained in this buffer
	 * pool.
	 *
	 * @return The number of non-empty buffers.
	 */
	public int getNumberOfActiveBuffers()
	{
		return bottomBuffer + 1;
	}


	@Override
	public long getFileSize()
	{
		return fileSize;
	}


	@Override
	public boolean setBytes(byte[] bytes, long pos)
		throws Exception
	{
		// Verify input.
		if (!isValidByteRange(bytes.length, pos))
		{
			System.out.println("Error: Invalid byte range.");
			return false;
		}
		// Move buffer object to top.
		getBuffer(pos);

		// Get the buffer object.
		Buffer b = buffers[0];
		// Get the block relative offset.
		int offset = (int)(pos % BUFFER_SIZE);
		// Read bytes into the provided byte array.
		b.setBytes(bytes, offset);
		return true;
	}


	@Override
	public boolean getBytes(byte[] bytes, long pos)
		throws Exception
	{
		// Verify input.
		if (!isValidByteRange(bytes.length, pos))
		{
			System.out.println("Error: Invalid byte range.");
			return false;
		}
		// Move buffer object to top.
		getBuffer(pos);
		// Get the buffer object.
		Buffer b = buffers[0];
		// Get the block relative offset.
		int offset = (int)(pos % BUFFER_SIZE);
		// Read bytes into the provided byte array.
		b.getBytes(bytes, offset);
		return true;
	}


	/**
	 * This method places a buffer containing the desired position at the top of
	 * the buffer array. It first looks through the list of buffers to see if
	 * the desired block is already there. If it is not there then a new block
	 * is read from disk. When the desired block is read from disk or found in
	 * the array, it is moved to the top of the array placing it at index 0.
	 *
	 * @param pos
	 *            The file position inside of the block to be moved to the top.
	 * @return True if the buffer could be found (or read) and moved to the top
	 *         of the buffer array.
	 * @throws Exception
	 *             Could not read file.
	 */
	private boolean getBuffer(long pos)
		throws Exception
	{
		int id = (int)(pos / BUFFER_SIZE);
		int index = -1;
		for (int i = 0; i < poolSize; i++)
		{
			if (buffers[i].getID() == id)
			{
				index = i;
			}
		}
		if (index == -1)
		{
			// Block is not in buffer pool.
			// Read in a new block to buffer pool.
			readBlock(pos);
			cacheMisses++;
		}
		else
		{
			// Block is in buffer pool.
			moveToTop(index);
			cacheHits++;
		}
		return true;
	}


	/**
	 * Reads a new block from disk and writes it to a buffer, it then moves this
	 * buffer to the top of the buffer array. If the buffer array is full, it
	 * flushed the last recently used buffer. If the buffer array is not full
	 * then it reads the new block to the next available empty buffer. Once the
	 * new buffer has being read and placed in the array list, it is moved to
	 * the top of the buffer array.
	 *
	 * @param pos
	 *            The position inside of the block to be read.
	 * @exception Exception
	 *                Could not read file.
	 * @return True if the desired block could be read and placed at the top of
	 *         the list. False otherwise.
	 */
	private boolean readBlock(long pos)
		throws Exception
	{
		// Check if buffer pool is full.
		if (isBufferPoolFull())
		{
			flushBuffer();
		}

		// Get block ID and block position.
		int id = (int)(pos / BUFFER_SIZE);
		long blockStart = (pos / BUFFER_SIZE) * BUFFER_SIZE;

		// Read new block into memory.
		srcFile.seek(blockStart);
		Buffer b = buffers[bottomBuffer + 1];
		srcFile.read(b.getArray());
		b.setID(id);

		bottomBuffer++;
		moveToTop(bottomBuffer);
		diskReads++;
		return true;
	}


	/**
	 * Determines if the byte array size and file position is valid. For these
	 * two parameters to be valid the following condition have to be met:
	 * <p>
	 * - pos + size < file size.<br>
	 * - pos and pos + size have to be in the same block.<br>
	 * - size has to be greater than zero.<br>
	 * </p>
	 *
	 * @param size
	 *            The size of the byte array.
	 * @param pos
	 *            The starting position of the byte array.
	 * @return True if the size and file position are valid. False otherwise.
	 */
	private boolean isValidByteRange(int size, long pos)
	{
		long startPos = pos;
		long endPos = pos + size - 1;
		int startBlock = (int)(startPos / BUFFER_SIZE);
		int endBlock = (int)(endPos / BUFFER_SIZE);
		// Check if byte range is zero.
		if (size == 0)
		{
			System.out.println("Error: Byte array is of length zero.");
			return false;
		}
		// Check if the byte range is within the same block.
		if (startBlock != endBlock)
		{
			System.out.println("Error: Byte array extends over two blocks. Pos: " + pos
				+ " Size: " + size);
			return false;
		}
		// Check if the byte range is within the current file.
		if (endPos >= fileSize)
		{
			System.out.println("Error: Byte array extends past end of file.");
			return false;
		}
		return true;
	}


	/**
	 * Flushes all of the non-empty buffers.
	 *
	 * @return True if all the non-empty buffer were flushed successfully.
	 * @throws Exception
	 *             Cannot write to file.
	 */
	private boolean closeBuffers()
		throws Exception
	{
		// Clear all the non-empty buffers.
		while (bottomBuffer >= 0)
		{
			// Clear buffer.
			clearBuffer(buffers[bottomBuffer]);
			// Update the bottom buffer index.
			bottomBuffer--;
		}
		return true;
	}


	/**
	 * Flushes the last buffer in the array.
	 *
	 * @return True if the last buffer in the array was flushed. False
	 *         otherwise.
	 * @throws Exception
	 *             Cannot write to file.
	 */
	private boolean flushBuffer()
		throws Exception
	{
		// Get buffer to be flushed.
		Buffer b = buffers[bottomBuffer];
		// Flush buffer.
		clearBuffer(b);
		// Update bottom buffer index.
		bottomBuffer--;
		return true;
	}


	/**
	 * Flushes the specified buffer. If the buffer has being modified it writes
	 * it to disk before flushing.
	 *
	 * @param b
	 *            The buffer to be flushed.
	 * @return True if the buffer was flushed successfully. False otherwise.
	 * @throws Exception
	 *             Cannot write to buffer.
	 */
	private boolean clearBuffer(Buffer b)
		throws Exception
	{
		// Check if buffer has changed.
		if (b.isDirty())
		{
			// Write buffer before flushing.

			// Get buffer position
			long pos = b.getID() * BUFFER_SIZE;
			// Set file pointer.
			srcFile.seek(pos);
			// Write to file.
			srcFile.write(b.getArray());
			diskWrites++;
		}
		// Flush buffer
		b.flush();
		return true;
	}


	/**
	 * Determines if the buffer array is full. The buffer array is full when all
	 * the buffers are non-empty.
	 *
	 * @return True if the buffer array is full. False otherwise.
	 */
	private boolean isBufferPoolFull()
	{
		if (bottomBuffer == (poolSize - 1))
		{
			return true;
		}
		else
		{
			return false;
		}
	}


	/**
	 * Moves the buffer at the indicated index to the top of the buffer array.
	 *
	 * @param index
	 *            The index of the buffer to be moved to the top of the buffer
	 *            array.
	 * @return True if the buffer was moved to the top of the buffer array.
	 *         False otherwise.
	 */
	private boolean moveToTop(int index)
	{
		while (index > 0)
		{
			swapBuffers(index, index - 1);
			index--;
		}

		return true;
	}


	/**
	 * It swaps the buffer at index i with the buffer at position j.
	 *
	 * @param i
	 *            The index of the buffer.
	 * @param j
	 *            The index of the buffer.
	 * @return True if the two buffers where swapped successfully.
	 */
	private boolean swapBuffers(int i, int j)
	{
		// Swap buffers
		Buffer bTemp = buffers[i];
		buffers[i] = buffers[j];
		buffers[j] = bTemp;

		return true;
	}


	/**
	 * Gets an integer array that contains all the metrics for this buffer pool
	 * in the following order:
	 * <p>
	 * [0] Cache Misses.<br>
	 * [1] Cache Hits.<br>
	 * [2] Disk Reads.<br>
	 * [3] Disk Writes.<br>
	 * </p>
	 *
	 * @return The list of metrics for this buffer pool.
	 */
	public int[] getMetrics()
	{
		int[] result = { cacheMisses, cacheHits, diskReads, diskWrites };
		return result;
	}

}

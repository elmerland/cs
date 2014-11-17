import java.nio.ByteBuffer;
import java.util.ArrayList;

/**
 * This class is in charge of storing and accessing data from disk. However this
 * class does not have direct access to disk, instead it uses the BufferPoolADT
 * to do this.
 *
 * @author Elmer Landaverde
 * @version Nov 16, 2013
 */
public class MemoryManager
{

	/**
	 * Holds a list of all the free blocks in the file.
	 */
	private ArrayList<MemoryBlock>	freeList;

	/**
	 * Holds the index of the current item in the free list.
	 */
	private int						currIndex;


	/**
	 * Create a new MemoryManager object.
	 */
	public MemoryManager()
	{
		freeList = new ArrayList<MemoryBlock>();
		currIndex = 0;
	}


	/**
	 * Insert the specified byte to memory. The location of the specified byte
	 * will be referred to by the memory handle that is returned by this method.
	 *
	 * @param bytes
	 *            The bytes to insert to memory.
	 * @return The memory handle to reference the bytes.
	 * @throws Exception
	 *             Could not write to file.
	 */
	public MemoryHandle insert(byte[] bytes)
		throws Exception
	{
		// Add the length of the byte array to the front of the array.
		ByteBuffer buffer = ByteBuffer.allocate(bytes.length + 2);
		buffer.position(0);
		buffer.putShort((short)bytes.length);
		buffer.put(bytes);
		byte modBytes[] = buffer.array();

		// Find a free block to put the byte array in.
		int index = -1;
		while (index == -1)
		{
			index = findFreeSpace(modBytes.length);
			if (index == -1)
			{
				increaseSpace();
				mergeFreeBlocks();
			}
		}

		// Mark the bytes as reserved and update the free block array.
		return reserveBytes(index, modBytes);
	}


	/**
	 * Get a copy of the bytes stored at the position specified by the memory
	 * handle.
	 *
	 * @param handle
	 *            The memory handle.
	 * @return The bytes stored at the handle.
	 * @throws Exception
	 *             Could not read from disk.
	 */
	public byte[] getRecord(MemoryHandle handle)
		throws Exception
	{
		// Get the position and size of the memory block.
		long startPos = handle.getPosition();
		long size = getBlockSize(handle);

		// Get the block byte array
		BufferPoolADT bp = DiskBintree.getBufferPool();
		byte[] bytes = new byte[(int)size];
		bp.getBytes(bytes, startPos + 2);
		return bytes;
	}


	/**
	 * Releases the memory block specified by the given memory handle and adds
	 * that memory block to the list of free blocks.
	 *
	 * @param handle
	 *            The memory handle.
	 * @throws Exception
	 *             Could not read from disk.
	 */
	public void release(MemoryHandle handle)
		throws Exception
	{
		// Find index to insert new free block.
		int i = 0;
		long handleStart = handle.getPosition();
		while (i < freeList.size() && freeList.get(i).getPosition() < handleStart)
		{
			i++;
		}

		// Add free block
		if (i == freeList.size())
		{
			freeList.add(new MemoryBlock(handle, getBlockSize(handle) + 2));
		}
		else
		{
			freeList.add(i, new MemoryBlock(handle, getBlockSize(handle) + 2));
		}
		// Merge free blocks.
		mergeFreeBlocks();
	}


	/**
	 * Determines the length of the memory block that is represented by the
	 * memory handle.
	 *
	 * @param handle
	 *            The memory handle.
	 * @return The size of the block located at the memory handle.
	 * @throws Exception
	 *             Could not read bytes from disk.
	 */
	private long getBlockSize(MemoryHandle handle)
		throws Exception
	{
		// Get bytes from disk.
		BufferPoolADT bp = DiskBintree.getBufferPool();
		byte[] bytes = new byte[2];
		bp.getBytes(bytes, handle.getPosition());

		// Get short from bytes.
		ByteBuffer buffer = ByteBuffer.wrap(bytes);
		return buffer.getShort();
	}


	/**
	 * This method reserves the first bytes.length bytes from the block of
	 * memory represented by the memory handle at the specified index.
	 *
	 * @param blockIndex
	 *            The index for the memory handle that represents the free space
	 *            where the bytes will be inserted.
	 * @param bytes
	 *            The bytes to be inserted.
	 * @return The memory handle for the bytes that where inserted.
	 * @throws Exception
	 *             Could not write bytes to disk.
	 */
	private MemoryHandle reserveBytes(int blockIndex, byte[] bytes)
		throws Exception
	{
		// Get free block
		MemoryBlock freeBlock = freeList.get(blockIndex);
		// Create new reserved block
		MemoryHandle reserved = new MemoryHandle(freeBlock.getPosition());

		// Check if free block is occupied completely.
		if (freeBlock.getSize() == bytes.length)
		{
			freeList.remove(blockIndex);
		}
		else
		{
			// Update size and position of free block.
			long newPos = freeBlock.getPosition() + bytes.length;
			long newSize = freeBlock.getSize() - bytes.length;
			freeBlock.setPosition(newPos);
			freeBlock.setSize(newSize);
		}

		// Copy bytes to disk.
		BufferPoolADT bp = DiskBintree.getBufferPool();
		bp.setBytes(bytes, reserved.getPosition());

		updateGlobalIndex();
		return reserved;
	}


	/**
	 * Updates the global index to make sure that the current index is not out
	 * of bounds.
	 */
	private void updateGlobalIndex()
	{
		if (freeList.size() == 0)
		{
			currIndex = 0;
		}
		else
		{
			currIndex = currIndex % freeList.size();
		}
	}


	/**
	 * Iterates through the list of free blocks and finds the first block that
	 * is big enough to hold the specified space.
	 *
	 * @param space
	 *            The required space.
	 * @return The index of the first free block that satisfies the space
	 *         requirements. Returns -1 if no such block was found.
	 */
	private int findFreeSpace(long space)
	{
		if (freeList.size() == 0)
		{
			return -1;
		}
		int endIndex = currIndex;
		do
		{
			MemoryBlock b = freeList.get(currIndex);
			if (b.getSize() >= space)
			{
				return currIndex;
			}
			currIndex = (currIndex + 1) % freeList.size();
		}
		while (currIndex != endIndex);

		return -1;
	}


	/**
	 * Increases the space in the array by one block.
	 *
	 * @throws Exception
	 *             Could not increase file size.
	 */
	private void increaseSpace()
		throws Exception
	{
		BufferPoolADT bp = DiskBintree.getBufferPool();
		MemoryBlock b = new MemoryBlock(bp.getFileSize(), bp.getBufferSize());
		freeList.add(b);
		bp.incFileSize(1);
	}


	/**
	 * Merge all adjacent free blocks of memory.
	 */
	private void mergeFreeBlocks()
	{
		if (freeList.size() <= 1)
		{
			return;
		}
		MemoryBlock prev = null;
		MemoryBlock curr = null;
		int index = 1;
		int size = freeList.size();
		// Iterate through all blocks to find any adjacent ones.
		while (index < size)
		{
			prev = freeList.get(index - 1);
			curr = freeList.get(index);

			// If they are adjacent, merge the blocks.
			if (prev.getEndPosition() == curr.getPosition())
			{
				// Get starting position and size.
				long startPos = prev.getPosition();
				long length = prev.getSize() + curr.getSize();
				// Create new block.
				MemoryBlock newBlock = new MemoryBlock(startPos, length);
				// Replace and delete blocks.
				freeList.set(index - 1, newBlock);
				freeList.remove(index);
				// Update list size.
				size = freeList.size();
				// Update index.
				index--;
			}
			index++;
		}

		updateGlobalIndex();
	}
}

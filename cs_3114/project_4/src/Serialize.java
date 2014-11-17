import java.nio.ByteBuffer;

/**
 * This class is in charge of serializing and deserializing objects.
 *
 * @author Elmer Landaverde
 * @version Dec 3, 2013
 */
public class Serialize
{

	/**
	 * Stores the given node. It first serializes the node and the stores it to
	 * disk.
	 *
	 * @param node
	 *            The node to store to disk.
	 * @return The memory handle that points to the serialized node in disk.
	 * @throws Exception
	 *             Could not store to disk.
	 */
	public static MemoryHandle storeNodeToDisk(BinTreeNode node)
		throws Exception
	{
		MemoryManager mm = DiskBintree.getMemoryManager();
		return mm.insert(serialize(node));
	}


	/**
	 * Gets a node object from a memory handle. It gets the byte array belonging
	 * to the specified memory handle and then deserializes it and returns the
	 * node object.
	 *
	 * @param handle
	 *            The memory handle pointing to the serialized node object.
	 * @return The node object that is pointed to by the memory handle.
	 * @throws Exception
	 *             Could not read from disk.
	 */
	public static BinTreeNode getNodeFromDisk(MemoryHandle handle)
		throws Exception
	{
		MemoryManager mm = DiskBintree.getMemoryManager();
		return deserializeNode(mm.getRecord(handle));
	}


	/**
	 * Releases the memory block pointed to by the memory handle. If the memory
	 * handle points to a leaf node, it first releases the memory block of the
	 * watcher user and then the memory block of the node itself.
	 *
	 * @param handle
	 *            The memory handle pointing to the node to be removed.
	 * @throws Exception
	 *             Could not read from disk.
	 */
	public static void freeNode(MemoryHandle handle)
		throws Exception
	{
		BinTreeNode node = getNodeFromDisk(handle);
		MemoryManager mm = DiskBintree.getMemoryManager();
		if (node instanceof BLeafNode)
		{

			ByteBuffer bf = ByteBuffer.wrap(mm.getRecord(handle));
			MemoryHandle watcherHandle = new MemoryHandle(bf.getInt(1));
			mm.release(watcherHandle);
		}
		mm.release(handle);
	}


	/**
	 * Serializes the BinTreeNode and turns it into a byte array.
	 *
	 * @param node
	 *            The node to be serialized.
	 * @return The byte array representation of the node.
	 * @throws Exception
	 *             Could not write to disk.
	 */
	public static byte[] serialize(BinTreeNode node)
		throws Exception
	{
		if (node instanceof BInternalNode)
		{
			// Serialize internal node.
			ByteBuffer bf = ByteBuffer.allocate(9);
			bf.put((byte)0);
			bf.putInt((int)node.leftHandle().getPosition());
			bf.putInt((int)node.rightHandle().getPosition());
			return bf.array();
		}
		else if (node instanceof BLeafNode)
		{
			// Serialize leaf node.
			WatcherUser user = node.element();
			MemoryHandle userHandle = storeWatcherToDisk(user);
			ByteBuffer bf = ByteBuffer.allocate(5);
			bf.put((byte)1);
			bf.putInt((int)userHandle.getPosition());
			return bf.array();
		}
		else
		{
			// Serialize empty node.
			byte[] bytes = { 2 };
			return bytes;
		}
	}


	/**
	 * Converts the byte array representation to BinTreeNode object.
	 *
	 * @param bytes
	 *            The byte array representation of the node.
	 * @return The node.
	 * @throws Exception
	 *             Could not read from disk.
	 */
	public static BinTreeNode deserializeNode(byte[] bytes)
		throws Exception
	{
		ByteBuffer bf = ByteBuffer.wrap(bytes);
		byte typeCode = bf.get();
		if (typeCode == 0)
		{
			// Deserialize internal node.
			MemoryHandle leftHandle = new MemoryHandle(bf.getInt());
			MemoryHandle rightHandle = new MemoryHandle(bf.getInt());
			BInternalNode intNode = new BInternalNode(leftHandle, rightHandle);
			return intNode;
		}
		else if (typeCode == 1)
		{
			// Deserialize leaf node.
			MemoryHandle handle = new MemoryHandle(bf.getInt());
			WatcherUser user = getWatcherFromDisk(handle);
			BLeafNode leafNode =
				new BLeafNode(user, user.getLongitude(), user.getLatitude());
			return leafNode;
		}
		else
		{
			// Deserialize empty node.
			return BinTree.emptyNode;
		}
	}


	/**
	 * Serializes this object and writes it to disk using the memory manager.
	 *
	 * @param user
	 *            The watcher user object to serialize.
	 * @return The memory handle to the serialized version of this object.
	 * @throws Exception
	 *             Could not write to disk.
	 */
	public static MemoryHandle storeWatcherToDisk(WatcherUser user)
		throws Exception
	{
		MemoryManager mm = DiskBintree.getMemoryManager();
		return mm.insert(serialize(user));
	}


	/**
	 * Deserializes the specified memory handle and converts it to a watcher
	 * user object.
	 *
	 * @param handle
	 *            The memory handle to the watcher user object.
	 * @return The watcher user object stored at the specified memory handle.
	 * @throws Exception
	 *             Could not read from disk.
	 */
	public static WatcherUser getWatcherFromDisk(MemoryHandle handle)
		throws Exception
	{
		MemoryManager mm = DiskBintree.getMemoryManager();
		return deserializeWatcher(mm.getRecord(handle));
	}


	/**
	 * Creates a byte array representation of the specified watcher user object.
	 *
	 * @param user
	 *            The watcher user.
	 * @return The byte array representation of the watcher user.
	 */
	public static byte[] serialize(WatcherUser user)
	{
		String name = user.getName();
		ByteBuffer bf = ByteBuffer.allocate(16 + (2 * name.length()));
		bf.putDouble(user.getLongitude() - 180.0);
		bf.putDouble(user.getLatitude() - 90.0);
		for (int i = 0; i < name.length(); i++)
		{
			bf.putChar(name.charAt(i));
		}
		return bf.array();
	}


	/**
	 * Converts the byte representation of a watcher user to a watcher user
	 * object.
	 *
	 * @param bytes
	 *            The byte array representation of a watcher user.
	 * @return The watcher user object
	 */
	public static WatcherUser deserializeWatcher(byte[] bytes)
	{
		ByteBuffer bf = ByteBuffer.wrap(bytes);
		double longi = bf.getDouble();
		double latti = bf.getDouble();
		StringBuilder sb = new StringBuilder();
		for (int i = 16; i < bytes.length; i += 2)
		{
			sb.append(bf.getChar());
		}
		WatcherUser user = new WatcherUser(sb.toString(), longi, latti);
		return user;
	}
}

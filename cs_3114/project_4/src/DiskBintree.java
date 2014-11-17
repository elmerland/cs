// On my honor:
//
// - I have not used source code obtained from another student,
// or any other unauthorized source, either modified or
// unmodified.
//
// - All source code and documentation used in my program is
// either my original work, or was derived by me from the
// source code published in the textbook for this course.
//
// - I have not discussed coding details about this project with
// anyone other than my partner (in the case of a joint
// submission), instructor, ACM/UPE tutors or the TAs assigned
// to this course. I understand that I may discuss the concepts
// of this program with other students, and that another student
// may help me debug my program so long as neither of us writes
// anything during the discussion or modifies any computer file
// during the discussion. I have violated neither the spirit nor
// letter of this restriction.

import java.io.File;

/**
 * Project 4 - CS 3114 Professor: Cliff Shaffer
 *
 * @author Elmer Landaverde
 * @version Nov 16, 2013
 */
public class DiskBintree
{

	/**
	 * The number of buffers that the buffer pool will have.
	 */
	private static int				numBuffers;

	/**
	 * The size each one of the buffers will have.
	 */
	private static int				bufferSize;

	/**
	 * The name of the input file.
	 */
	private static String			fileName;

	/**
	 * The index for the input file in the arguments to the main method.
	 */
	private static int				inputFileIndex	= 0;

	/**
	 * The index for the number of buffers in the arguments to the main method.
	 */
	private static int				numBuffersIndex	= 1;

	/**
	 * The index for the size of buffers in the arguments to the main method.
	 */
	private static int				bufferSizeIndex	= 2;

	/**
	 * The name of the output file.
	 */
	private static String			outputFile		= "p4bin.dat";

	/**
	 * The watcher manager object.
	 */
	private static WatcherManager	watcherManager;

	/**
	 * The memory manager for the bin tree.
	 */
	private static MemoryManager	memoryManager;

	/**
	 * The buffer pool manager for the memory manager.
	 */
	private static BufferPoolADT	bufferPool;

	/**
	 * Boolean used for testing purposes.
	 */
	public static boolean testing = false;


	/**
	 * Main method for the DiskBinTree program. This program takes in a file
	 * with a list of inputs. It digests every input and builds a bin tree out
	 * of it. The bin tree is stored on disk in the file "p4bin.dat", at the end
	 * of the program this file will store the final state of the bin tree.
	 *
	 * @param args
	 *            The arguments for the program: [file-name] [number-of-buffers]
	 *            [size-of-buffers]
	 * @throws Exception
	 *             Could not execute correctly.
	 */
	public static void main(String args[])
		throws Exception
	{
		// Digest arguments.
		if (!processArgs(args))
		{
			System.out.println("Error: Input is invalid.");
			return;
		}
		// Initialize buffer pool manager.
		bufferPool = new BufferPool(outputFile, numBuffers, bufferSize);

		// Inititalize memory manager.
		memoryManager = new MemoryManager();

		// Initialize the watcher manager.
		watcherManager = new WatcherManager();

		// Process input file.
		StreamManager.digestInputFile(fileName);

		if (!testing) {
			bufferPool.closeBufferPool();
		}
	}


	/**
	 * Gets the watcher manager object.
	 *
	 * @return A watcher manager object.
	 */
	public static WatcherManager getWatcherManager()
	{
		return watcherManager;
	}


	/**
	 * Gets the memory manager for the bin tree.
	 *
	 * @return The memory manager.
	 */
	public static MemoryManager getMemoryManager()
	{
		return memoryManager;
	}


	/**
	 * Gets the buffer pool manager.
	 *
	 * @return The buffer pool.
	 */
	public static BufferPoolADT getBufferPool()
	{
		return bufferPool;
	}


	/**
	 * Gets the number of buffers that the buffer pool should have.
	 *
	 * @return The number of buffers.
	 */
	public static int getNumberBuffers()
	{
		return numBuffers;
	}


	/**
	 * Validate and process the parameters to the main method. Valid arguments
	 * have a length of 3. The first index is the input file name, the second
	 * index is the number of buffers and the third index is the size of each
	 * buffer.
	 *
	 * @param args
	 *            The arguments to the main method.
	 * @return True if the arguments are valid and were process correctly.
	 */
	private static boolean processArgs(String args[])
	{
		// Check null.
		if (args == null)
		{
			System.out.println("Error: Input is null.");
			return false;
		}
		else if (args.length != 3) // Check length.
		{
			System.out.println("Error: Input is of invalid length.");
			return false;
		}
		// Check for null elements inside of arguments array.
		for (int i = 0; i < args.length; i++)
		{
			if (args[i] == null)
			{
				System.out.println("Error: Argument " + i + " is null.");
				return false;
			}
		}
		// Parse strings to integers.
		try
		{
			numBuffers = Integer.parseInt(args[numBuffersIndex]);
			bufferSize = Integer.parseInt(args[bufferSizeIndex]);
		}
		catch (Exception e)
		{
			System.out.println("Error: Could not parse arguments to integers.");
			return false;
		}
		// Check file name.
		File f = new File(args[inputFileIndex]);
		if (!f.exists() || !f.isFile())
		{
			System.out.println("Error: File name is invalid.");
			return false;
		}
		else
		{
			fileName = args[inputFileIndex];
		}
		// Check for output file.
		f = new File(outputFile);
		if (f.exists() && f.isFile())
		{
			f.delete();
		}
		// All arguments were processed successfully.
		return true;
	}
}

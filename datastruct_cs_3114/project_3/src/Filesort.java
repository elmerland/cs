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
 * Project 3.
 *
 * @author Elmer
 * @version Oct 29, 2013
 */
public class Filesort
{

	private static int		bufferCount;

	private static String	fileName;

	/**
	 * Testing variable that determines if the main method executed
	 * successfully.
	 */
	public static boolean	success	= false;


	/**
	 * Place a description of your method here.
	 *
	 * @param args
	 *            Arguments.
	 * @throws Exception
	 *             Problem with the buffer.
	 */
	public static void main(String args[])
		throws Exception
	{
		// Check input.
		if (!processArgs(args))
		{
			System.out.println("Error: Arguments are invalid.");
			success = false;
			return;
		}
		StringBuilder sb = new StringBuilder();

		sb.append("Sorting " + fileName + "\n");

		// Create buffer pool.
		BufferPoolADT bp = new BufferPool(fileName, bufferCount);
		// Create array.
		ShortArray sa = new ShortArray(bp);
		long size = sa.size();

		// Sort array.
		long startTime = System.currentTimeMillis();
		HeapSort heap = new HeapSort(sa, size);
		heap.sort();
		long endTime = System.currentTimeMillis();
		long time = endTime - startTime;

		bp.flushBuffers();

		sb.append(Util.getFormatedMetrics((BufferPool)bp) + "\n");
		sb.append("The time to execute the heapsort " + time + ".0\n");
		sb.append(sa.getBlocks() + "\n");
		sb.append(sa.size() + " records processed");

		bp.closeBufferPool();

		System.out.println(sb.toString());
		System.out.println("DFTBA!");
		success = true;
	}


	private static boolean processArgs(String args[])
	{
		// Check argument input.
		if (args == null)
		{
			System.out.println("Error: Arguments are null.");
			return false;
		}
		else if (args.length == 0)
		{
			System.out.println("Error: Arguments are empty.");
			return false;
		}
		else if (args.length != 2)
		{
			System.out.println("Error: Arguments has invalid size.");
			return false;
		}

		// Check file name.
		fileName = args[0];
		if (fileName == null)
		{
			System.out.println("Error: File name is null.");
			return false;
		}
		File f = new File(fileName);
		if (!f.exists())
		{
			System.out.println("Error: The specified file does not exist: " + fileName);
			return false;
		}
		else if (!f.isFile())
		{
			System.out.println("Error: The specified file is a directory.");
			return false;
		}

		// Check number of buffers.
		String buffers = args[1];
		if (buffers == null)
		{
			System.out.println("Error: The buffer is null.");
			return false;
		}
		try
		{
			bufferCount = Integer.parseInt(buffers);
		}
		catch (NumberFormatException e)
		{
			System.out.println("Error: The number of buffers could not be determined.");
			return false;
		}
		if (bufferCount <= 0)
		{
			System.out.println("Error: The number of buffers is zero.");
			return false;
		}

		// Everything checks out.
		return true;
	}

}

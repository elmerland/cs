import java.io.IOException;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileInputStream;
import java.nio.channels.FileChannel;

/**
 * Class of miscellaneous methods for use accross all classes.
 *
 * @author Elmer Landaverde
 * @version Nov 5, 2013
 */
public class Util
{

	/**
	 * Unsorted binary file used for testing.
	 */
	public static final String	bFile	= "unsorted_10_blocks_b.dat";

	/**
	 * Unsorted text file used for testing.
	 */
	public static final String	aFile	= "unsorted_10_blocks_a.dat";


	/**
	 * Gets a string representation of the buffer pool metrics.
	 *
	 * @param bp
	 *            The buffer pool.
	 * @return The string representation of the buffer pool metrics.
	 */
	public static String getFormatedMetrics(BufferPool bp)
	{
		int[] metrics = bp.getMetrics();
		StringBuilder sb = new StringBuilder();
		sb.append("Number of cache hits:\t" + metrics[1] + "\n");
		sb.append("Number of cache misses:\t" + metrics[0] + "\n");
		sb.append("Number of disk reads:\t" + metrics[2] + "\n");
		sb.append("Number of disk writes:\t" + metrics[3]);
		return sb.toString();
	}


	/**
	 * Copies a file to a different location.
	 *
	 * @param sourceFile
	 *            The source file.
	 * @param destFile
	 *            The destination file.
	 * @throws IOException
	 *             Exception when file cannot be read or written to.
	 */
	public static void copyFile(File sourceFile, File destFile)
		throws IOException
	{
		FileChannel source = null;
		FileChannel destination = null;

		FileInputStream input = new FileInputStream(sourceFile);
		FileOutputStream output = new FileOutputStream(destFile);

		source = input.getChannel();
		destination = output.getChannel();
		destination.transferFrom(source, 0, source.size());
		input.close();
		output.close();
		source.close();
		destination.close();
	}

}

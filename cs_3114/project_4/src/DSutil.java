
/**
 * Class of miscellaneous methods for use across all classes.
 *
 * @author Elmer Landaverde
 * @version Nov 5, 2013
 */
public class DSutil
{


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
		sb.append("Number of cache hits: " + metrics[1] + "\n");
		sb.append("Number of cache misses: " + metrics[0] + "\n");
		sb.append("Number of disk reads: " + metrics[2] + "\n");
		sb.append("Number of disk writes: " + metrics[3]);
		return sb.toString();
	}


	/**
	 * Increase the discriminator and returns the discriminator value for one
	 * depth level below the current discriminator. Place a description of your
	 * method here.
	 *
	 * @param currDiscriminator
	 *            The current discriminator;
	 * @return The new discriminator for one depth level below the current
	 *         discriminator.
	 */
	public static int incDisc(int currDiscriminator)
	{
		return (currDiscriminator + 1) % 2;
	}

}

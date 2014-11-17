/**
 * Source code example for "A Practical Introduction to Data Structures and
 * Algorithm Analysis, 3rd Edition (Java)" by Clifford A. Shaffer Copyright
 * 2008-2011 by Clifford A. Shaffer
 */

/**
 * Extend the standard Dictionary to 2D spatial point data
 *
 * @param <E>
 *            The generic type.
 */
public interface SpatialDictionary<E>
	extends Dictionary<double[], E>
{

	/**
	 * Visit all records within the radius distance of point k
	 *
	 * @param k
	 *            The origin point.
	 * @param rad
	 *            The distance from origin.
	 */
	public void regionSearch(double[] k, double rad);
}

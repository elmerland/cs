import java.text.SimpleDateFormat;
import java.util.Calendar;

/**
 * Source code example for "A Practical Introduction to Data Structures and
 * Algorithm Analysis, 3rd Edition (Java)" by Clifford A. Shaffer Copyright
 * 2008-2011 by Clifford A. Shaffer
 */

/**
 * A bunch of utility functions
 *
 * @param <E>
 *            The generic type
 * @author CS 3114
 * @version Sep 10, 2013
 */
class DSutil<E>
{

    /**
     * Time constant that is equivalent to six hour time in milliseconds.
     */
    public static long sixHourTime = 21600000L;


    /**
     * Helper method that converts time in milliseconds to time in hours.
     *
     * @param millis
     *            The time in milliseconds.
     * @return An hours representation of the time in milliseconds.
     */
    public static String printHours(long millis)
    {
        long hours = millis / 3600000;
        millis = millis % 3600000;
        long minutes = millis / 60000;
        millis = millis % 60000;
        long seconds = millis / 1000;
        millis = millis % 1000;
        return hours + ":" + minutes + ":" + seconds + "." + millis;
    }


    /**
     * Helper method that converts time in milliseconds to date.
     *
     * @param millis
     *            The time in milliseconds.
     * @return The date of the time in milliseconds.
     */
    public static String printDate(long millis)
    {
        Calendar c = Calendar.getInstance();
        c.setTimeInMillis(millis);
        SimpleDateFormat df = new SimpleDateFormat("yyyy.MM.dd HH:mm:ss Z");
        return df.format(c.getTime());
    }


    /**
     * Swap two Objects in an array
     *
     * @param <E>
     *            The generic type
     * @param a
     *            The array
     * @param p1
     *            Index of one Object in A
     * @param p2
     *            Index of another Object A
     */
    public static <E> void swap(E[] a, int p1, int p2)
    {
        E temp = a[p1];
        a[p1] = a[p2];
        a[p2] = temp;
    }


    /**
     * Swaps the indexes of two Indexable objects.
     *
     * @param a
     *            The first Indexable object.
     * @param b
     *            The seconds Indexable object.
     */
    public static void swapIndexables(Indexable a, Indexable b)
    {
        int indexA = a.getIndex();
        a.setIndex(b.getIndex());
        b.setIndex(indexA);
    }

}

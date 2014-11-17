import java.util.ArrayList;
import realtimeweb.earthquakeservice.domain.Report;
import realtimeweb.earthquakeservice.domain.Earthquake;

/**
 * This class is in charge of managing all the earthquake information. It
 * handles adding, deleting and querying earthquakes.
 *
 * @author Elmer Landaverde, Harjas Singh
 * @version Sep 3, 2013
 */
public class EarthquakeManager
{

    /**
     * This class is in charge of keeping all the relevant information to be
     * stored inside of the max heap.
     *
     * @author Elmer Landaverde, Harjas Singh
     * @version Sep 10, 2013
     */
    public static class HeapLog
        implements ComparableIndexable<HeapLog>
    {

        private double   magnitude;

        private QueueLog queueLog;


        /**
         * Create a new HeapLog object.
         *
         * @param earthquakeMagnitude
         *            The magnitude of the earthquake
         * @param theQueueLog
         *            The QueueLog object corresponding to this HeapLog object.
         */
        public HeapLog(double earthquakeMagnitude, QueueLog theQueueLog)
        {
            magnitude = earthquakeMagnitude;
            queueLog = theQueueLog;
        }


        @Override
        public int compareTo(HeapLog o)
        {
            return (int)(magnitude - o.magnitude);
        }

        @Override
        public Indexable getIndexable()
        {
            return getQueueLog();
        }

        /**
         * Gets the QueueLog object that corresponds to this HeapLog
         *
         * @return A QueueLog object
         */
        public QueueLog getQueueLog() {
            return queueLog;
        }

    }


    /**
     * The QueueLog class is meant to store all the relevant information to be
     * stored inside the linked queue.
     *
     * @author Elmer Landaverde, Harjas Singh
     * @version Sep 5, 2013
     */
    public static class QueueLog implements Indexable
    {

        private Earthquake earthquake;
        private int        heapLogIndex;


        /**
         * Creates a new QueueLog object.
         *
         * @param eq
         *            The earthquake object.
         * @param indexOfHeapLog
         *            The index that represents the current position of the
         *            HeapLog object corresponding to this QueueLog in the Max
         *            Heap
         */
        public QueueLog(Earthquake eq, int indexOfHeapLog)
        {
            earthquake = eq;
            heapLogIndex = indexOfHeapLog;
        }


        /**
         * Gets the Earthquake object belonging to this QueueLog.
         *
         * @return The Earthquake object.
         */
        public Earthquake getEarthquake()
        {
            return earthquake;
        }


        /**
         * Gets the magnitude of the earthquake.
         *
         * @return The magnitude of the earthquake.
         */
        public double getEarthquakeMaginuted()
        {
            return earthquake.getMagnitude();
        }


        /**
         * Gets the time at which the earthquake happened.
         *
         * @return The time of the earthquake.
         */
        public long getEarthquakeTime()
        {
            return earthquake.getTime();
        }


        /**
         * Gets the index of of the HeapLog that corresponds to this QueueLog
         *
         * @return The index of the HeapLog object inside of the max heap.
         */
        @Override
        public int getIndex()
        {
            return heapLogIndex;
        }


        /**
         * Sets the index of the HeapLog in the max heap.
         *
         * @param newIndex
         *            The new index.
         */
        @Override
        public void setIndex(int newIndex)
        {
            heapLogIndex = newIndex;
        }

    }

    /**
     * The linked queue that will keep track of all the earthquake logs.
     */
    private LQueue<QueueLog> queue;

    /**
     * The max heap that will keep track of the magnitude of all the earthquake
     * logs.
     */
    private MaxHeap<HeapLog> heap;

    /**
     * Array list that holds all the elements in the max heap.
     */
    private HeapLog[]        heapList                = new HeapLog[1000];

    /**
     * The time of the earthquake that what last added to the linked queue and
     * the max heap. This time is used to determine if there are any new
     * earthquakes ins the report.
     */
    private long             lastEarthquakeAddedTime = 0L;

    /**
     * Create a new Earthquakes object.
     */
    public EarthquakeManager()
    {
        initDataStructures();
    }


    /**
     * Takes and earthquake report and processes the report.
     *
     * @param r
     *            The earthquake report.
     */
    public void addEarthquakeReport(Report r)
    {
        // Add new earthquakes to the queue and heap.
        addNewEarthquakes(r);

        // Delete all earthquakes older than 6 hours.
        deleteOldEarthquakes(r);

    }


    /**
     * Makes a query that prints the earthquake with the largest magnitude.
     */
    public void queryEarthquakes()
    {
        // Test if the heap is empty.
        if (heap.heapsize() == 0)
        {
            StreamOutput.printNoQuery();
        }
        else
        {
            // Get the largest earthquake and output the message.
            HeapLog hl = heapList[0];
            Earthquake eq = hl.getQueueLog().getEarthquake();
            StreamOutput.printMagnitudeQuery(eq);
        }
    }


    /**
     * Take a Report object and adds all the new earthquakes inside the report.
     *
     * @param report
     *            The report that contains all the earthquake objects.
     */
    private void addNewEarthquakes(Report report)
    {
        // Get the earthquake object array from the report.
        ArrayList<Earthquake> earthquakeList = report.getEarthquakes();
        // Add all earthquake object that are older than the time of the last
        // report.
        for (Earthquake quake : earthquakeList)
        {
            // System.out.println("EQ:\t" + printDate(eq.getTime()));
            if (quake.getTime() > lastEarthquakeAddedTime)
            {
                // Update time of last earthquake
                lastEarthquakeAddedTime = quake.getTime();
                // Add earthquake to queue.
                QueueLog newEarthquakeLog =
                    new QueueLog(quake, heap.heapsize());
                queue.enqueue(newEarthquakeLog);
            }
        }
    }


    /**
     * Makes sure that every earthquake element in the list queue is not older
     * than six hours.
     *
     * @param currentTime
     *            The current time in milliseconds.
     */
    private void deleteOldEarthquakes(Report r)
    {
        // The minimum time an earthquake can have.
        long cutOffTime = r.getGeneratedTime() - DSutil.sixHourTime;
        // Check each earthquake and delete the old ones.
        while (heap.heapsize() > 0 && queue.length() > 0
            && queue.frontValue().getEarthquakeTime() < cutOffTime)
        {
            queue.dequeue();
        }
    }


    /**
     * Initializes the linked queue and max heap data structures.
     */
    private void initDataStructures()
    {
        // Initialize the linked queue and override the enqueue and dequeue
        // methods.
        queue = new LQueue<QueueLog>() {
            /**
             * Overrides original dequeue method. This methods removes the first
             * element on the queue list and returns the removed element. It
             * also modifies the max heap to reflect the changes made on the
             * linked queue.
             *
             * @return The QueueLog object that was removed form the top of the
             *         linked queue.
             */
            @Override
            public QueueLog dequeue()
            {
                // Remove the first element in the queue.
                QueueLog ql = super.dequeue();
                // Get the corresponding index of the HeapLog object linked to
                // this QueueLog object.
                int hlIndex = ql.getIndex();
                heap.remove(hlIndex);
                return ql;
            }


            /**
             * Overrides original enqueue method. This method adds a QueueLog
             * object to the back of the queue list. It also modifies the max
             * heap to reflect the changes made to the linked queue.
             */
            @Override
            public void enqueue(QueueLog log)
            {
                // Instantiate the HeapLog object that will correspond to the
                // QueueLog object.
                HeapLog hl = new HeapLog(log.getEarthquakeMaginuted(), log);
                heap.insert(hl);
                // Add the QueueLog object to the queue.
                super.enqueue(log);
                // Pint output
                StreamOutput.printEarthquakeAdded(log.getEarthquake());
                // Report earthquake to watcher manager.
                EqSimple.getWatcherManager().reportEarthquake(
                    log.getEarthquake());
            }
        };
        // Initialize the max heap.
        heap = new MaxHeap<HeapLog>(heapList, 0, 1000);
        heap.buildheap();
    }
}

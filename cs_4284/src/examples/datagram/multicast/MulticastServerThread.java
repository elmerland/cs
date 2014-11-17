

package examples.datagram.multicast;

import examples.datagram.QuoteServerThread;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.util.Date;

public class MulticastServerThread extends QuoteServerThread {

  private long FIVE_SECONDS = 2000;
  static final String BCAST_ADDR = "224.0.0.1";

  public MulticastServerThread() throws IOException {
    super("MulticastServerThread");
  }

  public void run() {
    while (moreQuotes) {
      try {
        byte[] buf = new byte[256];

        // Construct quote
        String dString;
        if (in == null) {
          dString = new Date().toString();
        } else {
          dString = getNextQuote();
        }
        buf = dString.getBytes();

        // Send it
        InetAddress group = InetAddress.getByName(BCAST_ADDR);
        DatagramPacket packet = new DatagramPacket(buf, buf.length, group, 4446);
        socket.send(packet);

        // Sleep for a while
        try {
          sleep((long)(Math.random() * FIVE_SECONDS));
        } catch (InterruptedException e) {}
      } catch (IOException e) {
        e.printStackTrace();
        moreQuotes = false;
      }
    }
    socket.close();
  }
}

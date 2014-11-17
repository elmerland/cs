package examples.datagram.multicast;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;

public class MulticastClient {

  static final String BCAST_ADDR = "224.0.0.1";

  public static void main (String args[]) throws IOException {
    MulticastSocket socket = new MulticastSocket(4446);
    InetAddress address = InetAddress.getByName(BCAST_ADDR);
    socket.joinGroup(address);

    DatagramPacket packet;

    // Get a few quotes
    for (int i = 0; i < 5; i++) {
      byte[] buf = new byte[256];
      packet = new DatagramPacket(buf, buf.length);
      socket.receive(packet);

      String received = new String(packet.getData(), 0, packet.getLength());
      System.out.println("Quote of the moment: " + received);
    }

    socket.leaveGroup(address);
    socket.close();
  }
}

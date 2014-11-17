package Communicator;

import java.io.IOException;
import java.io.PrintStream;
import java.net.*;
import java.util.Arrays;
import java.util.Collections;
import java.util.Enumeration;
import java.util.Vector;

public class MulticastListener implements Runnable {

  static final int MAX_QUEUE = 10;
  static final int MAX_MSG_SIZE = 1024 * 50; // 50 kb
  static final int SO_TIMEOUT = 1000; // Receive for a maximum of 1 seconds
  private Vector<String> messages = new Vector<String>();

  static final String BCAST_ADDR = "224.0.0.1";
  static final int PORT = 1691;

  static private PrintStream out = System.out;

  @Override
  public void run() {
    out.println("Starting Listener thread");
    try {
      MulticastSocket socket = new MulticastSocket(PORT);

      InetAddress address = InetAddress.getByName(BCAST_ADDR);
      socket.joinGroup(address);

      while (true) {
        try {
          if (putMessage(socket)) {
            break;
          }
        } catch (InterruptedException e) {
          System.err.println(e.getMessage());
        }
      }

      socket.leaveGroup(address);
      socket.close();
    } catch (IOException e) {
      System.err.println(e.getMessage());
      e.printStackTrace();
    }
  }

  private synchronized boolean putMessage(MulticastSocket socket)
      throws InterruptedException {
    // Wait until message queue is not empty
    while (messages.size() == MAX_QUEUE) {
      out.println("Put: Before wait");
      wait();
      out.println("Put: After wait");
    }

    // Get new message
    byte[] buf = new byte[MAX_MSG_SIZE];
    DatagramPacket packet = new DatagramPacket(buf, buf.length);
//      socket.receive(packet);
    receivePacket(socket, packet);

    String received = new String(packet.getData(), 0, packet.getLength());
    if (received.equals("end")) {
      return true;
    }
    messages.addElement(received);
    out.println("Put message: list size " + messages.size());
    notify(); // Notify other threads
    return false;
  }

  private synchronized void receivePacket(MulticastSocket socket,
                                             DatagramPacket packet)
    throws InterruptedException {

    try {
      socket.setSoTimeout(SO_TIMEOUT);

      while (true) {
        try {
          socket.receive(packet);
          break;
        } catch (SocketTimeoutException e) {
          System.err.println(e.getMessage());
          e.printStackTrace();
        } catch (IOException e) {
          System.err.println(e.getMessage());
          e.printStackTrace();
          break;
        }
        notify();
        out.println("Rec: Before wait");
        wait(SO_TIMEOUT);
        out.println("Rec: After wait");
      }


    } catch (SocketException e) {
      System.err.println(e.getMessage());
      e.printStackTrace();
    }
  }

  public synchronized String getMessage() throws InterruptedException {
    while (messages.size() == 0) {
      out.println("Get: Before wait");
      wait();
      out.println("Get: After wait");
    }
    out.println("Get message: list size " + messages.size());
    String msg = messages.firstElement();
    messages.removeElementAt(0);
    notify();
    return msg;
  }
}

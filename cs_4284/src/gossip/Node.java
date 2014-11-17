package gossip;

import javax.management.Notification;
import javax.management.NotificationListener;
import java.io.*;
import java.net.*;
import java.util.ArrayList;
import java.util.Random;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;

public class Node implements NotificationListener {

  private MemberManager memberManager;
  private DatagramSocket    gossipServer;
  private MulticastSocket   multicastServer;
  private String            myAddress;
  private AtomicBoolean     inGroup;
  private ExecutorService   executor;
  private Logger            log;
  /**
   * Setup the client's lists, gossiping parameters, and parse the startup config file.
   *
   * @throws SocketException
   * @throws InterruptedException
   * @throws UnknownHostException
   */
  public Node() throws SocketException, InterruptedException, UnknownHostException {

    Runtime.getRuntime().addShutdownHook(new Thread(new Runnable() {
      public void run() {
        System.out.println("Goodbye my friends...");
      }
    }));

    log = new Logger(this);
    memberManager = new MemberManager(this, log);
    inGroup = new AtomicBoolean(false);

    NetworkInterface networkInterface;
    InetSocketAddress address;
    InetAddress group;

    try {
      group = InetAddress.getByName(Gossip.MULTICAST_ADDRESS);

      networkInterface = NetworkInterface.getByName(Gossip.INTERFACE_NAME);
      address = new InetSocketAddress(group, Gossip.MULTICAST_PORT);

      multicastServer = new MulticastSocket(Gossip.MULTICAST_PORT);
      multicastServer.joinGroup(address, networkInterface);
    } catch (Exception e) {
      e.printStackTrace();
    }

    String myIpAddress = InetAddress.getLocalHost().getHostAddress();
    this.myAddress = myIpAddress + ":" + Gossip.GOSSIP_PORT;

    gossipServer = new DatagramSocket(Gossip.GOSSIP_PORT);

    executor = Executors.newCachedThreadPool();

    Member me = new Member(this.myAddress, 0, this, Gossip.GOSSIP_CLEAN);
    memberManager.addNewMember(me, true);
    log.addEvent("ADD: Add myself to the member list - " + me);
  }

  public MemberManager getMemberManager() { return memberManager; }

  public String getMyAddress() { return myAddress; }

  synchronized public void markInGroup() {
    if (!inGroup.get()) {
      inGroup.set(true);
    }
  }

  synchronized public void markOutGroup () {
    //TODO: Should we mark out of group when thread is alone?
    if (inGroup.get()) {
      inGroup.set(false);
    }
  }

  /**
   * The class handles gossiping the membership list.
   * This information is important to maintaining a common
   * state among all the nodes, and is important for detecting
   * failures.
   */
  private class MembershipGossiper implements Runnable {

    private AtomicBoolean keepRunning;

    public MembershipGossiper() {
      this.keepRunning = new AtomicBoolean(true);
    }

    @Override
    public void run() {
      while (this.keepRunning.get()) {
        try {
          TimeUnit.MILLISECONDS.sleep(Gossip.GOSSIP_PING);
          memberManager.sendMembershipList();
        } catch (InterruptedException e) {
          // TODO: handle exception
          // This membership thread was interrupted externally, shutdown
          e.printStackTrace();
          keepRunning.set(false);
        }
      }

      this.keepRunning = null;
    }

  }

  /**
   * This class listens on the multicast socket for new nodes.
   * Updated the member list as necessary.
   */
  private class AsynchronousMulticastReceiver implements Runnable {

    private AtomicBoolean keepRunning;
    private Node          myNode;

    public AsynchronousMulticastReceiver(Node node) {
      keepRunning = new AtomicBoolean(true);
      myNode = node;

    }

    @SuppressWarnings("unchecked")
    @Override
    public void run() {
      while (keepRunning.get()) {
        // --------------------- RECEIVE ON MULTICAST ---------------------
        DatagramPacket recv = null;
        String newNodeIp = "";
        try {
          do {
            // get their responses!
            byte[] buf = new byte[Gossip.PACKET_SIZE];
            recv = new DatagramPacket(buf, buf.length);
            log.addEvent("WAIT: Waiting for multicast message");

            multicastServer.receive(recv);

            newNodeIp = Gossip.getAddress(recv);
            log.addEvent("RECV: Received multicast message - " + newNodeIp);

          } while (((newNodeIp).equals(myAddress)));
        } catch (IOException e) {
          log.addEvent(LogType.ERROR, "Error receiving datagram packet: " + e.getMessage());
          e.printStackTrace();
        }

        Member newNode = new Member(newNodeIp, 0, myNode, Gossip.GOSSIP_CLEAN);

        markInGroup(); // Change to inGroup state
        memberManager.addNewMember(newNode, false); // Add new member to list
      }

      this.keepRunning = null;
    }
  }

  /**
   * This class handles the passive cycle, where this client
   * has received an incoming message.  For now, this message
   * is always the membership list, but if you choose to gossip
   * additional information, you will need some logic to determine
   * the incoming message.
   */
  class AsynchronousGossipReceiver implements Runnable {

    private AtomicBoolean keepRunning;
    private Node          myNode;

    public AsynchronousGossipReceiver() {
      keepRunning = new AtomicBoolean(true);
    }

    @SuppressWarnings("unchecked")
    @Override
    public void run() {
      while (keepRunning.get()) {
        try {
          //XXX: be mindful of this array size for later
          byte[] buf = new byte[Gossip.PACKET_SIZE];
          DatagramPacket p = new DatagramPacket(buf, buf.length);
          gossipServer.receive(p);

          // extract the member arraylist out of the packet
          // TODO: maybe abstract this out to pass just the bytes needed
          ByteArrayInputStream bais = new ByteArrayInputStream(p.getData());
          ObjectInputStream ois = new ObjectInputStream(bais);

          Object readObject = ois.readObject();
          if (readObject instanceof ArrayList<?>) {
//						inGroup = true;
            ArrayList<Member> list = (ArrayList<Member>) readObject;

            StringBuilder str = new StringBuilder();
            str.append("RECV: Receiving member list\n From: ")
               .append(p.getAddress()).append("\nList:\n");
            for (Member m : list) {
              str.append(m).append("\n");
            }
            log.addEvent(str.toString());

            markInGroup();
            // Merge our list with the one we just received
            memberManager.mergeLists(list);
          }

        } catch (IOException e) {
          log.addEvent(LogType.ERROR, "Gossip receiver IO error. Stopping thread...");
          e.printStackTrace();
          keepRunning.set(false);
        } catch (ClassNotFoundException e) {
          // TODO Auto-generated catch block
          e.printStackTrace();
        }
      }
    }
  }

  /**
   * Starts the client.  Specifically, start the various cycles for this protocol.
   * Start the gossip thread and start the receiver thread.
   *
   * @throws InterruptedException
   */
  public void start_listeners(Node node) throws InterruptedException {

    //  The receiver thread is a passive player that handles
    //  merging incoming membership lists from other neighbors.
    executor.execute(new AsynchronousGossipReceiver());

    // AsynchronousMulticastReceiver()
    executor.execute(new AsynchronousMulticastReceiver(node));

    while (!inGroup.get()) {
      node.send_multicast();
    }

    log.addEvent("IN GROUP!");
    executor.execute(new MembershipGossiper());

    // keep the main thread around
    while (true) {
      TimeUnit.SECONDS.sleep(10);
    }
  }

  private void send_multicast() {
    // join a Multicast group and send the group salutations
    try {
      String msg = "Hello";
      InetAddress group = InetAddress.getByName(Gossip.MULTICAST_ADDRESS);
      DatagramPacket hi = new DatagramPacket(msg.getBytes(), msg.length(), group, Gossip.MULTICAST_PORT);
      multicastServer.send(hi);
      Thread.sleep(3000);
    } catch (Exception e) {
      e.printStackTrace();
    }
  }

  /**
   * All timers associated with a member will trigger this method when it goes
   * off.  The timer will go off if we have not heard from this member in
   * <code> GOSSIP_CLEAN </code> time.
   */
  @Override
  public void handleNotification(Notification notification, Object handback) {

    Member deadMember = (Member) notification.getUserData();

    log.addEvent("DEAD: Dead member detected - " + deadMember);

    memberManager.killMember(deadMember);
  }
}

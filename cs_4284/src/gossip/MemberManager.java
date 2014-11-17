package gossip;

import org.json.simple.JSONArray;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.ArrayList;
import java.util.Random;

public class MemberManager {

  private ArrayList<Member> memberList;
  private ArrayList<Member> deadList;
  private Member me = null;
  private Node node;
  private Logger log;
  private Random            random;

  public MemberManager(Node n, Logger l) {
    node = n;
    log = l;
    random = new Random();
    memberList = new ArrayList<Member>();
    deadList = new ArrayList<Member>();
  }

  /**
   * Merge remote list (received from peer), and our local member list.
   * Simply, we must update the heartbeats that the remote list has with
   * our list.  Also, some additional logic is needed to make sure we have
   * not timed out a member and then immediately received a list with that
   * member.
   *
   * @param remoteList List from remote node
   */
  synchronized public void mergeLists(ArrayList<Member> remoteList) {
    // Iterate through every member of the remote list
    for (Member remoteMember : remoteList) {
      if (memberList.contains(remoteMember)) {
        // The remote member matches a local member. Synchronize heartbeats.
        log.addEvent("MERGE: Synchronize remote member");
        synchronizeHeartbeats(remoteMember);
      } else {
        // the local list does not contain the remote member

        // the remote member is either brand new, or a previously declared dead member
        // if its dead, check the heartbeat because it may have come back from the dead

        if (deadList.contains(remoteMember)) {
          // Remote member is in the deadlist. Determine if it has come back to life.
          Member localDeadMember = deadList.get(deadList.indexOf(remoteMember));
          if (remoteMember.getHeartbeat() > localDeadMember.getHeartbeat()) {
            // it's baa-aack
            log.addEvent("MERGE: Revive member");
            reviveMember(localDeadMember, remoteMember);
          } // else ignore
        } else {
          // brand spanking new member - welcome
          log.addEvent("MERGE: Add remote member - " + remoteMember);
          insertMember(remoteMember, false);
        }
      }
    }
  }

  /**
   * Add new member to list.
   * @param newMember New member object.
   */
  synchronized public void addNewMember(Member newMember, boolean addAsMe) {
    if (memberList.contains(newMember)) {
      log.addEvent(LogType.WARNING, "Could not add duplicate member - " + newMember);
    } else {
      insertMember(newMember, addAsMe);
    }
  }

  /**
   * Clone member object and add to list. It does NOT check for duplicated
   * @param newMember New member
   */
  synchronized private void insertMember(Member newMember, boolean addAsMe) {
    Member newLocalMember =
      new Member(newMember.getAddress(), newMember.getHeartbeat(), node, Gossip.GOSSIP_CLEAN);
    memberList.add(newLocalMember);
    if (!addAsMe) {
      newLocalMember.startTimeoutTimer();
    } else {
      me = newLocalMember;
    }
    log.addEvent("ADD: Added new member to list - " + newLocalMember);
  }

  /**
   * Synchronize heartbeats of remote member and matching local member.
   * @param remoteMember Remove member.
   */
  synchronized public void synchronizeHeartbeats(Member remoteMember) {
    Member localMember = memberList.get(memberList.indexOf(remoteMember));
    if (remoteMember.getHeartbeat() > localMember.getHeartbeat()) {
      // update local list with latest heartbeat
      localMember.setHeartbeat(remoteMember.getHeartbeat());
      // and reset the timeout of that member
      localMember.resetTimeoutTimer();
    }
  }

  /**
   * Bring member back from the dead list. Update its heartbeats to match the remote member.
   * @param localDeadMember Local member in dead list.
   * @param remoteMember Remote member.
   */
  synchronized public void reviveMember(Member localDeadMember, Member remoteMember) {
    deadList.remove(localDeadMember);
    Member newLocalMember =
      new Member(remoteMember.getAddress(), remoteMember.getHeartbeat(), node, Gossip.GOSSIP_CLEAN);
    memberList.add(newLocalMember);
    newLocalMember.startTimeoutTimer();
  }

  @SuppressWarnings("unchecked")
  synchronized public JSONArray getMembersJSON() {
    JSONArray json = new JSONArray();
    for (Member m : memberList) {
      json.add(m.toJSON());
    }
    return json;
  }

  /**
   * Send member list to a random member
   */
  synchronized public void sendMembershipList() {
    // Increase my own heartbeat
	if (me == null) {
		log.addEvent(LogType.ERROR, "SEND: I am null!");
	}
    me.setHeartbeat(me.getHeartbeat() + 1);

    // Send member list to random member
    try {
      Member member = getRandomMember();

      if (member != null) {
        // Convert member list into byte array
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        ObjectOutputStream oos = new ObjectOutputStream(baos);
        oos.writeObject(this.memberList);
        byte[] buf = baos.toByteArray();
        if (buf.length > Gossip.PACKET_SIZE) {
          log.addEvent(LogType.ERROR, "Member list is larger than packet size");
        }

        // Get address info of target member
        String address = member.getAddress();
        String host = address.split(":")[0];
        int port = Integer.parseInt(address.split(":")[1]);

        InetAddress dest = InetAddress.getByName(host);
        log.addEvent("SEND: sending member list to - " + dest);

//					System.out.println("Sending to " + dest);
//					System.out.println("---------------------");
//					for (Member m : memberList) {
//						System.out.println(m);
//					}
//					System.out.println("---------------------");

        //simulate some packet loss ~25%
        int percentToSend = random.nextInt(100);
        if (percentToSend > 25) {
          DatagramSocket socket = new DatagramSocket();
          DatagramPacket datagramPacket = new DatagramPacket(buf, buf.length, dest, port);
          socket.send(datagramPacket);
          socket.close();
        }
      }

    } catch (IOException e1) {
      e1.printStackTrace();
    }
  }

  /**
   * Find a random peer from the local membership list.
   * Ensure that we do not select ourselves, and keep
   * trying 10 times if we do.  Therefore, in the case
   * where this client is the only member in the list,
   * this method will return null
   *
   * @return Member random member if list is greater than 1, null otherwise
   */
  synchronized private Member getRandomMember() {
    Member member = null;

    if (this.memberList.size() > 1) {
      int tries = 10;
      do {
        int randomNeighborIndex = random.nextInt(this.memberList.size() - 1) + 1;
        member = this.memberList.get(randomNeighborIndex);
        if (--tries <= 0) {
          member = null;
          break;
        }
      } while (member.getAddress().equals(me.getAddress()));
    } else {
      log.addEvent("ALONE: I'm alone in this world.");
    }

    return member;
  }

  synchronized public void killMember(Member m) {
    memberList.remove(m);
    deadList.add(m);
    log.addEvent("KILL: killed member - " + m);
  }
}

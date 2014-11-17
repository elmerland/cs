package Multicast;

import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Collections;
import java.util.Enumeration;

/**
 * Created by Rich on 10/26/14.
 */
public class Driver {

    public static void main(String[] args) throws SocketException {

        /*
        MulticastSender sender = new MulticastSender();
        Thread senderThread = new Thread(sender);
        senderThread.start();
        */

        MulticastReciever reciver = new MulticastReciever("en1");
        Thread reciverThread = new Thread(reciver);
        reciverThread.start();
    }
}
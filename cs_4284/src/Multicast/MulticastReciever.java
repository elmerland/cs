package Multicast;

import java.net.*;
import java.util.Collections;
import java.util.Enumeration;

/**
 * Created by Rich on 10/26/14.
 */
public class MulticastReciever implements Runnable {

    private String networkInterfaceName;

    @Override
    public void run()
    {
        // join a Multicast group and send the group salutations
        InetAddress group;

        try {

            group = InetAddress.getByName("228.5.6.7");

            NetworkInterface networkInterface = NetworkInterface.getByName(networkInterfaceName);
            InetSocketAddress address = new InetSocketAddress(group, 6789);

            MulticastSocket s = new MulticastSocket(6789);
            s.joinGroup(address, networkInterface);

            // get their responses!
            byte[] buf = new byte[1000];
            while (true)
            {
                DatagramPacket recv = new DatagramPacket(buf, buf.length);
                s.receive(recv);
                System.out.println("src: " + recv.getAddress().toString() + " contents: " + new String(buf));
            }

            // OK, I'm done talking - leave the group...
            // s.leaveGroup(group);
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
    }

    public MulticastReciever(String networkIterfaceName)
    {
        this.networkInterfaceName = networkIterfaceName;
    }
}

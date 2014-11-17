package Multicast;

import java.net.*;

/**
 * Created by Rich on 10/26/14.
 */
public class MulticastSender implements Runnable {

    @Override
    public void run()
    {
        // join a Multicast group and send the group salutations
        String msg = "Hello";
        InetAddress group;

        try {

            group = InetAddress.getByName("228.5.6.7");

            NetworkInterface networkInterface = NetworkInterface.getByName("en1");
            InetSocketAddress address = new InetSocketAddress(group, 6789);

            MulticastSocket s = new MulticastSocket(6789);
            s.joinGroup(address, networkInterface);
            DatagramPacket hi = new DatagramPacket(msg.getBytes(), msg.length(),
                    group, 6789);

            System.out.println("Sender: starting to send!");
            int x = 0;
            while (x < 10)
            {
                s.send(hi);
                Thread.sleep(3000);
                x++;
                System.out.println("Sender: Sent x = " + x);
            }

            // OK, I'm done talking - leave the group...
            s.leaveGroup(group);
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
    }
}

package examples.net;

import java.net.InetAddress;
import java.net.InterfaceAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Arrays;
import java.util.Collections;
import java.util.Enumeration;

public class Network {
  public static void main(String args[]) throws SocketException {
//    displayNetworkInterfaces();
//    System.out.printf("###\n\n");
//    displayNetworks();
//    System.out.printf("###\n\n");
    displayNetworksEx();
  }

  static void displayNetworkInterfaces() throws SocketException {
    Enumeration<NetworkInterface> nets = NetworkInterface.getNetworkInterfaces();

    for (NetworkInterface netif : Collections.list(nets)) {
      System.out.printf("Display name: %s\n", netif.getDisplayName());
      System.out.printf("Name: %s\n", netif.getName());
      displaySubInterfaces(netif);
      System.out.printf("\n");
    }
  }

  static void displaySubInterfaces(NetworkInterface netif) throws SocketException {
    Enumeration<NetworkInterface> subifs = netif.getSubInterfaces();

    for (NetworkInterface subif : Collections.list(subifs)) {
      System.out.printf("\tSub Interface Display name: %s\n", subif.getDisplayName());
      System.out.printf("\tSub Interface Name: %s\n", subif.getName());
    }
  }

  static void displayNetworks() throws SocketException {
    Enumeration<NetworkInterface> nets = NetworkInterface.getNetworkInterfaces();
    for (NetworkInterface netint : Collections.list(nets)) {
      displayInterfaceInformation(netint);
    }
  }

  static void displayInterfaceInformation(NetworkInterface netint) throws SocketException {
    System.out.printf("Display name: %s\n", netint.getDisplayName());
    System.out.printf("Name: %s\n", netint.getName());
    Enumeration<InetAddress> inetAddresses = netint.getInetAddresses();
    for (InetAddress inetAddress : Collections.list(inetAddresses)) {
      System.out.printf("InetAddress: %s\n", inetAddress);
    }
    System.out.printf("\n");
  }

  static void displayNetworksEx() throws SocketException {
    Enumeration<NetworkInterface> nets = NetworkInterface.getNetworkInterfaces();
    for (NetworkInterface netint : Collections.list(nets))
      displayInterfaceInformationEx(netint);
  }

  static void displayInterfaceInformationEx(NetworkInterface netint) throws SocketException {
    System.out.printf("##\nDisplay name: %s\n", netint.getDisplayName());
    System.out.printf("Name: %s\n", netint.getName());
    Enumeration<InetAddress> inetAddresses = netint.getInetAddresses();

    for (InterfaceAddress address : netint.getInterfaceAddresses()) {
      System.out.printf("-- Address: %s\n", address.getAddress());
      System.out.printf("-- Broadcast address: %s\n", address.getBroadcast());
      System.out.printf("-- Address prefix length: %s\n", address.getNetworkPrefixLength());
      System.out.printf("\n");
    }

    for (InetAddress inetAddress : Collections.list(inetAddresses)) {
      System.out.printf("InetAddress: %s\n", inetAddress);
    }

    System.out.printf("Up? %s\n", netint.isUp());
    System.out.printf("Loopback? %s\n", netint.isLoopback());
    System.out.printf("PointToPoint? %s\n", netint.isPointToPoint());
    System.out.printf("Supports multicast? %s\n", netint.supportsMulticast());
    System.out.printf("Virtual? %s\n", netint.isVirtual());
    System.out.printf("Hardware address: %s\n", Arrays.toString(netint.getHardwareAddress()));
    System.out.printf("MTU: %s\n", netint.getMTU());
    System.out.printf("\n");
  }
}

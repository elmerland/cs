package Communicator;

public class Communicator {
  public static void main(String args[]) {
    MulticastListener multicastListener = new MulticastListener();
    Thread listenerThread = new Thread(multicastListener);
    listenerThread.start();

    ListManager listManager = new ListManager(multicastListener);
    Thread listThread = new Thread(listManager);
    listThread.start();
  }
}

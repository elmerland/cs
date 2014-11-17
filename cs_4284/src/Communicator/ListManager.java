package Communicator;

public class ListManager implements Runnable {
  private MulticastListener multicastListener;

  public ListManager(MulticastListener l) {
    multicastListener = l;
  }

  @Override
  public void run() {
    System.out.println("Starting ListManager thread");
    int count = 1;
    try {
      while (true) {
        String msg = multicastListener.getMessage();
        System.out.println("[" + count + "] Got message: " + msg);
        count++;
      }
    } catch (InterruptedException e) {}
  }
}

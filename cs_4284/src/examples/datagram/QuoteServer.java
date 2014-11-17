package examples.datagram;

import java.io.IOException;

public class QuoteServer {
  public static void main(String args[]) throws IOException {
    new QuoteServerThread().start();
  }
}

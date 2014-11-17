package gossip;

import http.LogServer;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import java.util.ArrayList;
import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;

public class Logger {

  class Event {
    private LogType type;
    private String msg;
    public Event(LogType logType, String message) {
      type = logType;
      msg = message;
    }

    @Override
    public String toString() {
      StringBuilder str = new StringBuilder();
      str.append(type)
         .append("\t")
         .append(msg);
      return str.toString();
    }
  }

  private Node myNode;
  private ArrayList<Event> events;
  private static final boolean DEBUG = true;

  public Logger(Node node) {

    myNode = node;
    events = new ArrayList<Event>();

    System.out.println("Start log server");
    try {
      LogServer server = new LogServer(this);
    } catch (Exception e) {
      e.printStackTrace();
      System.exit(1);
    }
  }

  synchronized public void addEvent(LogType type, String str) {
    str = str.replaceAll("(\r\n|\n)", "<br />");
    synchronized (Logger.this.events) {
      Event e = new Event(type, str);
      if (DEBUG) {
        System.out.println(e);
      }
      events.add(e);
    }
  }

  synchronized public void addEvent(String str) {
    addEvent(LogType.INFO, str);
  }

  public Node getNodeObj() { return myNode; }

  @SuppressWarnings("unchecked")
  synchronized public JSONArray getEventsJSON() {
    JSONArray json = new JSONArray();
    for (Event e : events) {
      JSONObject obj = new JSONObject();
      obj.put("log_type", e.type.name());
      obj.put("message", e.msg);
      json.add(obj);
    }
    events.clear();
    return json;
  }
}

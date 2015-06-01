package cs3744.pr1;

import java.io.File;

import cs3744.pr1.Shape.ShapeType;
import javafx.beans.property.ObjectProperty;
import javafx.beans.property.SimpleObjectProperty;
import javafx.scene.control.Slider;
import javafx.scene.control.TableView;
import javafx.stage.Stage;

/**
 * This class keeps track of all the global properties of the application.
 *
 */
public class PropertiesController {
	
	public static enum ViewState {
  	NEW, OPEN, OPENED, CLOSE, CLOSED, MODIFIED, SAVE, QUIT, RESIZE
  };
  
  public static enum ClipboardState {
  	IDLE, COPY, PASTE, DELETE
  };
	
	public Stage stage = null;
	public File file = null;
	public ObjectProperty<Shape> selection = null; // An observable circle object. Changes trigger repaint.
  public ObjectProperty<ViewState> viewState = null;
  public ObjectProperty<ClipboardState> clipboardState = null;
  public Shape clipboard = null; // The last copied object.
  public double mouseLastX = -1; // The x coordinate of the last reported mouse position;
  public double mouseLastY = -1; // The y coordinate of the last reported mouse position;
  public ShapeType defaultShape = null;
  public TableView<Shape> table = null;
  public Slider zoomSlider = null;
  
  public PR1Model model = null;
  public MenuBarController menubar = null;
  public CanvasController canvas = null;
	
	public PropertiesController(Stage stage) {
		this.stage = stage;
		selection = new SimpleObjectProperty<Shape>();
		viewState = new SimpleObjectProperty<ViewState>(ViewState.CLOSED);
		clipboardState = new SimpleObjectProperty<ClipboardState>(ClipboardState.IDLE);
		defaultShape = ShapeType.CIRCLE;
	}

}

package cs3744.pr1;

import java.net.URL;
import java.util.ResourceBundle;

import javafx.fxml.FXML;
import javafx.scene.canvas.Canvas;
import javafx.scene.control.MenuItem;
import javafx.scene.control.RadioButton;
import javafx.scene.control.Slider;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.ToggleGroup;
import javafx.scene.layout.BorderPane;
import javafx.scene.paint.Color;
import javafx.stage.Stage;

public class PR1Controller {

    @FXML
    private ResourceBundle resources;

    @FXML
    private URL location;

    @FXML
    private BorderPane borderPane;

    @FXML
    private MenuItem fileNew;

    @FXML
    private MenuItem fileOpen;

    @FXML
    private MenuItem fileClose;

    @FXML
    private MenuItem fileSave;

    @FXML
    private MenuItem fileQuit;

    @FXML
    private MenuItem editCopy;

    @FXML
    private MenuItem editPaste;

    @FXML
    private MenuItem editDelete;

    @FXML
    private MenuItem helpAbout;

    @FXML
    private RadioButton shapeCircle;

    @FXML
    private ToggleGroup shapes;

    @FXML
    private RadioButton shapeOval;

    @FXML
    private RadioButton shapeRect;

    @FXML
    private RadioButton shapeRoundRect;

    @FXML
    private RadioButton shapeText;

    @FXML
    private Slider zoomSlider;

    @FXML
    private Canvas canvas;

    @FXML
    private TableView<Shape> table;

    @FXML
    private TableColumn<Shape, String> columnShape;

    @FXML
    private TableColumn<Shape, Double> columnCenterX;

    @FXML
    private TableColumn<Shape, Double> columnCenterY;

    @FXML
    private TableColumn<Shape, Double> columnRadius;

    @FXML
    private TableColumn<Shape, Double> columnWidth;

    @FXML
    private TableColumn<Shape, Double> columnHeight;

    @FXML
    private TableColumn<Shape, Double> columnArcWidth;

    @FXML
    private TableColumn<Shape, Double> columnArcHeight;

    @FXML
    private TableColumn<Shape, Color> columnColor;

    @FXML
    private TableColumn<Shape, String> columnText;

    @FXML
    void initialize() {
        assert borderPane != null : "fx:id=\"borderPane\" was not injected: check your FXML file 'PR1.fxml'.";
        assert fileNew != null : "fx:id=\"fileNew\" was not injected: check your FXML file 'PR1.fxml'.";
        assert fileOpen != null : "fx:id=\"fileOpen\" was not injected: check your FXML file 'PR1.fxml'.";
        assert fileClose != null : "fx:id=\"fileClose\" was not injected: check your FXML file 'PR1.fxml'.";
        assert fileSave != null : "fx:id=\"fileSave\" was not injected: check your FXML file 'PR1.fxml'.";
        assert fileQuit != null : "fx:id=\"fileQuit\" was not injected: check your FXML file 'PR1.fxml'.";
        assert editCopy != null : "fx:id=\"editCopy\" was not injected: check your FXML file 'PR1.fxml'.";
        assert editPaste != null : "fx:id=\"editPaste\" was not injected: check your FXML file 'PR1.fxml'.";
        assert editDelete != null : "fx:id=\"editDelete\" was not injected: check your FXML file 'PR1.fxml'.";
        assert helpAbout != null : "fx:id=\"helpAbout\" was not injected: check your FXML file 'PR1.fxml'.";
        assert shapeCircle != null : "fx:id=\"shapeCircle\" was not injected: check your FXML file 'PR1.fxml'.";
        assert shapes != null : "fx:id=\"shapes\" was not injected: check your FXML file 'PR1.fxml'.";
        assert shapeOval != null : "fx:id=\"shapeOval\" was not injected: check your FXML file 'PR1.fxml'.";
        assert shapeRect != null : "fx:id=\"shapeRect\" was not injected: check your FXML file 'PR1.fxml'.";
        assert shapeRoundRect != null : "fx:id=\"shapeRoundRect\" was not injected: check your FXML file 'PR1.fxml'.";
        assert shapeText != null : "fx:id=\"shapeText\" was not injected: check your FXML file 'PR1.fxml'.";
        assert zoomSlider != null : "fx:id=\"zoomSlider\" was not injected: check your FXML file 'PR1.fxml'.";
        assert canvas != null : "fx:id=\"canvas\" was not injected: check your FXML file 'PR1.fxml'.";
        assert table != null : "fx:id=\"table\" was not injected: check your FXML file 'PR1.fxml'.";
        assert columnShape != null : "fx:id=\"columnShape\" was not injected: check your FXML file 'PR1.fxml'.";
        assert columnCenterX != null : "fx:id=\"columnCenterX\" was not injected: check your FXML file 'PR1.fxml'.";
        assert columnCenterY != null : "fx:id=\"columnCenterY\" was not injected: check your FXML file 'PR1.fxml'.";
        assert columnRadius != null : "fx:id=\"columnRadius\" was not injected: check your FXML file 'PR1.fxml'.";
        assert columnWidth != null : "fx:id=\"columnWidth\" was not injected: check your FXML file 'PR1.fxml'.";
        assert columnHeight != null : "fx:id=\"columnHeight\" was not injected: check your FXML file 'PR1.fxml'.";
        assert columnArcWidth != null : "fx:id=\"columnArcWidth\" was not injected: check your FXML file 'PR1.fxml'.";
        assert columnArcHeight != null : "fx:id=\"columnArcHeight\" was not injected: check your FXML file 'PR1.fxml'.";
        assert columnColor != null : "fx:id=\"columnColor\" was not injected: check your FXML file 'PR1.fxml'.";
        assert columnText != null : "fx:id=\"columnText\" was not injected: check your FXML file 'PR1.fxml'.";

    }
    
    // Initialize all controllers.
    public void startApp() {
      PropertiesController properties = new PropertiesController((Stage)borderPane.getScene().getWindow());
      
      properties.model = new PR1Model();
      
      new MenuBarController(fileNew, fileOpen, fileClose, 
      		fileSave, fileQuit, editCopy, editPaste, editDelete, helpAbout, 
      		properties);
      
      new CanvasController(canvas, properties);
      
      new TableController(table, columnShape, columnCenterX, columnCenterY, 
      		columnRadius, columnWidth, columnHeight, columnArcWidth, 
      		columnArcHeight, columnColor, columnText, properties);
      
      new ToolbarController(shapes, shapeCircle, shapeOval, shapeRect, 
      		shapeRoundRect, shapeText, zoomSlider, properties);
    }
}

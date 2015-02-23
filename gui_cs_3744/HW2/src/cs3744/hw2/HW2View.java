package cs3744.hw2;

import java.io.File;

import cs3744.hw2.HW2Model.Circle;
import javafx.beans.property.ObjectProperty;
import javafx.beans.property.SimpleObjectProperty;
import javafx.beans.value.ChangeListener;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.control.Button;
import javafx.scene.control.ColorPicker;
import javafx.scene.control.Menu;
import javafx.scene.control.MenuBar;
import javafx.scene.control.MenuItem;
import javafx.scene.control.SeparatorMenuItem;
import javafx.scene.control.TextArea;
import javafx.scene.input.KeyCombination;
import javafx.scene.layout.BorderPane;
import javafx.scene.paint.Color;
import javafx.stage.Modality;
import javafx.stage.Stage;

/**
 * This class represents the view of the applications and handles all drawing
 * and canvas modifications.
 * 
 * @author Elmer Landaverde
 *
 */
public class HW2View extends BorderPane {

	private ObjectProperty<File>	file		= null;
	private ObjectProperty<Color>	color		= null;
	public Stage									stage		= null;
	private Canvas								canvas	= null;
	private GraphicsContext				gc			= null;

	// Object for menu bar
	private MenuBar								menubar;
	private Menu									fileMenu;
	private Menu									editMenu;
	private Menu									helpMenu;
	public MenuItem								fileNew;
	public MenuItem								fileOpen;
	public MenuItem								fileSave;
	public MenuItem								fileClose;
	public MenuItem								fileQuit;
	public MenuItem								editCopy;
	public MenuItem								editPaste;
	public MenuItem								editDelete;
	public MenuItem								helpAbout;

	/**
	 * Creates a new view object based on the given stage.
	 * 
	 * @param s The stage.
	 */
	public HW2View(Stage s) {
		super();
		stage = s;
		file = new SimpleObjectProperty<File>();
		file.setValue(null);
		color = new SimpleObjectProperty<Color>();
		color.setValue(Color.RED);
		createMenuBar();
		canvas = new Canvas(800, 550);
		canvas.widthProperty().bind(this.widthProperty());
		canvas.heightProperty().bind(this.heightProperty());
		gc = canvas.getGraphicsContext2D();
		setCenter(canvas);
		BorderPane.setAlignment(canvas, Pos.TOP_LEFT);
	}

	/**
	 * Create all menu bar, menus, and items.
	 */
	private void createMenuBar() {
		menubar = new MenuBar();

		// Create file menu
		fileMenu = new Menu("File");
		fileNew = new MenuItem("New");
		fileOpen = new MenuItem("Open");
		fileClose = new MenuItem("Close");
		fileClose.setDisable(true);
		fileSave = new MenuItem("Save");
		fileSave.setDisable(true);
		fileQuit = new MenuItem("Quit");
		SeparatorMenuItem s = new SeparatorMenuItem();

		fileNew.setAccelerator(KeyCombination.keyCombination("Ctrl+N"));
		fileOpen.setAccelerator(KeyCombination.keyCombination("Ctrl+O"));
		fileClose.setAccelerator(KeyCombination.keyCombination("Ctrl+W"));
		fileSave.setAccelerator(KeyCombination.keyCombination("Ctrl+S"));
		fileQuit.setAccelerator(KeyCombination.keyCombination("Ctrl+Q"));

		fileMenu.getItems().addAll(fileNew, fileOpen, fileClose, fileSave, s,
				fileQuit);

		// Create edit menu
		editMenu = new Menu("Edit");
		editCopy = new MenuItem("Copy");
		editPaste = new MenuItem("Paste");
		editDelete = new MenuItem("Delete");

		editCopy.setDisable(true);
		editPaste.setDisable(true);
		editDelete.setDisable(true);

		editCopy.setAccelerator(KeyCombination.keyCombination("Ctrl+C"));
		editPaste.setAccelerator(KeyCombination.keyCombination("Ctrl+V"));
		editDelete.setAccelerator(KeyCombination.keyCombination("Ctrl+X"));

		editMenu.getItems().addAll(editCopy, editPaste, editDelete);

		// Create about menu
		helpMenu = new Menu("Help");
		helpAbout = new MenuItem("About");

		helpAbout.setAccelerator(KeyCombination.keyCombination("Ctrl+A"));

		helpMenu.getItems().addAll(helpAbout);

		menubar.getMenus().addAll(fileMenu, editMenu, helpMenu);
		setTop(menubar);
	}

	public void addCanvasSizeListener(ChangeListener<Number> listener) {
		canvas.widthProperty().addListener(listener);
		canvas.heightProperty().addListener(listener);
	}

	/**
	 * Add event handler to all menu items.
	 * 
	 * @param handler The event handler.
	 */
	public void addMenuBarHandler(EventHandler<ActionEvent> handler) {
		fileNew.setOnAction(handler);
		fileOpen.setOnAction(handler);
		fileSave.setOnAction(handler);
		fileClose.setOnAction(handler);
		fileQuit.setOnAction(handler);
		editCopy.setOnAction(handler);
		editPaste.setOnAction(handler);
		editDelete.setOnAction(handler);
		helpAbout.setOnAction(handler);
	}

	/**
	 * Get the file object property.
	 * 
	 * @return The file object property.
	 */
	public ObjectProperty<File> fileProperty() {
		return file;
	}

	/**
	 * Get the color object property.
	 * 
	 * @return The color object property.
	 */
	public ObjectProperty<Color> colorProperty() {
		return color;
	}

	/**
	 * Get the canvas object of this view.
	 * 
	 * @return The canvas object.
	 */
	public Canvas getCanvas() {
		return canvas;
	}

	/**
	 * Clear canvas.
	 */
	public void clear() {
		gc.clearRect(0, 0, canvas.getWidth(), canvas.getHeight());
	}

	/**
	 * Draw a circle based on the circle object 'c'.
	 * 
	 * @param c The circle object to be drawn.
	 */
	public void draw(Circle c) {
		if (c.stroke) { // Draw ring
			gc.setStroke(new Color(c.red, c.green, c.blue, 1.0));
			gc.setLineWidth(5);
			gc.strokeOval(c.getTopLeftX(), c.getTopLeftY(), c.getSideLength(),
					c.getSideLength());
		} else { // Draw circle
			gc.setFill(new Color(c.red, c.green, c.blue, 1.0));
			gc.fillOval(c.getTopLeftX(), c.getTopLeftY(), c.getSideLength(),
					c.getSideLength());
		}
	}

	/**
	 * Show color dialog.
	 * @param defaultColor The default color of the color picker.
	 */
	public void showColorDialog(Color defaultColor) {
		ColorDialog d = new ColorDialog(defaultColor);
		d.show();
	}

	/**
	 * Show about dialog.
	 */
	public void showAboutDialog() {
		MessageDialog d = new MessageDialog();
		d.show();
	}

	/**
	 * A dialog class that contains information about this homework assignment.
	 * 
	 * @author Elmer Landaverde
	 *
	 */
	class MessageDialog extends Stage {

		/**
		 * Creates an instance of <code>MessageDialog</code> class.
		 * 
		 * @param title The dialog title.
		 * @param message The dialog message.
		 */
		public MessageDialog() {
			super();
			setTitle("About Homework 2");
			setResizable(false);
			initModality(Modality.APPLICATION_MODAL);

			// Create layout object
			BorderPane root = new BorderPane();
			root.getStyleClass().add("message-dialog");
			// Create text area
			TextArea messageArea = new TextArea("Homework 2 version 1.");
			messageArea.setEditable(false);
			messageArea.setWrapText(true);
			messageArea.setFocusTraversable(false);
			root.setCenter(messageArea);
			root.getStyleClass().add("about-dialog");
			// Create button
			Button okButton = new Button("OK");
			okButton.setOnAction((e) -> {
				close();
			});
			BorderPane.setAlignment(okButton, Pos.CENTER);
			BorderPane.setMargin(okButton, new Insets(10, 10, 10, 10));
			root.setBottom(okButton);

			Scene scene = new Scene(root, 200, 150);
			try {
				scene.getStylesheets().add(
						getClass().getResource("hw2.css").toExternalForm());
			} catch (Exception e) {
				e.printStackTrace();
			}
			setScene(scene);
		}
	}

	/**
	 * A dialog class that contains a color picker.
	 * 
	 * @author Elmer Landaverde
	 *
	 */
	class ColorDialog extends Stage {

		/**
		 * Creates an instance of <code>MessageDialog</code> class.
		 * 
		 * @param title The dialog title.
		 * @param message The dialog message.
		 */
		public ColorDialog(Color defaultColor) {
			super();
			setTitle("About Homework 2");
			setResizable(false);
			initModality(Modality.APPLICATION_MODAL);

			// Create layout object
			BorderPane root = new BorderPane();
			root.getStyleClass().add("color-dialog");
			// Create color picker
			final ColorPicker colorPicker = new ColorPicker();
			colorPicker.setValue(defaultColor);
			root.setCenter(colorPicker);
			root.getStyleClass().add("message-dialog");
			// Create button
			Button okButton = new Button("OK");
			okButton.setOnAction((e) -> {
				color.setValue(colorPicker.getValue());
				close();
			});
			BorderPane.setAlignment(okButton, Pos.CENTER);
			BorderPane.setMargin(okButton, new Insets(10, 10, 10, 10));
			root.setBottom(okButton);

			Scene scene = new Scene(root, 200, 150);
			try {
				scene.getStylesheets().add(
						getClass().getResource("hw2.css").toExternalForm());
			} catch (Exception e) {
				e.printStackTrace();
			}
			setScene(scene);
		}
	}
}

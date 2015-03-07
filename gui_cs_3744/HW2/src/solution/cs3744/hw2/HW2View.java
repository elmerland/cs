package solution.cs3744.hw2;

import java.io.File;

import javafx.beans.property.ObjectProperty;
import javafx.beans.property.SimpleObjectProperty;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
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
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Priority;
import javafx.scene.paint.Color;
import javafx.stage.FileChooser;
import javafx.stage.Modality;
import javafx.stage.Stage;

/**
 * Homework 2 view class. Provides the GUI.
 * 
 * @author Denis Gracanin
 * @version 1
 */
public class HW2View extends BorderPane {
	private Stage stage = null;
	private Canvas canvas = null;
	private GraphicsContext gc = null;

	private MenuItem fileNew = null;
	private MenuItem fileOpen = null;
	private MenuItem fileClose = null;
	private MenuItem fileSave = null;
	private MenuItem fileQuit = null;

	private MenuItem editPaste = null;
	private MenuItem editCopy = null;
	private MenuItem editDelete = null;

	public static enum ViewState {
		NEW, OPEN, OPENED, CLOSE, CLOSED, MODIFIED, SAVE, QUIT, RESIZE
	};

	private ObjectProperty<ViewState> viewState = null;

	public static enum ClipboardState {
		IDLE, COPY, PASTE, DELETE
	};

	private ObjectProperty<ClipboardState> clipboardState = null;
	private ChangeListener<Number> canvasListener = null;
	private File file = null;

	private static final String ABOUT_TITLE = "About Homework 2";
	private static final String ABOUT_MESSAGE = "Homework 2 version 1.";
	private static final String COLOR_TITLE = "Circle Color";
	private static final String CSS_FILE = "hw2.css";

	/**
	 * Creates GUI and shows the application window.
	 * 
	 * @param s Top level container.
	 */
	public HW2View(Stage s) {
		super();
		stage = s;
		setFile(null);
		viewState = new SimpleObjectProperty<ViewState>(ViewState.CLOSE);
		clipboardState = new SimpleObjectProperty<ClipboardState>(
				ClipboardState.IDLE);
		canvasListener = new ChangeListener<Number>() {

			/**
			 * Handles the canvas resizing.
			 * 
			 * @param event The event object.
			 */
			@Override
			public void changed(ObservableValue<? extends Number> observable,
					Number oldValue, Number newValue) {
				viewState.set(ViewState.RESIZE);
			}
		};

		MenuBar menubar = new MenuBar();

		Menu menuFile = new Menu("File");

		fileNew = new MenuItem("New");
		fileNew.setAccelerator(KeyCombination.keyCombination("Ctrl+N"));
		fileNew.setOnAction((e) -> {
			FileChooser fileChooser = new FileChooser();
			fileChooser.setTitle("Open New File");
			File f = fileChooser.showSaveDialog(stage);
			if (f != null) {
				setFile(f);
				viewState.set(ViewState.NEW);
			}
		});

		fileOpen = new MenuItem("Open");
		fileOpen.setAccelerator(KeyCombination.keyCombination("Ctrl+O"));
		fileOpen.setOnAction((e) -> {
			FileChooser fileChooser = new FileChooser();
			fileChooser.setTitle("Open File");
			File f = fileChooser.showOpenDialog(stage);
			if (f != null) {
				setFile(f);
				viewState.set(ViewState.OPEN);
			}
		});

		fileClose = new MenuItem("Close");
		fileClose.setAccelerator(KeyCombination.keyCombination("Ctrl+W"));
		fileClose.setOnAction((e) -> viewState.set(ViewState.CLOSE));

		fileSave = new MenuItem("Save");
		fileSave.setAccelerator(KeyCombination.keyCombination("Ctrl+S"));
		fileSave.setOnAction((e) -> viewState.set(ViewState.SAVE));

		SeparatorMenuItem separatorOne = new SeparatorMenuItem();

		fileQuit = new MenuItem("Quit");
		fileQuit.setAccelerator(KeyCombination.keyCombination("Ctrl+Q"));
		fileQuit.setOnAction((e) -> viewState.set(ViewState.QUIT));

		menuFile.getItems().addAll(fileNew, fileOpen, fileClose, fileSave,
				separatorOne, fileQuit);

		Menu menuEdit = new Menu("Edit");

		editCopy = new MenuItem("Copy");
		editCopy.setAccelerator(KeyCombination.keyCombination("Ctrl+C"));
		editCopy.setOnAction((e) -> clipboardState.set(ClipboardState.COPY));

		editPaste = new MenuItem("Paste");
		editPaste.setAccelerator(KeyCombination.keyCombination("Ctrl+V"));
		editPaste.setOnAction((e) -> clipboardState.set(ClipboardState.PASTE));

		editDelete = new MenuItem("Delete");
		editDelete.setAccelerator(KeyCombination.keyCombination("Ctrl+X"));
		editDelete.setOnAction((e) -> clipboardState.set(ClipboardState.DELETE));
		menuEdit.getItems().addAll(editCopy, editPaste, editDelete);

		Menu menuHelp = new Menu("Help");

		MenuItem about = new MenuItem("About");
		about.setOnAction((e) -> showMessageDialog(ABOUT_TITLE, ABOUT_MESSAGE));
		menuHelp.getItems().addAll(about);

		menubar.getMenus().addAll(menuFile, menuEdit, menuHelp);
		setTop(menubar);

		canvas = new Canvas();
		gc = canvas.getGraphicsContext2D();
		setCenter(canvas);
		canvas.widthProperty().bind(widthProperty());
		canvas.heightProperty().bind(
				heightProperty().subtract(menubar.heightProperty()));
		canvas.heightProperty().addListener(canvasListener);
		canvas.widthProperty().addListener(canvasListener);
	}

	/**
	 * Draws a circle.
	 * 
	 * @param x The x-coordinate of the center of the circle.
	 * @param y The y-coordinate of the center of the circle.
	 * @param r The radius of the circle
	 * @param c The color of the circle.
	 * @param selection If true, draws the circle, otherwise the selection ring.
	 */
	public void drawCircle(double x, double y, double r, Color c,
			boolean selection) {
		if (selection) {
			gc.setStroke(Color.RED);
			gc.setLineWidth(3);
			gc.strokeOval(x - r - 3, y - r - 3, 2 * r + 6, 2 * r + 6);
		} else {
			gc.setFill(c);
			gc.fillOval(x - r, y - r, 2 * r, 2 * r);
		}
	}

	/**
	 * Clears the view.
	 */
	public void clear() {
		gc.clearRect(0, 0, canvas.getWidth(), canvas.getHeight());
	}

	/**
	 * Gets the current file.
	 * 
	 * @return The file.
	 */
	public File getFile() {
		return file;
	}

	/**
	 * Sets the current file.
	 * 
	 * @param f The file.
	 */
	public void setFile(File f) {
		file = f;
	}

	/**
	 * Gets the view state property.
	 * 
	 * @return The view state property.
	 */
	public ObjectProperty<ViewState> viewStateProperty() {
		return viewState;
	}

	/**
	 * Gets the view state property.
	 * 
	 * @return The view state.
	 */
	public ViewState getViewState() {
		return viewState.get();
	}

	/**
	 * Sets the view state property.
	 * 
	 * @param v The view state.
	 */
	public void setViewState(ViewState v) {
		viewState.set(v);
	}

	/**
	 * Gets the clipboard state property.
	 * 
	 * @return The clipboard state property.
	 */
	public ObjectProperty<ClipboardState> clipboardStateProperty() {
		return clipboardState;
	}

	/**
	 * Gets the clipboard state.
	 * 
	 * @return The clipboard state.
	 */
	public ClipboardState getClipboardState() {
		return clipboardState.get();
	}

	/**
	 * Sets the clipboard state.
	 * 
	 * @param v The clipboard.
	 */
	public void setClipboardState(ClipboardState v) {
		clipboardState.set(v);
	}

	/**
	 * Adds the mouse event handler to the canvas.
	 * 
	 * @param eh The mouse event handler.
	 */
	public void addMouseListener(EventHandler<MouseEvent> eh) {
		canvas.addEventHandler(MouseEvent.MOUSE_PRESSED, eh);
		canvas.addEventHandler(MouseEvent.MOUSE_DRAGGED, eh);
		canvas.addEventHandler(MouseEvent.MOUSE_RELEASED, eh);
		canvas.addEventHandler(MouseEvent.MOUSE_CLICKED, eh);
		canvas.addEventHandler(MouseEvent.MOUSE_MOVED, eh);
	}

	/**
	 * Enables/disables the File menu items.
	 * 
	 * @param n The File menu New menu item disabled.
	 * @param o The File menu Open menu item disabled.
	 * @param c The File menu Close menu item disabled.
	 * @param s The File menu Save menu item disabled.
	 * @param q The File menu Quit menu item disabled.
	 */
	public void setFileMenu(boolean n, boolean o, boolean c, boolean s, boolean q) {
		fileNew.setDisable(n);
		fileOpen.setDisable(o);
		fileClose.setDisable(c);
		fileSave.setDisable(s);
		fileQuit.setDisable(q);
	}

	/**
	 * Enables/disables the Edit menu items.
	 * 
	 * @param c The Edit menu Copy menu item disabled.
	 * @param p The Edit menu Paste menu item disabled.
	 * @param d The Edit menu Delete menu item disabled.
	 */
	public void setEditMenu(boolean c, boolean p, boolean d) {
		editCopy.setDisable(c);
		editPaste.setDisable(p);
		editDelete.setDisable(d);
	}

	/**
	 * Shows the color dialog.
	 * 
	 * @param c The color
	 * @return The picked color.
	 */
	public Color showColorDialog(Color c) {
		Color color = null;
		if (c != null) {
			ColorDialog dialog = new ColorDialog(COLOR_TITLE, c);
			color = dialog.pick();
		}
		return color;
	}

	/**
	 * Shows the message dialog.
	 * 
	 * @param title The dialog title.
	 * @param message The dialog message.
	 */
	public void showMessageDialog(String title, String message) {
		MessageDialog dialog = new MessageDialog(title, message);
		dialog.show();
	}

	/**
	 * Shows the choice dialog.
	 * 
	 * @param title The title.
	 * @param message The message.
	 * @return The choice.
	 */
	public boolean showChoiceDialog(String title, String message) {
		ChoiceDialog dialog = new ChoiceDialog(title, message);
		return dialog.pick();
	}

	/**
	 * Creates the color dialog.
	 * 
	 * @author Denis Gracanin
	 * @version 1
	 */
	public class ColorDialog extends Stage {
		private Color color;

		/**
		 * Creates an instance of <code>ColorDialog</code> class.
		 * 
		 * @param title The dialog title.
		 * @param c The dialog color.
		 */
		public ColorDialog(String title, Color c) {
			super();
			color = c;
			setTitle(title);
			setResizable(false);
			initModality(Modality.APPLICATION_MODAL);

			BorderPane root = new BorderPane();
			root.getStyleClass().add("color-dialog");
			ColorPicker colorPicker = new ColorPicker(c);
			root.setCenter(colorPicker);
			root.getStyleClass().add("color-dialog");
			Button okButton = new Button("OK");
			okButton.setOnAction((e) -> {
				color = colorPicker.getValue();
				close();
			});
			BorderPane.setAlignment(okButton, Pos.CENTER);
			BorderPane.setMargin(okButton, new Insets(10, 10, 10, 10));
			root.setBottom(okButton);

			Scene scene = new Scene(root, 200, 150);
			try {
				scene.getStylesheets().add(
						getClass().getResource(CSS_FILE).toExternalForm());
			} catch (Exception e) {
				e.printStackTrace();
			}
			setScene(scene);
		}

		/**
		 * Returns the color value selected in the color picker.
		 * 
		 * @return The selected color value.
		 */
		public Color pick() {
			showAndWait();
			return color;
		}

	}

	/**
	 * Creates the message dialog.
	 * 
	 * @author Denis Gracanin
	 * @version 1
	 */
	public class MessageDialog extends Stage {

		/**
		 * Creates an instance of <code>MessageDialog</code> class.
		 * 
		 * @param title The dialog title.
		 * @param message The dialog message.
		 */
		public MessageDialog(String title, String message) {
			super();
			setTitle(title);
			setResizable(false);
			initModality(Modality.APPLICATION_MODAL);

			BorderPane root = new BorderPane();
			root.getStyleClass().add("message-dialog");
			TextArea messageArea = new TextArea(message);
			messageArea.setEditable(false);
			messageArea.setWrapText(true);
			messageArea.setFocusTraversable(false);
			root.setCenter(messageArea);
			root.getStyleClass().add("message-dialog");
			Button okButton = new Button("OK");
			okButton.setOnAction((e) -> close());
			BorderPane.setAlignment(okButton, Pos.CENTER);
			BorderPane.setMargin(okButton, new Insets(10, 10, 10, 10));
			root.setBottom(okButton);

			Scene scene = new Scene(root, 200, 150);
			try {
				scene.getStylesheets().add(
						getClass().getResource(CSS_FILE).toExternalForm());
			} catch (Exception e) {
				e.printStackTrace();
			}
			setScene(scene);
		}

	}

	/**
	 * Creates the choice dialog.
	 * 
	 * @author Denis Gracanin
	 * @version 1
	 */
	public class ChoiceDialog extends Stage {
		private boolean ok = false;

		/**
		 * Creates an instance of <code>MessageDialog</code> class.
		 * 
		 * @param title The dialog title.
		 * @param message The dialog message.
		 */
		public ChoiceDialog(String title, String message) {
			super();
			setTitle(title);
			setResizable(false);
			initModality(Modality.APPLICATION_MODAL);

			BorderPane root = new BorderPane();
			root.getStyleClass().add("choice-dialog");
			TextArea messageArea = new TextArea(message);
			messageArea.setEditable(false);
			messageArea.setWrapText(true);
			messageArea.setFocusTraversable(false);
			root.setCenter(messageArea);
			root.getStyleClass().add("choice-dialog");
			Button okButton = new Button("OK");
			okButton.setOnAction((e) -> {
				ok = true;
				close();
			});
			Button cancelButton = new Button("Cancel");
			cancelButton.setOnAction((e) -> {
				ok = false;
				close();
			});
			HBox hbox = new HBox(20);
			HBox.setHgrow(okButton, Priority.ALWAYS);
			HBox.setHgrow(cancelButton, Priority.ALWAYS);
			okButton.setMaxWidth(Double.MAX_VALUE);
			cancelButton.setMaxWidth(Double.MAX_VALUE);
			hbox.getChildren().addAll(okButton, cancelButton);
			BorderPane.setAlignment(hbox, Pos.CENTER);
			BorderPane.setMargin(hbox, new Insets(10, 10, 10, 10));
			root.setBottom(hbox);

			Scene scene = new Scene(root, 200, 150);
			try {
				scene.getStylesheets().add(
						getClass().getResource(CSS_FILE).toExternalForm());
			} catch (Exception e) {
				e.printStackTrace();
			}
			setScene(scene);
		}

		/**
		 * Returns the boolean value.
		 * 
		 * @return The boolean.
		 */
		public boolean pick() {
			showAndWait();
			return ok;
		}

	}

}

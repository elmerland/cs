package cs3744.hw3;

import java.io.File;

import cs3744.hw3.HW3Model.HW3Circle;
import javafx.beans.property.ObjectProperty;
import javafx.beans.property.SimpleObjectProperty;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.geometry.Orientation;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.control.Button;
import javafx.scene.control.ColorPicker;
import javafx.scene.control.ContentDisplay;
import javafx.scene.control.Menu;
import javafx.scene.control.MenuBar;
import javafx.scene.control.MenuItem;
import javafx.scene.control.ScrollPane;
import javafx.scene.control.SeparatorMenuItem;
import javafx.scene.control.SplitPane;
import javafx.scene.control.TableCell;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableColumn.CellEditEvent;
import javafx.scene.control.TableView;
import javafx.scene.control.TextArea;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.control.cell.TextFieldTableCell;
import javafx.scene.input.KeyCombination;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Priority;
import javafx.scene.paint.Color;
import javafx.stage.FileChooser;
import javafx.stage.Modality;
import javafx.stage.Stage;
import javafx.util.Callback;
import javafx.util.converter.DoubleStringConverter;

/**
 * Homework 3 view class. Provides the GUI.
 * 
 * Code was adapted from homework 2 solution.
 * 
 * @author Elmer Landaverde
 * @version 1
 */
public class HW3View extends BorderPane {
	private Stage stage = null;
	private Canvas canvas = null;
	private GraphicsContext gc = null;
	private TableView<HW3Circle> table = null;

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
	private static final String CSS_FILE = "hw3.css";
	private static final double CANVAS_WIDTH = 1000;
	private static final double CANVAS_HEIGHT = 1000;

	/**
	 * Creates GUI and shows the application window.
	 * 
	 * @param s Top level container.
	 */
	public HW3View(Stage s) {
		super();
		stage = s;
		setFile(null);
		viewState = new SimpleObjectProperty<ViewState>(ViewState.CLOSED);
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

		MenuBar menubar = makeMenuBar();
		setTop(menubar);

		SplitPane splitPane = new SplitPane();
		splitPane.prefWidthProperty().bind(widthProperty());
		splitPane.prefHeightProperty().bind(
				heightProperty().subtract(menubar.heightProperty()));
		splitPane.setOrientation(Orientation.HORIZONTAL);

		canvas = new Canvas();
		gc = canvas.getGraphicsContext2D();
		canvas.setWidth(CANVAS_WIDTH);
		canvas.setHeight(CANVAS_HEIGHT);
		canvas.heightProperty().addListener(canvasListener);
		canvas.widthProperty().addListener(canvasListener);

		ScrollPane scrollpane = new ScrollPane();
		scrollpane.setContent(canvas);

		table = makeTableView();

		splitPane.getItems().addAll(scrollpane, table);
		setCenter(splitPane);

	}

	/**
	 * Creates the menu bar and all menu items.
	 * 
	 * @return The menu bar that was created.
	 */
	private MenuBar makeMenuBar() {
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
		return menubar;
	}

	/**
	 * Creates the table view and formatting for every column.
	 * 
	 * @return The table view that was created.
	 */
	@SuppressWarnings("unchecked")
	private TableView<HW3Circle> makeTableView() {
		TableView<HW3Circle> tableview = new TableView<HW3Circle>();
		tableview.setEditable(true);

		TableColumn<HW3Circle, Double> centerxCol = new TableColumn<>("Center X");
		centerxCol.setMinWidth(80);
		centerxCol.setCellValueFactory(new PropertyValueFactory<>("centerX"));
		centerxCol.setCellFactory(TextFieldTableCell
				.<HW3Circle, Double> forTableColumn(new DoubleStringConverter()));
		centerxCol.setOnEditCommit((CellEditEvent<HW3Circle, Double> t) -> {
			HW3Circle c = (HW3Circle) t.getTableView().getItems()
					.get(t.getTablePosition().getRow());
			c.setCenterX(t.getNewValue());
		});

		TableColumn<HW3Circle, Double> centeryCol = new TableColumn<>("Center Y");
		centeryCol.setMinWidth(80);
		centeryCol.setCellValueFactory(new PropertyValueFactory<>("centerY"));
		centeryCol.setCellFactory(TextFieldTableCell
				.<HW3Circle, Double> forTableColumn(new DoubleStringConverter()));
		centeryCol.setOnEditCommit((CellEditEvent<HW3Circle, Double> t) -> {
			HW3Circle c = (HW3Circle) t.getTableView().getItems()
					.get(t.getTablePosition().getRow());
			c.setCenterY(t.getNewValue());
		});

		TableColumn<HW3Circle, Double> radiusCol = new TableColumn<>("Radius");
		radiusCol.setMinWidth(80);
		radiusCol.setCellValueFactory(new PropertyValueFactory<>("radius"));
		radiusCol.setCellFactory(TextFieldTableCell
				.<HW3Circle, Double> forTableColumn(new DoubleStringConverter()));
		radiusCol.setOnEditCommit((CellEditEvent<HW3Circle, Double> t) -> {
			HW3Circle c = (HW3Circle) t.getTableView().getItems()
					.get(t.getTablePosition().getRow());
			c.setRadius(t.getNewValue());
		});

		TableColumn<HW3Circle, Color> colorCol = new TableColumn<>("Color");
		colorCol.setMinWidth(80);
		colorCol.setCellValueFactory(new PropertyValueFactory<>("color"));
		colorCol
				.setCellFactory(new Callback<TableColumn<HW3Circle, Color>, TableCell<HW3Circle, Color>>() {

					@Override
					public TableCell<HW3Circle, Color> call(
							TableColumn<HW3Circle, Color> param) {
						return new ColorPickerTableCell();
					}
				});
		colorCol.setOnEditCommit((CellEditEvent<HW3Circle, Color> t) -> {
			HW3Circle c = (HW3Circle) t.getTableView().getItems()
					.get(t.getTablePosition().getRow());
			c.setColor(t.getNewValue());
		});

		tableview.getColumns().addAll(centerxCol, centeryCol, radiusCol, colorCol);
		return tableview;
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
	 * Get the table view object.
	 * 
	 * @return The table view.
	 */
	public TableView<HW3Circle> getTable() {
		return table;
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
	 * Custom color picker table cell.
	 * 
	 * @author Elmer Landaverde
	 *
	 */
	public class ColorPickerTableCell extends TableCell<HW3Circle, Color> {
		private ColorPicker cp;

		public ColorPickerTableCell() {
			cp = new ColorPicker();
			cp.setOnAction((ActionEvent e) -> {
				commitEdit(cp.getValue());
				updateItem(cp.getValue(), isEmpty());
				HW3Circle c = (HW3Circle) getTableView().getItems().get(
						getTableRow().getIndex());
				c.setColor(cp.getValue());
			});
			setGraphic(cp);
			setContentDisplay(ContentDisplay.GRAPHIC_ONLY);
			setEditable(true);
		}

		protected void updateItem(Color item, boolean empty) {
			super.updateItem(item, empty);
			cp.setVisible(!empty);
			this.setItem(item);
			cp.setValue(item);
		}
	}

	/**
	 * Creates the color dialog.
	 * 
	 * @author Elmer Landaverde
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
	 * @author Elmer Landaverde
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
	 * @author Elmer Landaverde
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

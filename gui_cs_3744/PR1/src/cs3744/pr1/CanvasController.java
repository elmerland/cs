package cs3744.pr1;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.StandardOpenOption;

import cs3744.pr1.PropertiesController.ClipboardState;
import cs3744.pr1.PropertiesController.ViewState;
import javafx.beans.value.ObservableValue;
import javafx.collections.ListChangeListener;
import javafx.event.EventHandler;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.input.MouseButton;
import javafx.scene.input.MouseEvent;
import javafx.scene.paint.Color;
import javafx.scene.text.Font;
import javafx.stage.Modality;
import javafx.stage.Stage;
import javafx.stage.StageStyle;

/**
 * This class controls the canvas. It performs any drawing or manipulation.
 *
 */
public class CanvasController {
	
	private Canvas canvas = null;
	private GraphicsContext gc = null;
	private PropertiesController props;
	
	private static final String CHOICE_TITLE = "File Save";
	private static final String CHOICE_MESSAGE = "File not saved. Proceed?";
	private static final String COLOR_TITLE = "Circle Color";
	
	public CanvasController(Canvas canvas, PropertiesController properties) {
		this.props = properties;
		this.props.canvas = this;
		this.canvas = canvas;
		this.gc = this.canvas.getGraphicsContext2D();
		gc.setFont(new Font("Courier", 12));
		
		props.selection.addListener((ObservableValue<? extends Shape> observable,
				Shape oldValue, Shape newValue) -> {
			repaint();
		});
		props.model.drawDataProperty().addListener(
				(ListChangeListener.Change<? extends Shape> e) -> {
					repaint();
				});
		props.viewState.addListener(
				(ObservableValue<? extends ViewState> observable,
						ViewState oldValue, ViewState newValue) -> {
					updateViewState(oldValue, newValue);
				});

		props.clipboardState
				.addListener(
						(ObservableValue<? extends ClipboardState> observable,
								ClipboardState oldValue, ClipboardState newValue) -> {
							updateClipboardState(oldValue, newValue);
						});
		addMouseListener((MouseEvent e) -> {
			handleMouseEvent(e);
		});
	}
	
	/**
	 * Handles the view state changes (From File menu and window resizing).
	 * 
	 * @param oldValue The old view state.
	 * @param newValue The new view state.
	 */
	private void updateViewState(ViewState oldValue,
			ViewState newValue) {
		if (!newValue.equals(oldValue)) {
			switch (newValue) {
				case CLOSED: // No file opened (when the application starts or when the current file is closed.
					props.menubar.setFileMenu(false, false, true, true, false); // Configures individual file menu items (enabled/disabled).
					if (props.zoomSlider != null) props.zoomSlider.setDisable(true);
					break;
				case NEW: // A new file to be opened.
					if (props.file != null) {
						if (props.file.exists()) {
							props.file.delete(); // Delete the file if a file with that name already exists (overwrite).
						}
					}
					if (props.zoomSlider != null) props.zoomSlider.setDisable(false);
				case OPEN: // An existing file opened.
					if (props.file != null) {
						Charset charset = Charset.forName("US-ASCII");
						try {
							props.file.createNewFile();
							BufferedReader reader = Files.newBufferedReader(props.file.toPath(), charset);
							props.file.createNewFile();
							String line = null;
							while ((line = reader.readLine()) != null) {
								try {
									props.model.add(line); // Read the file line by line and add the circle (line) to the model.
								} catch (NumberFormatException e) {} // Ignores an incorrectly formatted line.
								catch (ArrayIndexOutOfBoundsException e) {} // Ignores an incorrectly formatted line.
							}
							props.viewState.set(ViewState.OPENED);
						} catch (IOException e) {
							props.viewState.set(ViewState.CLOSE);
						}
					}
					break;
				case OPENED: // The file is opened.
					props.menubar.setFileMenu(true, true, false, true, false); // Configures individual file menu items (enabled/disabled).
					if (props.zoomSlider != null) props.zoomSlider.setDisable(false);
					break;
				case CLOSE: // The file has to be closed.
					props.file = null; // Clears the file.
					setSelection(null); // Clears the selection;
					setClipboard(null); // Clears the selection;
					props.model.clear(); // Clears the model.
					clear(); // Clears the view.
					props.viewState.set(ViewState.CLOSED);
					break;
				case MODIFIED: // The file has been modified, needs saving.
					props.menubar.setFileMenu(true, true, true, false, false);
					break;
				case SAVE: // Save the file.
					if (props.file != null) {
						Charset charset = Charset.forName("US-ASCII");
						try {
							BufferedWriter writer = Files.newBufferedWriter(props.file.toPath(), charset, StandardOpenOption.WRITE);
							for (Shape c : props.model.drawDataProperty()) {
								String line = c.toString() + "\n";
								writer.write(line);
							}
							writer.close();
							props.viewState.set(ViewState.OPENED);
						} catch (IOException e) {
							props.menubar.showMessageDialog("File Error", e.toString());
						}
					}
					break;
				case QUIT: // Quit the application
					if (oldValue == ViewState.MODIFIED
							&& !props.menubar.showChoiceDialog(CHOICE_TITLE, CHOICE_MESSAGE)) {
						props.viewState.set(oldValue);
						break;
					}
					System.exit(0);
					break;
				case RESIZE: // Redraw the view when the application window resizes.
					repaint();
					props.viewState.set(oldValue);
					break;
				default:
					break;
			}
		}
	}

	/**
	 * Handles clipboard changes.
	 * 
	 * @param oldValue Old clipboard state.
	 * @param newValue New clipboard state.
	 */
	private void updateClipboardState(ClipboardState oldValue,
			ClipboardState newValue) {
		Shape c = null;
		if (getSelection() != null) {
			switch (newValue) {
				case COPY: // Copy the selection to the clipboard. 
					setClipboard(getSelection());
					props.menubar.setEditMenu(false, false, false); // Enable all Edit menu items.			
					break;
				case PASTE: // Paste the clipboard to the view.
					c = props.model.add(props.defaultShape, props.mouseLastX, props.mouseLastY);
					c.setRadius(getClipboard().getRadius());
					c.setColor(getClipboard().getColor());
					props.viewState.set(ViewState.MODIFIED);
					break;
				case DELETE: // delete the selection.
					c = getSelection();
					setSelection(null);
					props.model.remove(c);
					props.menubar.setEditMenu(true, true, true); // Disable all Edit menu items.		
					props.viewState.set(ViewState.MODIFIED);
					break;
				case IDLE:
					break;
				default:
					break;
			}
			props.clipboardState.set(ClipboardState.IDLE);
		}
	}
	
	/**
	 * Handles mouse events.
	 * 
	 * @param e The mouse event.
	 */
	private void handleMouseEvent(MouseEvent e) {
		props.mouseLastX = e.getX();
		props.mouseLastY = e.getY();

		if (props.viewState.get() == ViewState.CLOSED) {
			return;
		} else if (e.getEventType() == MouseEvent.MOUSE_PRESSED) {
			props.table.getSelectionModel().clearSelection();
			setSelection(props.model.select(e.getX(), e.getY()));
			if (e.getButton() == MouseButton.PRIMARY) {
				if (getSelection() != null) {
					props.model.setTop(getSelection());
					props.table.getSelectionModel()
							.select(props.model.getIndex(getSelection()));
				}
				props.viewState.set(ViewState.MODIFIED);
			}
		} else if (e.getEventType() == MouseEvent.MOUSE_DRAGGED) {
			if (getSelection() != null) {
				if (e.getButton() == MouseButton.PRIMARY) {
					getSelection().setCenterX(e.getX());
					getSelection().setCenterY(e.getY());
					props.viewState.set(ViewState.MODIFIED);
				} else if (e.getButton() == MouseButton.SECONDARY) {
					getSelection().setRadius(getSelection().getRadius() + 0.25
									* (e.getX() - getSelection().getCenterX()));
					getSelection().setWidth(getSelection().getWidth() + 0.25
							* (e.getX() - getSelection().getCenterX()));
					getSelection().setHeight(getSelection().getHeight() + 0.25
							* (getSelection().getCenterY() - e.getY()));
					props.viewState.set(ViewState.MODIFIED);
				}
			}
		} else if (e.getEventType() == MouseEvent.MOUSE_CLICKED) {
			if (e.getButton().equals(MouseButton.PRIMARY)) {
				switch (e.getClickCount()) {
					case 1:
						if (getSelection() == null) {
							setSelection(props.model.add(props.defaultShape, e.getX(), e.getY()));
						}
						props.viewState.set(ViewState.MODIFIED);
						break;
					case 2:
						setSelection(props.model.select(e.getX(), e.getY()));
						getSelection().setColor(
								showColorDialog(getSelection().getColor()));
						props.viewState.set(ViewState.MODIFIED);
						break;
					default:
						break;
				}
			}
		}
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
	
	public Canvas getCanvas() { return canvas; }
	
	/**
	 * Gets the selected circle.
	 * 
	 * @return The selected circle.
	 */
	private Shape getSelection() {
		return props.selection.get();
	}

	/**
	 * Selects a circle.
	 * 
	 * @param s The circle to be selected.
	 */
	public void setSelection(Shape s) {
		props.selection.set(s);
		props.menubar.setEditMenu(getSelection() == null, true, getSelection() == null); // Enable the Edit menu items as needed.
	}

	/**
	 * Gets the clipboard's content.
	 * 
	 * @return The content.
	 */
	private Shape getClipboard() {
		return props.clipboard;
	}

	/**
	 * Sets the clipboard's content.
	 * 
	 * @param c The content.
	 */
	private void setClipboard(Shape c) {
		props.clipboard = c;
	}

	/**
	 * Repaints all the circles in the model. The selected circle (if any) has a
	 * red ring around it.
	 */
	private void repaint() {
		clear();
		for (Shape c : props.model.drawDataProperty()) {
			drawShape(c, false);
		}
		if (getSelection() != null) {
			Shape c = getSelection();
			drawShape(c, true);
		}
	}
	
	/**
	 * Clears the view.
	 */
	public void clear() {
		gc.clearRect(0, 0, canvas.getWidth(), canvas.getHeight());
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
	public void drawShape(Shape s, boolean selection) {
		double x = s.getCenterX();
		double y = s.getCenterY();
		double r = s.getRadius();
		double w = s.getWidth();
		double h = s.getHeight();
		double aw = s.getArcWidth();
		double ah = s.getArcHeight();
		Color c = s.getColor();
		String text = s.getText();
		if (selection) {
			gc.setStroke(Color.RED);
			gc.setLineWidth(3);
			switch (s.getShapeType()) {
				case CIRCLE:
					gc.strokeOval(x - r - 3, y - r - 3, 2 * r + 6, 2 * r + 6);
					break;
				case OVAL:
					gc.strokeOval(x - w/2 - 3, y - h/2 - 3, w + 6, h + 6);
					break;
				case RECT:
					gc.strokeRect(x - w/2 - 3, y - h/2 - 3, w + 6, h + 6);
					break;
				case ROUND_RECT:
					gc.strokeRoundRect(x - w/2 - 3, y - h/2 - 3, w + 6, h + 6, aw + 6, ah + 6);
					break;
				case TEXT:
					double width = text.length() * Shape.CHAR_LENGTH;
					double height = Shape.FONT_SIZE;
					gc.strokeRect(x - width/2 - 3, y - height - 3, width + 6, height + 6);
					break;
			}
		} else {
			gc.setFill(c);
			switch (s.getShapeType()) {
				case CIRCLE:
					gc.fillOval(x - r, y - r, 2 * r, 2 * r);
					break;
				case OVAL:
					gc.fillOval(x - w/2, y - h/2, w, h);
					break;
				case RECT:
					gc.fillRect(x - w/2, y - h/2, w, h);
					break;
				case ROUND_RECT:
					gc.fillRoundRect(x - w/2, y - h/2, w, h, aw, ah);
					break;
				case TEXT:
					gc.setFont(new Font("Courier", Shape.FONT_SIZE));
					double width = text.length() * Shape.CHAR_LENGTH;
					gc.fillText(text, x - width/2, y);
					break;
			}
		}
	}
	
	/**
	 * Shows the color dialog.
	 * 
	 * @param c The color
	 * @return The picked color.
	 */
	public Color showColorDialog(Color c) {
		FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("color.fxml"));
		Stage dialogStage = new Stage(StageStyle.DECORATED);
		Color result = null;
		try {
			Parent root = (Parent)fxmlLoader.load();
			Scene scene = new Scene(root, 200, 150);
			ColorDialogController controller = fxmlLoader.<ColorDialogController>getController();
			controller.setColor(c);
			scene.getStylesheets().add(getClass().getResource("pr1.css").toExternalForm());
			dialogStage.setTitle(COLOR_TITLE);
			dialogStage.setScene(scene);
			dialogStage.initModality(Modality.APPLICATION_MODAL);
			dialogStage.showAndWait();;
			result = controller.getColor();
		}
		catch (Exception e) {
			e.printStackTrace();
		}
		return result;
	}
	
}

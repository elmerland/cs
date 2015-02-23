package cs3744.hw2;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.charset.Charset;

import cs3744.hw2.HW2Model.Circle;
import javafx.beans.value.ObservableValue;
import javafx.collections.ListChangeListener;
import javafx.event.ActionEvent;
import javafx.scene.control.MenuItem;
import javafx.scene.input.MouseButton;
import javafx.scene.input.MouseEvent;
import javafx.scene.paint.Color;
import javafx.stage.FileChooser;

/**
 * This class represents the controller of the application. It handles
 * communication between the view and the model. It also handles events and
 * changes.
 * 
 * @author Elmer Landaverde
 *
 */
public class HW2Controller {

	HW2View					view;
	HW2Model				model;

	private boolean	isDragging			= false;

	// Starting coordinates for mouse drag
	private double	start_x					= 0.0;
	private double	start_y					= 0.0;

	private Circle	selectionRing		= null;	// Ring around currently selected
	// circle
	private Circle	selectedCircle	= null;	// Currently selected circle
	private Circle	clipboardCircle	= null;	// Circle copied by user

	/**
	 * Create new controller object.
	 * 
	 * @param v The view.
	 * @param m The model.
	 */
	public HW2Controller(HW2View v, HW2Model m) {
		view = v;
		model = m;

		// Redraw all circles on canvas size change
		view.addCanvasSizeListener((ObservableValue<? extends Number> ov,
				Number old_val, Number new_val) -> {
			redraw();
		});

		// Handle canvas click events
		view.getCanvas().addEventHandler(MouseEvent.MOUSE_CLICKED,
				(MouseEvent e) -> {
					handleMouseClick(e);
				});

		// Handle mouse press events
		view.getCanvas().addEventHandler(MouseEvent.MOUSE_PRESSED,
				(MouseEvent e) -> {
					handleMousePress(e);
				});

		// Handle mouse drag event
		view.getCanvas().addEventHandler(MouseEvent.MOUSE_DRAGGED,
				(MouseEvent e) -> {
					handleMouseDrag(e);
				});

		// Handle menu item event
		view.addMenuBarHandler((ActionEvent t) -> {
			handleMenuEvent(t);
		});

		// Listen to changes in the circle list
		model.dataProperty().addListener(
				(ListChangeListener.Change<? extends Circle> circles) -> {
					view.fileSave.setDisable(false);
					redraw();
				});

		// Listen to changes in the file
		view.fileProperty().addListener(
				(ObservableValue<? extends File> observable, File oldValue,
						File newValue) -> {
					handleFileChange(newValue);
				});

		// Listen to changes to the color on the currently selected circle
		view.colorProperty().addListener(
				(ObservableValue<? extends Color> observable, Color oldValue,
						Color newValue) -> {
					handleColorChange(newValue);
				});

	}

	/**
	 * Handle all menu item events.
	 */
	private void handleMenuEvent(ActionEvent t) {
		switch (((MenuItem) t.getSource()).getText()) {
			case "New": { // Create new file
				FileChooser fileChooser = new FileChooser();
				fileChooser.setTitle("Open Drawing File");
				fileChooser.setInitialFileName("hw2.txt");
				File f = fileChooser.showSaveDialog(view.stage);
				view.fileProperty().setValue(f);
				if (f != null) {
					view.fileNew.setDisable(true);
					view.fileOpen.setDisable(true);
					view.fileSave.setDisable(true);
					view.fileClose.setDisable(false);
				}
				break;
			}
			case "Open": { // Open existing file
				FileChooser fileChooser = new FileChooser();
				fileChooser.setTitle("Open Drawing File");
				File f = fileChooser.showOpenDialog(view.stage);
				view.fileProperty().setValue(f);
				if (f != null) {
					view.fileNew.setDisable(true);
					view.fileOpen.setDisable(true);
					view.fileSave.setDisable(true);
					view.fileClose.setDisable(false);
				}
				break;
			}
			case "Save": { // Save circles to file
				File f = view.fileProperty().getValue();
				Charset charset = Charset.forName("US-ASCII");
				try (BufferedWriter writer = Files.newBufferedWriter(f.toPath(),
						charset)) {
					for (Circle c : model.dataProperty()) {
						writer.write(c.toCSV());
					}
				} catch (IOException e) {
					System.err.format("Write IOException: %s%n", e);
				}
				view.fileSave.setDisable(true);
				break;
			}
			case "Close": { // Close current file
				model.dataProperty().clear();
				view.fileProperty().setValue(null);
				view.fileNew.setDisable(false);
				view.fileOpen.setDisable(false);
				view.fileClose.setDisable(true);
				view.fileSave.setDisable(true);
				view.editCopy.setDisable(true);
				view.editPaste.setDisable(true);
				view.editDelete.setDisable(true);
				setSelectedCircle(null);
				clipboardCircle = null;
				break;
			}
			case "Quit": { // Exit application
				System.exit(0);
				break;
			}
			case "Copy": { // Copy currently selected circle
				clipboardCircle = selectedCircle;
				view.editPaste.setDisable(false);
				break;
			}
			case "Paste": { // Paste circle on clip-board to center of canvas
				if (clipboardCircle == null) { return; }
				double x = view.getCanvas().getWidth() / 2;
				double y = view.getCanvas().getHeight() / 2;
				final Circle c = new Circle(x, y, clipboardCircle.radius,
						clipboardCircle.red, clipboardCircle.green, clipboardCircle.blue,
						false);
				setSelectedCircle(null);
				model.dataProperty().add(c);
				break;
			}
			case "Delete": { // Delete currently selected circle
				if (selectedCircle == null) {
					break;
				}
				model.dataProperty().remove(selectedCircle);
				setSelectedCircle(null);
				redraw();
				break;
			}
			case "About": { // Show about dialog
				view.showAboutDialog();
				break;
			}
		}
	}

	/**
	 * Upon file change if new file exists read all circles and draw them.
	 * 
	 * @param newFile The new file.
	 */
	private void handleFileChange(File newFile) {
		view.clear();
		// Check if file doesn't exist
		if (newFile == null || !newFile.exists()) { return; }
		// Read from file and create all circles
		Charset charset = Charset.forName("US-ASCII");
		try (BufferedReader reader = Files.newBufferedReader(newFile.toPath(),
				charset)) {
			String line = null;
			// Read each line in the file
			while ((line = reader.readLine()) != null) {
				// Parse line and add to circle list
				model.dataProperty().add(HW2Model.parseCircle(line));
			}
		} catch (IOException e) {
			System.err.format("Read IOException: %s%n", e);
		}
	}

	/**
	 * Upon color change, update the color of the currently selected circle.
	 * 
	 * @param newColor The new color.
	 */
	private void handleColorChange(Color newColor) {
		if (selectedCircle == null) { return; }
		final Circle c = new Circle(selectedCircle.x, selectedCircle.y,
				selectedCircle.radius, newColor.getRed(), newColor.getGreen(),
				newColor.getBlue(), false);
		replaceSelectedCircle(c);
	}

	/**
	 * Upon mouse press, update the selected circle if one was clicked on as well
	 * as updating the starting x and y-coordinates of the mouse.
	 * 
	 * @param e The mouse event.
	 */
	private void handleMousePress(MouseEvent e) {
		if (!hasOpenFile()) { return; }
		updateSelectedCircle(e);
		redraw();
		// Save starting location of mouse drag
		start_x = e.getX();
		start_y = e.getY();
	}

	/**
	 * Handle mouse click action. If mouse is on top of a circle that circle will
	 * be marked as selected. If the mouse is not on top of any circle, a new
	 * circle will be drawn. If the user double clicks on a circle then a dialog
	 * will open to choose the circle color.
	 * 
	 * @param e The mouse event.
	 */
	private void handleMouseClick(MouseEvent e) {
		if (!hasOpenFile()) { return; }
		// If user is dragging do nothing
		if (isDragging) {
			isDragging = false;
			return;
		}
		if (e.getButton() == MouseButton.PRIMARY && e.getClickCount() == 1
				&& getSelectedCircle(e) == null) {
			makeNewCircle(e);
		}
		if (e.getButton() == MouseButton.PRIMARY && e.getClickCount() == 2) {
			if (getSelectedCircle(e) == selectedCircle) {
				view.showColorDialog(selectedCircle.getColor());
			}
		}
	}

	/**
	 * Handle mouse drag action. If mouse is dragged with primary button and there
	 * is a circle selected
	 * 
	 * @param e The mouse event.
	 */
	private void handleMouseDrag(MouseEvent e) {
		if (!hasOpenFile()) { return; }
		isDragging = true;
		if (selectedCircle == null) { return; }
		// Get change in mouse position
		double delta_x = e.getX() - start_x;
		double delta_y = e.getY() - start_y;
		// Update starting location for next drag event
		start_x = e.getX();
		start_y = e.getY();
		// Change circle position
		if (e.getButton() == MouseButton.PRIMARY) {
			final Circle c = new Circle(selectedCircle.x + delta_x, selectedCircle.y
					+ delta_y, selectedCircle.radius, selectedCircle.red,
					selectedCircle.green, selectedCircle.blue, false);
			replaceSelectedCircle(c);
		} else { // Change circle size
			final Circle c = new Circle(selectedCircle.x, selectedCircle.y,
					selectedCircle.radius + delta_x, selectedCircle.red,
					selectedCircle.green, selectedCircle.blue, false);
			replaceSelectedCircle(c);
		}
	}

	/**
	 * Clears the canvas and re-draws every circle. If there is a currently
	 * selected circle, this method will also draw a ring around the curently
	 * selected circle.
	 */
	private void redraw() {
		view.clear();
		for (Circle c : model.dataProperty()) { // Draw all circles
			view.draw(c);
		}
		if (selectionRing != null) { // Draw ring around selected circle
			view.draw(selectionRing);
		}
	}

	/**
	 * Determines if there is a file open. Without an open file no circles can be
	 * created or modified.
	 * 
	 * @return True if there is a file open, otherwise returns false.
	 */
	private boolean hasOpenFile() {
		return view.fileProperty().getValue() != null;
	}

	/**
	 * Removed the currently selected circle from the circle list and replaces it
	 * with the given circle 'c'.
	 * 
	 * @param c The new currently selected circle.
	 */
	private void replaceSelectedCircle(Circle c) {
		model.dataProperty().remove(selectedCircle);
		setSelectedCircle(c);
		model.dataProperty().add(c);
	}

	/**
	 * Update currently selected circle based on the position of the mouse event.
	 * The first circle that overlaps with the mouse event will be set as the
	 * currently selected circle.
	 * 
	 * @param e The mouse event.
	 */
	private void updateSelectedCircle(MouseEvent e) {
		// Iterate through circles and find the one selected
		Circle c = getSelectedCircle(e);
		setSelectedCircle(c);
	}

	/**
	 * Returns the first circle that overlaps the position of the mouse event.
	 * 
	 * @param e The mouse event.
	 * @return The first circle that overlaps with the mouse event. Returns null
	 *         if no circle overlaps with mouse event.
	 */
	private Circle getSelectedCircle(MouseEvent e) {
		for (Circle c : model.dataProperty()) {
			if (getDistance(c.x, c.y, e.getX(), e.getY()) <= c.radius) { return c; }
		}
		return null;
	}

	/**
	 * Update the selected circle and create ring object for currently selected
	 * circle.
	 * 
	 * @param c The new currently selected circle.
	 */
	private void setSelectedCircle(Circle c) {
		if (c == null) {
			selectedCircle = null;
			selectionRing = null;
		} else {
			selectedCircle = c;
			selectionRing = HW2Model.makeRing(c);
		}
		view.editCopy.setDisable(c == null ? true : false);
		view.editDelete.setDisable(c == null ? true : false);
	}

	/**
	 * Get distance between the two points specified by '(x1, y1)' and '(x2, y2)'
	 * 
	 * @param x1 Coordinate.
	 * @param y1 Coordinate.
	 * @param x2 Coordinate.
	 * @param y2 Coordinate.
	 * @return
	 */
	private double getDistance(double x1, double y1, double x2, double y2) {
		return Math.abs(Math.sqrt(Math.pow((x2 - x1), 2) + Math.pow(y2 - y1, 2)));
	}

	/**
	 * Creates a new circle based on the mouse event position. The new circle will
	 * be black and have a radius of 50.
	 * 
	 * @param e The mouse event.
	 */
	private void makeNewCircle(MouseEvent e) {
		final Circle c = new Circle(e.getX(), e.getY(), 50.0, 0.0, 0.0, 0.0, false);
		model.dataProperty().add(c);
	}
}

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

public class HW2Controller {

	HW2View view;
	HW2Model model;
	
	private boolean isDragging = false;
	private double start_x = 0.0;
	private double start_y = 0.0;

	private Circle selectionRing = null;
	private Circle selectedCircle = null;
	private Circle clipboardCircle = null;

	public HW2Controller(HW2View v, HW2Model m) {
		view = v;
		model = m;
		addCanvasListeners();
		addMenuItemListeners();

		// Listen to changes in the circle list
		model.dataProperty().addListener(
				(ListChangeListener.Change<? extends Circle> circles) -> {
					redraw();
				});

		// Listen to changes in the file
		view.fileProperty().addListener(
				(ObservableValue<? extends File> observable, File oldValue,
						File newValue) -> {
					// If file is null then clear canvas
					if (newValue == null || !newValue.exists()) {
						view.clear();
						return;
					}
					Charset charset = Charset.forName("US-ASCII");
					try (BufferedReader reader = Files.newBufferedReader(
							newValue.toPath(), charset)) {
						String line = null;
						// Read each line in the file
						while ((line = reader.readLine()) != null) {
							// Parse line and add to circle list
							model.dataProperty()
									.add(HW2Model.parseCircle(line));
						}
					} catch (IOException e) {
						System.err.format("Read IOException: %s%n", e);
					}
				});
		
		view.colorProperty().addListener((ObservableValue<? extends Color> observable, Color oldValue,
						Color newValue) -> {
							if (selectedCircle != null) {
								final Circle c = new Circle(
										selectedCircle.x,
										selectedCircle.y,
										selectedCircle.radius,
										newValue.getRed(),
										newValue.getGreen(),
										newValue.getBlue(),
										false);
								replaceSelectedCircle(c);
							}
						});
	}
	
	private void addMenuItemListeners() {
		view.addMenuBarHandler((ActionEvent t) -> {
			switch (((MenuItem) t.getSource()).getText()) {
			case "New": {
				FileChooser fileChooser = new FileChooser();
				fileChooser.setTitle("Open Drawing File");
				fileChooser.setInitialFileName("hw2.txt");
				File f = fileChooser.showSaveDialog(view.stage);
				view.fileProperty().setValue(f);
				if (f != null) {
					view.fileNew.setDisable(true);
					view.fileOpen.setDisable(true);
					view.fileClose.setDisable(false);
					view.fileSave.setDisable(false);
				}
				break;
			}
			case "Open": {
				FileChooser fileChooser = new FileChooser();
				fileChooser.setTitle("Open Drawing File");
				File f = fileChooser.showOpenDialog(view.stage);
				view.fileProperty().setValue(f);
				if (f != null) {
					view.fileNew.setDisable(true);
					view.fileOpen.setDisable(true);
					view.fileClose.setDisable(false);
					view.fileSave.setDisable(false);
				}
				break;
			}
			case "Save": {
				saveFile();
				break;
			}
			case "Close": {
				view.fileProperty().setValue(null);
				view.fileNew.setDisable(false);
				view.fileOpen.setDisable(false);
				view.fileClose.setDisable(true);
				view.fileSave.setDisable(true);
			}
			case "Quit": {
				System.exit(0);
			}
			case "Copy": {
				clipboardCircle = selectedCircle;
				view.editPaste.setDisable(false);
				break;
			}
			case "Paste": {
				pasteCircle();
				break;
			}
			case "Delete": {
				if (selectedCircle == null) { break; }
				model.dataProperty().remove(selectedCircle);
				setSelectedCircle(null);
				redraw();
				break;
			}
			case "About": {
				view.showAbout();
				break;
			}
			}
		});
	}

	private void addCanvasListeners() {
		view.getCanvas()
				.widthProperty()
				.addListener(
						(ObservableValue<? extends Number> ov, Number old_val,
								Number new_val) -> {
							redraw();
						});

		view.getCanvas()
				.heightProperty()
				.addListener(
						(ObservableValue<? extends Number> ov, Number old_val,
								Number new_val) -> {
							redraw();
						});

		view.getCanvas().addEventHandler(
				MouseEvent.MOUSE_CLICKED,
				(MouseEvent e) -> {
					if (isDragging) { isDragging = false; return; }
					if (e.getButton() == MouseButton.PRIMARY
							&& e.getClickCount() == 1) {
						if (getSelectedCircle(e) == null && hasOpenFile()) {
							// Create new circle
							setSelectedCircle(null);
							makeNewCircle(e);
						} else if (getSelectedCircle(e) == selectedCircle) {
							// Un-select current circle
							setSelectedCircle(null);
							redraw();
							return;
						} else {
							// Select circle
							updateSelectedCircle(e);
							redraw();
						}
					}
					if (e.getClickCount() == 2) {
						if (getSelectedCircle(e) == selectedCircle) {
							updateSelectedCircle(e);
							redraw();
							view.showDialog();
						}
					}
				});
		
		view.getCanvas().addEventHandler(MouseEvent.MOUSE_PRESSED,
				(MouseEvent e) -> {
					start_x = e.getX();
					start_y = e.getY();
				});

		view.getCanvas().addEventHandler(MouseEvent.MOUSE_DRAGGED,
				(MouseEvent e) -> {
					isDragging = true;
					if (selectedCircle == null) { return; }
					double delta_x = e.getX() - start_x;
					double delta_y = e.getY() - start_y;
					start_x = e.getX();
					start_y = e.getY();
					if (e.getButton() == MouseButton.PRIMARY) {
						final Circle c = new Circle(
								selectedCircle.x + delta_x,
								selectedCircle.y + delta_y,
								selectedCircle.radius,
								selectedCircle.red,
								selectedCircle.green,
								selectedCircle.blue,
								false);
						replaceSelectedCircle(c);
					} else {
						final Circle c = new Circle(
								selectedCircle.x,
								selectedCircle.y,
								selectedCircle.radius + delta_x,
								selectedCircle.red,
								selectedCircle.green,
								selectedCircle.blue,
								false);
						replaceSelectedCircle(c);
					}
				});
	}

	private void redraw() {
		view.clear();
		for (Circle c : model.dataProperty()) {
			view.draw(c);
		}
		if (selectionRing != null) {
			view.draw(selectionRing);
		}
	}

	/**
	 * Determines if there is a file open. Without an open file no circles can
	 * be drawn.
	 * 
	 * @return
	 */
	private boolean hasOpenFile() {
		return view.fileProperty().getValue() != null;
	}
	
	private void replaceSelectedCircle(Circle c) {
		model.dataProperty().remove(selectedCircle);
		setSelectedCircle(c);
		model.dataProperty().add(c);
	}
	
	private void updateSelectedCircle(MouseEvent e) {
		// Iterate through circles and find the one selected
		Circle c = getSelectedCircle(e);
		setSelectedCircle(c);
	}

	private Circle getSelectedCircle(MouseEvent e) {
		for (Circle c : model.dataProperty()) {
			if (getDistance(c.x, c.y, e.getX(), e.getY()) <= c.radius) {
				return c;
			}
		}
		return null;
	}

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

	private double getDistance(double x1, double y1, double x2, double y2) {
		return Math
				.abs(Math.sqrt(Math.pow((x2 - x1), 2) + Math.pow(y2 - y1, 2)));
	}

	private void makeNewCircle(MouseEvent e) {
		final Circle c = new Circle(e.getX(), e.getY(), 50.0, 0.0, 0.0, 0.0,
				false);
		model.dataProperty().add(c);
	}
	
	private void pasteCircle() {
		if (clipboardCircle == null) { return; }
		double x = view.getCanvas().getWidth()/2;
		double y = view.getCanvas().getHeight()/2;
		final Circle c = new Circle(x, y, 
				clipboardCircle.radius, 
				clipboardCircle.red, 
				clipboardCircle.green, 
				clipboardCircle.blue,
				false);
		setSelectedCircle(null);
		model.dataProperty().add(c);
	}
	
	private void saveFile() {
		File f = view.fileProperty().getValue();
		Charset charset = Charset.forName("US-ASCII");
		try (BufferedWriter writer = Files.newBufferedWriter(f.toPath(), charset)) {
			for (Circle c : model.dataProperty()) {
				writer.write(c.toCSV());
			}
		} catch (IOException e) {
			System.err.format("Write IOException: %s%n", e);
		}
	}

}

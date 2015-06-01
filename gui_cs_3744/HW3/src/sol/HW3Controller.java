package sol;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.StandardOpenOption;

import javafx.beans.property.ObjectProperty;
import javafx.beans.property.SimpleObjectProperty;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.ListChangeListener;
import javafx.event.EventHandler;
import javafx.scene.input.MouseButton;
import javafx.scene.input.MouseEvent;

/**
 * Homework 2 controller class.
 * Coordinates model and view.
 * 
 * @author Denis Gracanin
 * @version 1
 */
public class HW3Controller {
	private HW3Model model = null; // The model object.
	private HW3View view = null; // The view object.
	private ObjectProperty<HW3Model.HW3Circle> selection = null; // An observable circle object. Changes trigger repaint.
	private HW3Model.HW3Circle clipboard = null; // The last copied object.
	private double mouseLastX = -1; // The x coordinate of the last reported mouse position;
	private double mouseLastY = -1; // The y coordinate of the last reported mouse position;
	private static final String CHOICE_TITLE = "File Save";
	private static final String CHOICE_MESSAGE = "File not saved. Proceed?";

	/**
	 * Creates an instance of <code>HW2Controller</code> class.
	 * 
	 * @param m The model object.
	 * @param v The view object.
	 */
	public HW3Controller(HW3Model m, HW3View v) {
		model = m; // Stores the model object.
		view = v; // Stores the view object.
		selection = new SimpleObjectProperty<HW3Model.HW3Circle>(); // Creates an observable circle object.
		setSelection(null);
		setClipboard(null);
		selection.addListener(new ChangeListener<HW3Model.HW3Circle>() { // 

			/**
			 * Handles the selection changes.
			 * Always repaints, regardless of the event object. Inefficient but works.
			 * 
			 * @param e The event object.
			 */
			@Override
			public void changed(ObservableValue<? extends HW3Model.HW3Circle> observable, HW3Model.HW3Circle oldValue, HW3Model.HW3Circle newValue) {
				repaint();
			}
		});
		model.drawDataProperty().addListener(new ListChangeListener<HW3Model.HW3Circle>() {

			/**
			 * Handles the model changes.
			 * Always repaints, regardless of the event object. Inefficient but works!
			 * 
			 * @param e The event object.
			 */
			@Override
			public void onChanged(ListChangeListener.Change<? extends HW3Model.HW3Circle> e) { repaint(); }
		});
		view.tableSelectionProperty().addListener(new ChangeListener<HW3Model.HW3Circle>() {

			/**
			 * Handles the selection changes.
			 * Always repaints, regardless of the event object. Inefficient but works.
			 * 
			 * @param e The event object.
			 */
			@Override
			public void changed(ObservableValue<? extends HW3Model.HW3Circle> observable, HW3Model.HW3Circle oldValue, HW3Model.HW3Circle newValue) {
				setSelection(newValue);
				repaint();
			}
		});
		view.viewStateProperty().addListener(new ChangeListener<HW3View.ViewState>() {

			/**
			 * Handles the view state changes (from File menu and window resizing).
			 * 
			 * @param event The event object.
			 */
			@Override
			public void changed(ObservableValue<? extends HW3View.ViewState> observable, HW3View.ViewState oldValue, HW3View.ViewState newValue) {
				if (!newValue.equals(oldValue)) {
					switch (newValue) {
					case CLOSED: // No file opened (when the application starts or when the current file is closed.
						view.setFileMenu(false, false, true, true, false); // Configures individual file menu items (enabled/disabled).
						break;
					case NEW: // A new file to be opened.
						if (view.getFile() != null) {
							if (view.getFile().exists()) {
								view.getFile().delete(); // Delete the file if it the file with that name already exists (overwrite).
							}
						}
					case OPEN: // An existing file opened.
						if (view.getFile() != null) {
							Charset charset = Charset.forName("US-ASCII");
							try  {
								view.getFile().createNewFile();
								BufferedReader reader = Files.newBufferedReader(view.getFile().toPath(), charset);
								String line = null;
								while ((line = reader.readLine()) != null) {
									try {
										model.add(line); // Read the file line by line and add the circle (line) to the model.
									}
									catch (NumberFormatException e) { } // Ignores an incorrectly formatted line.
									catch (ArrayIndexOutOfBoundsException e) { } // Ignores an incorrectly formatted line.
								}
								System.out.println("Opened");
								view.viewStateProperty().set(HW3View.ViewState.OPENED);
							}
							catch (IOException e) {
								System.out.println("Close");
								view.viewStateProperty().set(HW3View.ViewState.CLOSE);
							}
						}
						break;
					case OPENED:  // The file is opened.
						view.setFileMenu(true, true, false, true, false);  // Configures individual file menu items (enabled/disabled).
						break;
					case CLOSE: // The file has to be closed.
						view.setFile(null); // Clears the file.
						setSelection(null); // Clears the selection;
						setClipboard(null); // Clears the selection;
						model.clear(); // Clears the model.
						view.clear(); // Clears the view.
						view.viewStateProperty().set(HW3View.ViewState.CLOSED);
						break;				
					case MODIFIED: // The file has been modified, needs saving.
						view.setFileMenu(true, true, true, false, false);
						break;
					case SAVE: // Save the file.
						if (view.getFile() != null) {
							Charset charset = Charset.forName("US-ASCII");
							try {
								BufferedWriter writer = Files.newBufferedWriter(view.getFile().toPath(), charset, StandardOpenOption.WRITE);
								for (HW3Model.HW3Circle c : model.drawDataProperty()) {
									String line = c.getCenterX() + "," +  c.getCenterY() + "," + c.getRadius() + "," +  c.getColor().getRed() + "," + c.getColor().getGreen() + "," + c.getColor().getBlue() + "\n";
									writer.write(line);
								}
								writer.close();
								view.viewStateProperty().set(HW3View.ViewState.OPENED);
							}
							catch (IOException e) {
								view.showMessageDialog("File Error", e.toString());
							}
						}
						break;
					case QUIT: // Quit the application
						if (oldValue == HW3View.ViewState.MODIFIED && !view.showChoiceDialog(CHOICE_TITLE, CHOICE_MESSAGE)) {
							view.viewStateProperty().set(oldValue);
							break;
						}
						System.exit(0);
						break;
					case RESIZE: // Redraw the view when the application window resizes.
						repaint();
						view.viewStateProperty().set(oldValue);
						break;
					default:
						break;
					}
				}
			}
		});
		view.clipboardStateProperty().addListener(new ChangeListener<HW3View.ClipboardState>() {

			/**
			 * Handles the clipboard changes.
			 * 
			 * @param event The event object.
			 */
			@Override
			public void changed(ObservableValue<? extends HW3View.ClipboardState> observable, HW3View.ClipboardState oldValue, HW3View.ClipboardState newValue) {
				HW3Model.HW3Circle c = null;
				if (getSelection() != null) {
					switch (newValue) {
					case COPY: // Copy the selection to the clipboard. 
						setClipboard(getSelection());
						view.setEditMenu(false, false, false); // Enable all Edit menu items.			
						break;
					case PASTE: // Paste the clipboard to the view.
						c = model.add(mouseLastX, mouseLastY);
						c.setRadius(getClipboard().getRadius());
						c.setColor(getClipboard().getColor());
						view.viewStateProperty().set(HW3View.ViewState.MODIFIED);
						break;
					case DELETE: // delete the selection.
						c = getSelection();
						setSelection(null);
						model.remove(c);
						view.setEditMenu(true, true, true); // Disable all Edit menu items.		
						view.viewStateProperty().set(HW3View.ViewState.MODIFIED);
						break;
					case IDLE:
						break;
					default:
						break;
					}
					view.clipboardStateProperty().set(HW3View.ClipboardState.IDLE);
				}
			}

		});
		view.addMouseListener(new EventHandler<MouseEvent>() {

			/**
			 * Handles the mouse events.
			 * 
			 * @param event The event object.
			 */
			@Override
			public void handle(MouseEvent e) {
				mouseLastX = e.getX();
				mouseLastY = e.getY();
				if (view.viewStateProperty().get() == HW3View.ViewState.CLOSED) {
					return;
				}
				else if (e.getEventType() == MouseEvent.MOUSE_PRESSED){
					HW3Model.HW3Circle c = model.select(e.getX(), e.getY());
					if (e.getButton() == MouseButton.PRIMARY) {
						if (c != null) {
							model.setTop(c);
						}
						view.viewStateProperty().set(HW3View.ViewState.MODIFIED);
					}
					setSelection(c);
				}
				else if (e.getEventType() == MouseEvent.MOUSE_DRAGGED) {
					if (getSelection() != null) {
						if (e.getButton() == MouseButton.PRIMARY) {
							getSelection().setCenterX(e.getX());
							getSelection().setCenterY(e.getY());
							view.viewStateProperty().set(HW3View.ViewState.MODIFIED);
						}
						else if (e.getButton() == MouseButton.SECONDARY) {
							getSelection().setRadius(getSelection().getRadius() + 0.25 * (e.getX() - getSelection().getCenterX()));
							view.viewStateProperty().set(HW3View.ViewState.MODIFIED);
						}
					}
				}
				else if (e.getEventType() == MouseEvent.MOUSE_CLICKED) {
					if (e.getButton().equals(MouseButton.PRIMARY)){
						switch (e.getClickCount()) {
						case 1:
							if (getSelection() == null) {
								setSelection(model.add(e.getX(), e.getY()));
							}
							else {
								setSelection(model.select(e.getX(), e.getY()));								
							}
							view.viewStateProperty().set(HW3View.ViewState.MODIFIED);
							break;
						case 2:
							setSelection(model.select(e.getX(), e.getY()));
							getSelection().setColor(view.showColorDialog(getSelection().getColor()));
							view.viewStateProperty().set(HW3View.ViewState.MODIFIED);
							break;
						default:
							break;
						}
					}
				}
			}
		});
		view.setFileMenu(false, false, true, true, false);
		view.setEditMenu(true, true, true);
		view.setTableItems(model.drawDataProperty());
	}

	/**
	 * Gets the selected circle.
	 * 
	 * @return The selected circle. 
	 */
	private HW3Model.HW3Circle getSelection() { return selection.get(); }

	/**
	 * Selects a circle.
	 * 
	 * @param s The circle to be selected.
	 */
	private void setSelection(HW3Model.HW3Circle s) {
		selection.set(s);
		view.setTableSelection(getSelection());
		view.setEditMenu(getSelection() == null, true, getSelection() == null); // Enable the Edit menu items as needed.
	}

	/**
	 * Gets the clipboard's content.
	 * 
	 * @return The content.
	 */
	private HW3Model.HW3Circle getClipboard() { return clipboard; }

	/**
	 * Sets the clipboard's content.
	 * 
	 * @param c The content.
	 */
	private void setClipboard(HW3Model.HW3Circle c) { clipboard = c; }

	/**
	 * Repaints all the circles in the model.
	 * The selected circle (if any) has a red ring around it. 
	 */
	private void repaint() {
		view.clear();
		for (HW3Model.HW3Circle c : model.drawDataProperty()) {
			view.drawCircle(c.getCenterX(), c.getCenterY(), c.getRadius(), c.getColor(), false);
		}
		if (getSelection() != null) {
			view.drawCircle(getSelection().getCenterX(), getSelection().getCenterY(), getSelection().getRadius(), getSelection().getColor(), true);
		}
	}

}

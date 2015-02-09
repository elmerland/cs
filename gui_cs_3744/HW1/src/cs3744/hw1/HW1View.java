package cs3744.hw1;

import cs3744.hw1.HW1Controller.RGBColor;
import javafx.beans.value.ChangeListener;
import javafx.geometry.HPos;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.geometry.VPos;
import javafx.scene.control.Label;
import javafx.scene.control.Slider;
import javafx.scene.layout.ColumnConstraints;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.RowConstraints;

/**
 * Represents the view of the application. This view consists of a main color
 * label at the top of the pane. At the bottom there are three ColorSlider
 * elements. Each of which has two labels and a slider.
 * 
 * @author Elmer Landaverde
 *
 */
public class HW1View extends GridPane {

	// Main color label
	private Label colorLabel = null;

	// Three slider components. Each having a slider and two labels.
	private ColorSlider red = null;
	private ColorSlider green = null;
	private ColorSlider blue = null;

	/**
	 * Create a new view object.
	 */
	public HW1View() {
		// Initial settings for grid pane layout
		setAlignment(Pos.CENTER);
		setHgap(10);
		setVgap(10);
		setPadding(new Insets(10));

		// Column widths as a percentage of window width.
		// Column 1: 100%
		ColumnConstraints col1 = new ColumnConstraints();
		col1.setPercentWidth(100);
		getColumnConstraints().addAll(col1);

		// Row heights as a percentage of window height.
		// Row 1: 70%, row 2: 10%, row 3: 10%, row 4: 10%
		RowConstraints row1 = new RowConstraints();
		row1.setPercentHeight(70);
		RowConstraints row2 = new RowConstraints();
		row2.setPercentHeight(10);
		RowConstraints row3 = new RowConstraints();
		row3.setPercentHeight(10);
		RowConstraints row4 = new RowConstraints();
		row4.setPercentHeight(10);
		getRowConstraints().addAll(row1, row2, row3, row4);

		// Main color label
		colorLabel = new Label();
		colorLabel.setId("COLOR_LABEL");
		colorLabel.setStyle(new RGBColor(0, 0, 0).toRGBString());
		colorLabel.setMaxHeight(Double.MAX_VALUE);
		colorLabel.setMaxWidth(Double.MAX_VALUE);
		add(colorLabel, 0, 0);

		// Create slider components
		red = new ColorSlider("RED", "Red");
		green = new ColorSlider("GREEN", "Green");
		blue = new ColorSlider("BLUE", "Blue");
		add(red, 0, 1);
		add(green, 0, 2);
		add(blue, 0, 3);
	}

	/**
	 * Add an event handler to all sliders.
	 * 
	 * @param handler
	 *            The event handler.
	 */
	public void addColorChangeHandler(ChangeListener<? super Number> handler) {
		red.addEventHandler(handler);
		blue.addEventHandler(handler);
		green.addEventHandler(handler);
	}

	/**
	 * Update the color of the main color label.
	 * 
	 * @param c
	 *            The new color.
	 */
	public void updateColorLabel(RGBColor c) {
		colorLabel.setStyle(c.toRGBString());
	}

	/**
	 * Update the slider values.
	 * 
	 * @param c
	 *            The new color.
	 */
	public void updateColorValues(RGBColor c) {
		red.updateValueLabel(c.red);
		green.updateValueLabel(c.green);
		blue.updateValueLabel(c.blue);
	}

	/**
	 * Gets the current color values from all sliders.
	 * 
	 * @return The current color.
	 */
	public RGBColor getColorValues() {
		return new RGBColor(red.getSliderValue(), green.getSliderValue(),
				blue.getSliderValue());
	}

	/**
	 * This class encapsulates represents a slider component. It consists of a
	 * slider and two labels. This is to facilitate the creation of all elements
	 * and to abstract away some functionality.
	 */
	public class ColorSlider extends GridPane {

		private Slider slider = null;
		private Label name = null; // Holds the color component of the slider
		private Label value = null; // Holds the value of the slider

		/**
		 * Create a new color slider component.
		 * 
		 * @param id
		 *            The ID of the slider component.
		 * @param sliderName
		 *            The name to put on the slider component name label.
		 */
		public ColorSlider(String id, String sliderName) {
			setAlignment(Pos.BASELINE_CENTER);
			setHgap(10);
			setVgap(10);
			setPadding(new Insets(0));
			// setGridLinesVisible(true);
			this.setId(id);

			// Column 1: 25%, column 2: 50%, column 3: 25%
			ColumnConstraints col1 = new ColumnConstraints();
			col1.setPercentWidth(25);
			ColumnConstraints col2 = new ColumnConstraints();
			col2.setPercentWidth(50);
			ColumnConstraints col3 = new ColumnConstraints();
			col3.setPercentWidth(25);
			getColumnConstraints().addAll(col1, col2, col3);

			// Row 1: 100%
			RowConstraints row1 = new RowConstraints();
			row1.setPercentHeight(100);
			getRowConstraints().addAll(row1);

			// Name label
			name = new Label(sliderName);
			name.setId("NAME");
			setHalignment(name, HPos.CENTER);
			setValignment(name, VPos.CENTER);
			add(name, 0, 0);

			// Slider
			slider = new Slider(0, 100, 0);
			slider.setShowTickLabels(true);
			slider.setShowTickMarks(true);
			slider.setMajorTickUnit(50);
			slider.setMinorTickCount(5);
			slider.setId(id + "_SLIDER");
			setHalignment(slider, HPos.CENTER);
			setValignment(slider, VPos.CENTER);
			add(slider, 1, 0);

			// Value label
			value = new Label("0.00");
			value.setId("VALUE");
			value.setAlignment(Pos.CENTER);
			setHalignment(value, HPos.RIGHT);
			setValignment(value, VPos.CENTER);
			add(value, 2, 0);
		}

		/**
		 * Add an event handler to the slider node.
		 * 
		 * @param handler
		 *            The event handler.
		 */
		public void addEventHandler(ChangeListener<? super Number> handler) {
			slider.valueProperty().addListener(handler);
		}

		/**
		 * Update the value of the value label. The value will be trimmed to a
		 * precision of two decimal numbers.
		 * 
		 * @param new_val
		 *            The new value.
		 */
		public void updateValueLabel(double new_val) {
			value.setText(String.format("%.2f", new_val));
		}

		/**
		 * Get the value of the slider.
		 * 
		 * @return The value of the slider.
		 */
		public double getSliderValue() {
			return slider.getValue();
		}
	}
}

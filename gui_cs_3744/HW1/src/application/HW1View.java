package application;

import javafx.beans.value.ChangeListener;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.control.Label;
import javafx.scene.layout.ColumnConstraints;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.RowConstraints;

public class HW1View extends GridPane {

	// Main color label
	private Label colorLabel = null;
	
	// Three slider components. Each having a slider and two labels.
	private ColorSlider red = null;
	private ColorSlider green = null;
	private ColorSlider blue = null;

	public HW1View() {
		// Initial settings for grid pane layout
		setAlignment(Pos.CENTER);
		setHgap(0);
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
	 * Update the color of the color label.
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
}

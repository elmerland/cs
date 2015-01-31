package application;

import application.HW1Model.Color;
import javafx.beans.value.ChangeListener;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.control.Label;
import javafx.scene.layout.ColumnConstraints;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.RowConstraints;

public class HW1View extends GridPane {
	
	private Label colorLabel = null;
	private ColorSlider red = null;
	private ColorSlider green = null;
	private ColorSlider blue = null;
	
	public HW1View() {
		setAlignment(Pos.CENTER);
		setHgap(0);
		setVgap(10);
		setPadding(new Insets(10));
//		setGridLinesVisible(true);
		
		// Column 1: 100%
		ColumnConstraints col1 = new ColumnConstraints();
		col1.setPercentWidth(100);
		getColumnConstraints().addAll(col1);
		
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
		colorLabel.setMaxHeight(Double.MAX_VALUE);
		colorLabel.setMaxWidth(Double.MAX_VALUE);
		colorLabel.setStyle("-fx-background-color: rgb(0, 0, 0);");
		add(colorLabel, 0, 0);
		
		// Red slider
		red = new ColorSlider("RED", "Red");
		add(red, 0, 1);
		
		// Blue slider
		blue = new ColorSlider("BLUE", "Blue");
		add(blue, 0, 2);
		
		// Green slider
		green = new ColorSlider("GREEN", "Green");
		add(green, 0, 3);
	}
	
	public void addColorChangeHandler(
			Color c, ChangeListener<? super Number> handler) {
		switch (c) {
		case RED:
			red.addEventHandler(handler);
			break;
		case BLUE:
			blue.addEventHandler(handler);
			break;
		case GREEN:
			green.addEventHandler(handler);
			break;
		}
	}
	
	public void updateColorLabel(double r, double b, double g) {
		colorLabel.setStyle(
				"-fx-background-color: rgb("
				+ String.format("%.2f", r) + "%, "
				+ String.format("%.2f", b) + "%, "
				+ String.format("%.2f", g) + "%);");
	}
	
	public void updateSliderValue(Color c, double new_val) {
		switch (c) {
		case RED:
			red.updateValueLabel(new_val);
			break;
		case BLUE:
			blue.updateValueLabel(new_val);
			break;
		case GREEN:
			green.updateValueLabel(new_val);
			break;
		}
	}
}

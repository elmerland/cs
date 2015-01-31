package application;

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

public class ColorSlider extends GridPane {
	
	private Slider slider = null;
	private Label name = null;
	private Label value = null;
	
	public ColorSlider(String id, String sliderName) {
		setAlignment(Pos.BASELINE_CENTER);
		setHgap(0);
		setVgap(0);
		setPadding(new Insets(0, 0, 0, 0));
//		setGridLinesVisible(true);
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
		setHalignment(value, HPos.CENTER);
		setValignment(value, VPos.CENTER);
		add(value, 2, 0);
	}
	
	public void addEventHandler(ChangeListener<? super Number> handler) {
		slider.valueProperty().addListener(handler);
	}
	
	public void updateValueLabel(double new_val) {
		value.setText(String.format("%.2f", new_val));
	}
}

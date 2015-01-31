package application;

import javafx.beans.value.ObservableValue;
import application.HW1Model.Color;

public class HW1Controller {
	HW1Model model = null;
	HW1View view = null;
	
	public HW1Controller(HW1Model m, HW1View v) {
		model = m;
		view = v;
		view.addColorChangeHandler(
				Color.RED,
				(ObservableValue<? extends Number> ov, Number old_val, 
						Number new_val) -> {
					if (model.setColor(Color.RED, (Double)new_val)) {
						view.updateSliderValue(Color.RED, model.red());
						view.updateColorLabel(
								model.red(),
								model.blue(),
								model.green());
					}
				});
		view.addColorChangeHandler(
				Color.BLUE,
				(ObservableValue<? extends Number> ov, Number old_val, 
						Number new_val) -> {
					if (model.setColor(Color.BLUE, (Double)new_val)) {
						view.updateSliderValue(Color.BLUE, model.blue());
						view.updateColorLabel(
								model.red(),
								model.blue(),
								model.green());
					}
				});
		view.addColorChangeHandler(
				Color.GREEN,
				(ObservableValue<? extends Number> ov, Number old_val, 
						Number new_val) -> {
					if (model.setColor(Color.GREEN, (Double)new_val)) {
						view.updateSliderValue(Color.GREEN, model.green());
						view.updateColorLabel(
								model.red(),
								model.blue(),
								model.green());
					}
				});
	}

}

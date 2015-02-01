package application;

import javafx.beans.value.ObservableValue;

public class HW1Controller {
	HW1Model model = null;
	HW1View view = null;

	public HW1Controller(HW1Model m, HW1View v) {
		model = m;
		view = v;
		view.addColorChangeHandler((ObservableValue<? extends Number> ov,
				Number old_val, Number new_val) -> {
			// Update the model colors.
			if (model.updateColors(view.getColorValues())) {
				// Update the view color only if color was updated.
				RGBColor c = model.getColors();
				view.updateColorValues(c);
				view.updateColorLabel(c);
			}
		});
	}

}

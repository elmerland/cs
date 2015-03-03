package cs3744.hw1;

import javafx.beans.value.ObservableValue;

/**
 * The controller of the application. It handles events arising from the view
 * and passes the information back to the model. If there were changes to the
 * model, this class will then make the appropriate changes back to the view.
 * 
 * @author Elmer Landaverde
 *
 */
public class HW1Controller {

	HW1Model model = null;
	HW1View view = null;

	/**
	 * Creates a new controller object.
	 * 
	 * @param m
	 *            The model associated with this controller.
	 * @param v
	 *            The view associated with this controller.
	 */
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

	/**
	 * Represents a color consisting of three components: red, green, and blue.
	 * All fields of this class are final. Therefore once created the fields
	 * cannot be modified. This means that objects of this class are meant for a
	 * one time use.
	 * 
	 * @author Elmer Landaverde
	 *
	 */
	static class RGBColor {
		final double red;
		final double blue;
		final double green;

		/**
		 * Create a new RGBColor object.
		 * 
		 * @param r
		 *            The value for the red color component.
		 * @param g
		 *            The value for the green color component.
		 * @param b
		 *            The value for the blue color component.
		 */
		public RGBColor(double r, double g, double b) {
			red = r;
			green = g;
			blue = b;
		}

		/**
		 * Returns a string representation of this object. The string is
		 * formatted to accommodate the JFX CSS style.
		 * 
		 * @return The string representation of this color object.
		 */
		public String toRGBString() {
			StringBuilder str = new StringBuilder();
			str.append("-fx-background-color: rgb(")
					.append(String.format("%.2f", red)).append("%, ")
					.append(String.format("%.2f", green)).append("%, ")
					.append(String.format("%.2f", blue)).append("%);");
			return str.toString();
		}
	}
}

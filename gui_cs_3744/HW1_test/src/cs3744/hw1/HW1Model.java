package cs3744.hw1;

import cs3744.hw1.HW1Controller.RGBColor;

/**
 * The model of this application. This model holds the three components of a
 * color: red, green, and blue. Changes to these components will only take
 * effect if the new values are significantly different from the current values
 * as defined by 'delta'.
 * 
 * @author Elmer Landaverde
 */
public class HW1Model {

	/**
	 * Enumerator to represent the different color components.
	 */
	public enum Color {
		RED, BLUE, GREEN;
	}

	/**
	 * This value indicates how different two double values need to be to be
	 * considered significantly different.
	 */
	private double delta = 0.01;

	/**
	 * Values of the color components.
	 */
	private double red;
	private double green;
	private double blue;

	/**
	 * Create a new model object containing the red, blue and green components
	 * of a color.
	 */
	public HW1Model() {
		red = green = blue = 0.0;
	}

	/**
	 * If "new_val" is more than delta away from the current color value, this
	 * method will update the old color value to the new one, and return true.
	 * Otherwise it returns false.
	 * 
	 * @param c
	 *            The color to be changed.
	 * @param new_val
	 *            The new color value.
	 * @return True if the color value was updated, false if the new color value
	 *         is not different enough to change the old color value.
	 */
	private boolean setColor(Color c, double new_val) {
		switch (c) {
		case RED:
			if (isNotEqual(red, new_val)) {
				red = new_val;
				return true;
			}
		case GREEN:
			if (isNotEqual(green, new_val)) {
				green = new_val;
				return true;
			}
		case BLUE:
			if (isNotEqual(blue, new_val)) {
				blue = new_val;
				return true;
			}
		default:
			return false;
		}
	}

	/**
	 * Get the current values for all colors.
	 * 
	 * @return The color values.
	 */
	public RGBColor getColors() {
		return new RGBColor(red, green, blue);
	}

	/**
	 * Update the current color values. The color values will only be updated if
	 * they are significantly different from the current values. If at least one
	 * color was update this method will return true. Otherwise it returns
	 * false.
	 * 
	 * @param c
	 *            The new color values.
	 * @return True if at least one color value was updated.
	 */
	public boolean updateColors(RGBColor c) {
		boolean hasChanged = setColor(Color.RED, c.red);
		hasChanged = setColor(Color.GREEN, c.green) | hasChanged;
		hasChanged = setColor(Color.BLUE, c.blue) | hasChanged;
		return hasChanged;
	}

	/**
	 * Determines if the two double values have a difference greater than delta.
	 * 
	 * @param v1
	 *            A value to compare.
	 * @param v2
	 *            A value to compare.
	 * @return True if the two values are further away from each other than
	 *         delta.
	 */
	private boolean isNotEqual(double v1, double v2) {
		return Math.abs(v1 - v2) >= delta;
	}
}

package application;

public class HW1Model {

	public enum Color {
		RED, BLUE, GREEN;
	}

	private double red = 0.0;
	private double blue = 0.0;
	private double green = 0.0;
	private double delta = 0.01;

	public HW1Model() { /* Nothing to see here */ }

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
	public boolean setColor(Color c, double new_val) {
		switch (c) {
		case RED:
			if (Math.abs(red - new_val) >= delta) {
				red = new_val;
				return true;
			}
		case BLUE:
			if (Math.abs(blue - new_val) >= delta) {
				blue = new_val;
				return true;
			}
		case GREEN:
			if (Math.abs(green - new_val) >= delta) {
				green = new_val;
				return true;
			}
		default:
			return false;
		}
	}
	
	public double red() { return red; }
	public double blue() { return blue; }
	public double green() { return green; }
}

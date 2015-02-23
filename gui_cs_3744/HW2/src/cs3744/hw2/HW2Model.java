package cs3744.hw2;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.scene.paint.Color;

/**
 * This class represents the model for this application. It holds the data about
 * the state of the canvas.
 * 
 * @author Elmer Landaverde
 *
 */
public class HW2Model {

	private ObservableList<Circle> data;

	/**
	 * Create a new model object.
	 */
	public HW2Model() {
		data = FXCollections.observableArrayList();
	}

	/**
	 * Get the circle data list.
	 * 
	 * @return The circle data list.
	 */
	public ObservableList<Circle> dataProperty() {
		return data;
	}

	/**
	 * Parses a string representation of a circle into a circle object.
	 * 
	 * @param s
	 *            The string.
	 * @return A circle object that represents the string.
	 */
	static Circle parseCircle(String s) {
		String[] comps = s.split(",");
		final Circle circle = new Circle(Double.parseDouble(comps[0]),
				Double.parseDouble(comps[1]), Double.parseDouble(comps[2]),
				Double.parseDouble(comps[3]), Double.parseDouble(comps[4]),
				Double.parseDouble(comps[5]), false);
		return circle;
	}

	/**
	 * Make a ring that surrounds the given circle.
	 * 
	 * @param c
	 *            The circle to make the ring out of.
	 * @return A ring that surrounds the given circle.
	 */
	static Circle makeRing(Circle c) {
		final Circle circle = new Circle(c.x, c.y, c.radius + 5, 1.0, 0.0, 0.0,
				true);
		return circle;
	}

	/**
	 * This class represents a circle object.
	 * 
	 * @author Elmer Landaverde
	 *
	 */
	static class Circle {
		public double x;
		public double y;
		public double radius;
		public double red;
		public double green;
		public double blue;
		public boolean stroke;

		/**
		 * Create a new circle object.
		 * 
		 * @param x
		 *            The x-coordinate of the center point.
		 * @param y
		 *            The y-coordinate of the center point.
		 * @param radius
		 *            The radius of the circle.
		 * @param r
		 *            The red component of the circle color.
		 * @param g
		 *            The green component of the circle color.
		 * @param b
		 *            The blue component of the circle color.
		 * @param stroke
		 *            Set to true if this is a ring, and to false if this is a
		 *            circle.
		 */
		public Circle(double x, double y, double radius, double r, double g,
				double b, boolean stroke) {
			this.x = x;
			this.y = y;
			this.radius = radius;
			red = r;
			green = g;
			blue = b;
			this.stroke = stroke;
		}
		
		public Color getColor() {
			return new Color(red, green, blue, 1.0);
		}

		/**
		 * Get the top left x-coordinate of this circle.
		 * 
		 * @return The top left x-coordinate.
		 */
		public double getTopLeftX() {
			return x - radius;
		}

		/**
		 * Get the top left y-coordinate of this circle.
		 * 
		 * @return The top left y-coordinate.
		 */
		public double getTopLeftY() {
			return y - radius;
		}

		/**
		 * Get the length of sides of the circle if a square was laid over it.
		 * 
		 * @return The length of the sides of the circle.
		 */
		public double getSideLength() {
			return radius * 2;
		}

		@Override
		public String toString() {
			return String.format("%.2f, %.2f, %.2f, %.2f %.2f, %.2f %b", x, y,
					radius, red, green, blue, stroke);
		}

		/**
		 * Get a CSV representation of this circle.
		 * 
		 * @return A CSV representation of this circle.
		 */
		public String toCSV() {
			return String.format("%f, %f, %f, %f, %f, %f%n", x, y, radius, red,
					green, blue);
		}
	}
}

package cs3744.hw2;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;

public class HW2Model {
	
	private ObservableList<Circle> data;
	
	public HW2Model() {
		data = FXCollections.observableArrayList();
	}
	
	public ObservableList<Circle> dataProperty() { return data; }
	
	static Circle parseCircle(String s) {
		String[] comps = s.split(",");
		final Circle circle = new Circle(
				Double.parseDouble(comps[0]),
				Double.parseDouble(comps[1]),
				Double.parseDouble(comps[2]),
				Double.parseDouble(comps[3]),
				Double.parseDouble(comps[4]),
				Double.parseDouble(comps[5]), false);
		return circle;
	}
	
	static Circle makeRing(Circle c) {
		final Circle circle = new Circle(c.x, c.y, c.radius + 5, 1.0, 0.0, 0.0, true);
		return circle;
	}
	
	static class Circle {
		public double x;
		public double y;
		public double radius;
		public double red;
		public double green;
		public double blue;
		public boolean stroke;
		
		public Circle(double x, double y, double radius, double r, double g, double b, boolean stroke) {
			this.x = x;
			this.y = y;
			this.radius = radius;
			red = r;
			green = g;
			blue = b;
			this.stroke = stroke;
		}
		
		public double getTopLeftX() {
			return x - radius;
		}
		
		public double getTopLeftY() {
			return y - radius;
		}
		
		public double getSideLength() {
			return radius * 2;
		}
		
		@Override
		public String toString() {
			return String.format("%.2f, %.2f, %.2f, %.2f %.2f, %.2f %b", x, y, radius, red, green, blue, stroke);
		}
		
		public String toCSV() {
			return String.format("%f, %f, %f, %f, %f, %f%n", x, y, radius, red, green, blue); 
		}
	}
}

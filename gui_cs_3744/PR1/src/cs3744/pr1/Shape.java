package cs3744.pr1;

import javafx.beans.property.DoubleProperty;
import javafx.beans.property.ObjectProperty;
import javafx.beans.property.SimpleDoubleProperty;
import javafx.beans.property.SimpleObjectProperty;
import javafx.beans.property.SimpleStringProperty;
import javafx.beans.property.StringProperty;
import javafx.scene.paint.Color;

/**
 * This class represents any of the five type of shapes. It holds all their
 * properties and information.
 *
 */
public class Shape {
	public static enum ShapeType { CIRCLE, OVAL, RECT, ROUND_RECT, TEXT };
	
	private ObjectProperty<ShapeType> shapeType = null;
	private DoubleProperty centerX = null;
	private DoubleProperty centerY = null;
	private DoubleProperty radius = null;
	private DoubleProperty width = null;
	private DoubleProperty height = null;
	private DoubleProperty arcWidth = null;
	private DoubleProperty arcHeight = null;
	private ObjectProperty<Color> color = null;
	private StringProperty text = null;
	
	// Min, Max, and default for all shape properties
	public static final double RADIUS_MIN = 10;
	public static final double RADIUS_MAX = 100;
	public static final double RADIUS_DEFAULT = 50;
	
	public static final double X_MIN = 0;
	public static final double X_MAX = 1000;
	public static final double X_DEFAULT = 0;
	
	public static final double Y_MIN = 0;
	public static final double Y_MAX = 1000;
	public static final double Y_DEFAULT = 0;
	
	public static final double WIDTH_MIN = 5;
	public static final double WIDTH_MAX = 100;
	public static final double WIDTH_DEFAULT = 70;
	
	public static final double HEIGHT_MIN = 5;
	public static final double HEIGHT_MAX = 100;
	public static final double HEIGHT_DEFAULT = 50;
	
	public static final double ARC_WIDTH_MIN = 5;
	public static final double ARC_WIDTH_MAX = 100;
	public static final double ARC_WIDTH_DEFAULT = 20;
	
	public static final double ARC_HEIGHT_MIN = 5;
	public static final double ARC_HEIGHT_MAX = 100;
	public static final double ARC_HEIGHT_DEFAULT = 20;
	public static final double FONT_SIZE = 16;
	public static final double CHAR_LENGTH = 9.6;
	
	public Shape(ShapeType s, double x, double y) {
		initialize(s, x, y, RADIUS_DEFAULT, WIDTH_DEFAULT, HEIGHT_DEFAULT, 
				ARC_WIDTH_DEFAULT, ARC_HEIGHT_DEFAULT, 
				new Color(0, 0, 0, 1), "Hello World!");
	}

	public Shape(String[] t) throws NumberFormatException,
			ArrayIndexOutOfBoundsException {
		ShapeType s = null;
		double x = Double.parseDouble(t[1]);
		double y = Double.parseDouble(t[2]);
		double ra = 0, w = 0, h = 0, aw = 0, ah = 0;
		Color c = null;
		String label = null;
		int colorIndex = 0;
		switch(t[0].toLowerCase()) {
			case "circle":
				s = ShapeType.CIRCLE;
				ra = Double.parseDouble(t[3]);
				colorIndex = 4;
				break;
			case "oval":
				s = ShapeType.OVAL;
				w = Double.parseDouble(t[3]);
				h = Double.parseDouble(t[4]);
				colorIndex = 5;
				break;
			case "rect":
				s = ShapeType.RECT;
				w = Double.parseDouble(t[3]);
				h = Double.parseDouble(t[4]);
				colorIndex = 5;
				break;
			case "roundrect":
				s = ShapeType.ROUND_RECT;
				w = Double.parseDouble(t[3]);
				h = Double.parseDouble(t[4]);
				aw = Double.parseDouble(t[5]);
				ah = Double.parseDouble(t[6]);
				colorIndex = 7;
				break;
			case "text":
				s = ShapeType.TEXT;
				label = joinArray(t, 6);
				colorIndex = 3;
				break;
			default:
				System.exit(1);
		}
		double r = Double.parseDouble(t[colorIndex]);
		double g = Double.parseDouble(t[colorIndex + 1]);
		double b = Double.parseDouble(t[colorIndex + 2]);
		c = new Color(r, g, b, 1);
		
		initialize(s, x, y, ra, w, h, aw, ah, c, label);
	}

	public Shape(Shape s) {
		this(s.getShapeType(), s.getCenterX(), s.getCenterY(), s.getRadius(), 
				s.getWidth(), s.getHeight(), s.getArcWidth(), s.getArcHeight(), 
				s.getColor(), s.getText());
	}

	public Shape(ShapeType s, double x, double y, double r, double w, double h, double aw, double ah, Color c, String t) {
		initialize(s, x, y, r, w, h, aw, ah, c, t);
	}
	
	/**
	 * Initialize and set all shape properties.
	 * @param s Shape type.
	 * @param x Center x coordinate.
	 * @param y Center y coordinate.
	 * @param r Radius.
	 * @param w Width.
	 * @param h Height.
	 * @param aw Arc width.
	 * @param ah Arc height.
	 * @param c Color.
	 * @param t Text label.
	 */
	private void initialize(ShapeType s, double x, double y, double r, double w, double h, double aw, double ah, Color c, String t) {
		shapeType = new SimpleObjectProperty<ShapeType>();
		centerX = new SimpleDoubleProperty();
		centerY = new SimpleDoubleProperty();
		radius = new SimpleDoubleProperty();
		width = new SimpleDoubleProperty();
		height = new SimpleDoubleProperty();
		arcWidth = new SimpleDoubleProperty();
		arcHeight = new SimpleDoubleProperty();
		color = new SimpleObjectProperty<Color>();
		text = new SimpleStringProperty();
		setShape(s);
		setCenterX(x);
		setCenterY(y);
		setRadius(r);
		setWidth(w);
		setHeight(h);
		setArcWidth(aw);
		setArcHeight(ah);
		setColor(c);
		setText(t);
	}
	
	/**
	 * Gets the label for text elements.
	 * @param t The comma separated string array.
	 * @param startIndex The starting index for the label.
	 * @return A string with all the text for the label.
	 */
	private String joinArray(String[] t, int startIndex) {
		StringBuilder str = new StringBuilder();
		for (int i = startIndex; i < t.length; i++) {
			str.append(t[i]);
			// Add comma if not last element
			if (i != t.length - 1) { str.append(','); }
		}
		return str.toString();
	}
	
	// Object properties, getters, and setters for all the shape properties
	
	public ObjectProperty<ShapeType> shapeTypeProperty() { return shapeType; }
	public ShapeType getShapeType() {return shapeType.get(); }
	public String getShapeTypeString() {return shapeType.get().toString(); }
	public void setShape(ShapeType s) { shapeType.set(s); }
	public void setShape(String s) {
		ShapeType type = null;
		switch (s.toLowerCase()) {
			case "circle":
				type = ShapeType.CIRCLE;
				break;
			case "oval":
				type = ShapeType.OVAL;
				break;
			case "rect":
				type = ShapeType.RECT;
				break;
			case "round_rect":
				type = ShapeType.ROUND_RECT;
				break;
			case "text":
				type = ShapeType.TEXT;
				break;
		}
		shapeType.set(type);
	}

	public DoubleProperty centerXProperty() { return centerX; } 
	public double getCenterX() { return centerX.get(); }
	public void setCenterX(double x) { centerX.set(clamp(x, X_MIN, X_MAX)); }

	public DoubleProperty centerYProperty() { return centerY; }
	public double getCenterY() { return centerY.get(); }
	public void setCenterY(double y) { centerY.set(clamp(y, Y_MIN, Y_MAX)); }
	
	public ObjectProperty<Color> colorProperty() { return color; }
	public Color getColor() { return color.get(); }
	public void setColor(Color c) { color.set(c); }

	// Radius
	
	public DoubleProperty radiusProperty() { return radius; }
	
	public double getRadius() {
		switch(shapeType.get()) {
			case CIRCLE:
				return radius.get();
			default:
				return 0;
		}
	}
	
	public void setRadius(double r) {
		switch(shapeType.get()) {
			case CIRCLE:
				radius.set(clamp(r, RADIUS_MIN, RADIUS_MAX));
				break;
			default:
				break;
		}
	}
	
	// Width
	
	public DoubleProperty widthProperty() { return width; }
	
	public double getWidth() {
		switch(shapeType.get()) {
			case OVAL:
			case RECT:
			case ROUND_RECT:
				return width.get();
			default:
				return 0;
		}
	}
	
	public void setWidth(double w) {
		switch(shapeType.get()) {
			case OVAL:
			case RECT:
			case ROUND_RECT:
				width.set(clamp(w, WIDTH_MIN, WIDTH_MAX));
				break;
			default:
				break;
		}
	}
	
	// Height
	
	public DoubleProperty heightProperty() { return height; }
	
	public double getHeight() {
		switch(shapeType.get()) {
			case OVAL:
			case RECT:
			case ROUND_RECT:
				return height.get();
			default:
				return 0;
		}
	}
	
	public void setHeight(double w) {
		switch(shapeType.get()) {
			case OVAL:
			case RECT:
			case ROUND_RECT:
				height.set(clamp(w, HEIGHT_MIN, HEIGHT_MAX));
				break;
			default:
				break;
		}
	}
	
	// Arc Width
	
	public DoubleProperty arcWidthProperty() { return arcWidth; }
	
	public double getArcWidth() {
		switch(shapeType.get()) {
			case ROUND_RECT:
				return arcWidth.get();
			default:
				return 0;
		}
	}
	
	public void setArcWidth(double w) {
		switch(shapeType.get()) {
			case ROUND_RECT:
				arcWidth.set(clamp(w, ARC_WIDTH_MIN, ARC_WIDTH_MAX));
				break;
			default:
				break;
		}
	}
	
	// Arc Height
	
	public DoubleProperty arcHeightProperty() { return arcHeight; }
	
	public double getArcHeight() {
		switch(shapeType.get()) {
			case ROUND_RECT:
				return arcHeight.get();
			default:
				return 0;
		}
	}
	
	public void setArcHeight(double w) {
		switch(shapeType.get()) {
			case ROUND_RECT:
				arcHeight.set(clamp(w, ARC_HEIGHT_MIN, ARC_HEIGHT_MAX));
				break;
			default:
				break;
		}
	}
	
	// Text
	
	public StringProperty textProperty() { return text; }
	
	public String getText() {
		switch(shapeType.get()) {
			case TEXT:
				return text.get();
			default:
				return null;
		}
	}
	
	public void setText(String t) {
		switch(shapeType.get()) {
			case TEXT:
				text.set(t == null ? "" : t);
				break;
			default:
				break;
		}
	}

	/**
	 * Checks if the circle contains a point.
	 * 
	 * @param x The x-coordinate of the point.
	 * @param y The y-coordinate of the point.
	 * @return <code>true</code> if the point is contained, <code>false</code>
	 *         otherwise.
	 */
	public boolean contains(double x, double y) {
		boolean inside = false;
		switch(shapeType.get()) {
			case CIRCLE:
				inside = (Math.pow(x - centerX.get(), 2) + Math.pow(y - centerY.get(), 2)) <= Math.pow(radius.get(), 2);
				break;
			case OVAL:
			case RECT:
			case ROUND_RECT: {
				double x1 = centerX.get() - (width.get()/2);
				double x2 = centerX.get() + (width.get()/2);
				double y1 = centerY.get() - (height.get()/2);
				double y2 = centerY.get() + (height.get()/2);
				boolean insideX = (x >= x1) && (x <= x2);
				boolean insideY = (y >= y1) && (y <= y2);
				inside = insideX && insideY;
				break;
			}
			case TEXT: {
				double width = text.get().length() * CHAR_LENGTH;
				double height = FONT_SIZE;
				double x1 = centerX.get() - (width/2);
				double x2 = centerX.get() + (width/2);
				double y1 = centerY.get() - (height);
				double y2 = centerY.get();
				boolean insideX = (x >= x1) && (x <= x2);
				boolean insideY = (y >= y1) && (y <= y2);
				inside = insideX && insideY;
				break;
			}
			default:
				break;
		}
		return inside;
	}

	/**
	 * Clamps the value between <code>min</code> and <code>max</code>.
	 * 
	 * @param v The value.
	 * @param min The minimum value.
	 * @param max The maximum value.
	 * @return The clamped value.
	 */
	private double clamp(double v, double min, double max) {
		return (v < min ? min : (v > max ? max : v));
	}
	
	public String toString() {
		StringBuilder str = new StringBuilder();
		// Add shape name component
		switch(shapeType.get()) {
			case CIRCLE:
				str.append("circle");
				break;
			case OVAL:
				str.append("oval");
				break;
			case RECT:
				str.append("rect");
				break;
			case ROUND_RECT:
				str.append("roundrect");
				break;
			case TEXT:
				str.append("text");
				break;
		}
		// Add x and y components
		str.append(',');
		str.append(getCenterX());
		str.append(',');
		str.append(getCenterY());
		// Add shape specific components
		switch(shapeType.get()) {
			case CIRCLE:
				str.append(',');
				str.append(getRadius());
				break;
			case OVAL:
			case RECT:
				str.append(',');
				str.append(getWidth());
				str.append(',');
				str.append(getHeight());
				break;
			case ROUND_RECT:
				str.append(',');
				str.append(getWidth());
				str.append(',');
				str.append(getHeight());
				str.append(',');
				str.append(getArcWidth());
				str.append(',');
				str.append(getArcHeight());
				break;
			case TEXT:
				break;
		}
		// Add color components
		str.append(',');
		str.append(getColor().getRed());
		str.append(',');
		str.append(getColor().getGreen());
		str.append(',');
		str.append(getColor().getBlue());
		// Add text component if necessary
		if (getShapeType() == ShapeType.TEXT) {
			str.append(',');
			str.append(getText());
		}
		// Return shape string representation
		return str.toString();
	}
}

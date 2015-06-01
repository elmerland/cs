package cs3744.pr1;

import cs3744.pr1.Shape.ShapeType;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.scene.paint.Color;

/**
 * This class represents the data model of this applicaiton.
 *
 */
public class PR1Model {
	
	private ObservableList<Shape> drawData = null;
	private ChangeListener<Number> doubleListener = null;
	private ChangeListener<Color> colorListener = null;
	private ChangeListener<String> textListener = null;
	
	/**
	 * Creates an instance of <code>HW3Model</code> class with no data.
	 */
	public PR1Model() {
		drawData = FXCollections.observableArrayList();
		doubleListener = new ChangeListener<Number>(){
			@Override
			public void changed(ObservableValue<? extends Number> observable,
					Number oldValue, Number newValue) {
				touch();
			}
		};
		colorListener = new ChangeListener<Color>() {

			@Override
			public void changed(ObservableValue<? extends Color> observable,
					Color oldValue, Color newValue) {
				touch();
			}
		};
		textListener = new ChangeListener<String>() {

			@Override
			public void changed(ObservableValue<? extends String> observable,
					String oldValue, String newValue) {
				touch();
			}
		};
	}
	
	/**
	 * Generates a list event.
	 */
	private void touch() {
		if (drawData.size() > 0) {
			Shape s = drawData.get(0);
			drawData.remove(0);
			drawData.add(0, s);
		}
	}
	
	/**
	 * Gets the data property.
	 * 
	 * @return The observable list of shapes.
	 */
	public ObservableList<Shape> drawDataProperty() {
		return drawData;
	}

	/**
	 * Selects a circle that contains a point (<code>null</code> if no shape).
	 * 
	 * @param x The point's x-coordinate.
	 * @param y The point's y-coordinate.
	 * @return The shape.
	 */
	public Shape select(double x, double y) {
		Shape selection = null;
		for (Shape o : drawData) {
			if (o.contains(x, y)) {
				selection = o;
			}
		}
		return selection;
	}
	
	/**
	 * Sets the circle at the end of the list (the last shape drawn).
	 * 
	 * @param c The shape
	 */
	public void setTop(Shape c) {
		if (c != null) {
			int i = drawData.lastIndexOf(c);
			if (i != -1 && i != (drawData.size() - 1)) {
				drawData.remove(i);
				add(c);
			}
		}
	}

	/**
	 * Adds a shape to the model.
	 * 
	 * @param s The string (a comma-separated list of values representing a shape).
	 */
	public void add(String s) {
		add(new Shape(s.split("[ ]*,[ ]*")));
	}

	/**
	 * Adds a shape to the model.
	 * 
	 * @param type The shape's type.
	 * @param x The shape's x-coordinate.
	 * @param y The shape's y-coordinate.
	 * @return The shape.
	 */
	public Shape add(ShapeType type, double x, double y) {
		Shape c = new Shape(type, x, y);
		add(c);
		return c;
	}

	/**
	 * Adds a shape to the model.
	 * 
	 * @param c The shape
	 */
	public void add(Shape c) {
		c.centerXProperty().addListener(doubleListener);
		c.centerYProperty().addListener(doubleListener);
		c.radiusProperty().addListener(doubleListener);
		c.widthProperty().addListener(doubleListener);
		c.heightProperty().addListener(doubleListener);
		c.arcWidthProperty().addListener(doubleListener);
		c.arcHeightProperty().addListener(doubleListener);
		c.colorProperty().addListener(colorListener);
		c.textProperty().addListener(textListener);
		drawData.add(c);
	}

	/**
	 * Removes a shape from the model.
	 * 
	 * @param c The shape.
	 */
	public void remove(Shape c) {
		c.centerXProperty().removeListener(doubleListener);
		c.centerYProperty().removeListener(doubleListener);
		c.radiusProperty().removeListener(doubleListener);
		c.widthProperty().removeListener(doubleListener);
		c.heightProperty().removeListener(doubleListener);
		c.arcWidthProperty().removeListener(doubleListener);
		c.arcHeightProperty().removeListener(doubleListener);
		c.colorProperty().removeListener(colorListener);
		c.textProperty().removeListener(textListener);
		drawData.remove(c);
	}

	/**
	 * Clears the model (erases all data).
	 */
	public void clear() {
		drawData.clear();
	}

	/**
	 * Gets the index of the given shape in the draw list.
	 * 
	 * @param c The shape to find.
	 * @return The index of the given shape.
	 */
	public int getIndex(Shape c) {
		int index = 0;
		for (Shape o : drawData) {
			if (c == o) { return index; }
			index++;
		}
		return -1;
	}

}

package lecture7;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;

/**
 * Lecture 7: <code>BOTModel</code> class implements a model.
 * Each element is a string that contains a description of one <code>GraphicsContext</code> method and its arguments.
 * 
 * @author Denis Gracanin
 * @version 1
 */
public class BOTModel {
	private ObservableList<String> drawData;
	
	/**
	 * Creates a model object.
	 */
	public BOTModel() { 
		drawData = FXCollections.observableArrayList();
	}
	
	/**
	 * Gets the list of drawing data.
	 */
	public ObservableList<String> drawDataProperty() { return drawData; }

}
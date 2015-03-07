package cs3744.hw3;

import javafx.application.Application;
import javafx.stage.Stage;
import javafx.scene.Scene;

/**
 * Homework 3 main class.
 * 
 * Code was adapted from homework 2 solution.
 * 
 * @author Elmer Landaverde
 * @version 1
 */
public class HW3 extends Application {
	private final static String TITLE = "HW3: elmer";

	/**
	 * Creates the MVC components and shows the application window.
	 * 
	 * @param stage Top level container.
	 */
	@Override
	public void start(Stage stage) {
		HW3Model model = new HW3Model(); // The model object.
		HW3View view = new HW3View(stage); // The view object.
		@SuppressWarnings("unused")
		HW3Controller controller = new HW3Controller(model, view); // The controller object (knows about the model and the view objects). 
		Scene scene = new Scene(view, 800, 600); // The application window size is 800 by 600.
		try {
			scene.getStylesheets().add(getClass().getResource("hw3.css").toExternalForm());
		}
		catch (Exception e) {
			e.printStackTrace();
		}
		stage.setTitle(TITLE); // The application window title is <title>HW3: elmer</title>
		stage.setScene(scene);
		stage.show();
	}

	/**
	 * Invokes the program from the command line.
	 * 
	 * @param args Command line arguments.
	 */
	public static void main(String[] args) {
		launch(args);
	}

}

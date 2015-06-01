package sol;

import javafx.application.Application;
import javafx.stage.Stage;
import javafx.scene.Scene;

/**
 * Homework 3 main class.
 * 
 * @author Denis Gracanin
 * @version 1
 */
public class HW3 extends Application {
	private final static String TITLE = "HW3: gracanin";

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
		Scene scene = new Scene(view, 600, 600); // The application window size is 600 by 600.
		try {
			scene.getStylesheets().add(getClass().getResource("hw3.css").toExternalForm());
		}
		catch (Exception e) {
			e.printStackTrace();
		}
		stage.setTitle(TITLE); // The application window title is <title>HW3: gracanin</title>
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

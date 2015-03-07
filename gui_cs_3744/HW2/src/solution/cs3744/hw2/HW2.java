package solution.cs3744.hw2;

import javafx.application.Application;
import javafx.stage.Stage;
import javafx.scene.Scene;

/**
 * Homework 2 main class.
 * 
 * @author Denis Gracanin
 * @version 1
 */
public class HW2 extends Application {
	private final static String TITLE = "HW2: gracanin";

	/**
	 * Creates the MVC components and shows the application window.
	 * 
	 * @param stage Top level container.
	 */
	@Override
	public void start(Stage stage) {
		HW2Model model = new HW2Model(); // The model object.
		HW2View view = new HW2View(stage); // The view object.
		@SuppressWarnings("unused")
		HW2Controller controller = new HW2Controller(model, view); // The controller object (knows about the model and the view objects). 
		Scene scene = new Scene(view, 800, 600); // The application window size is 800 by 600.
		try {
			scene.getStylesheets().add(getClass().getResource("hw2.css").toExternalForm());
		}
		catch (Exception e) {
			e.printStackTrace();
		}
		stage.setTitle(TITLE); // The application window title is <title>HW2: gracanin</title>
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

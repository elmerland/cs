package cs3744.hw1;

import javafx.application.Application;
import javafx.stage.Stage;
import javafx.scene.Scene;

/**
 * The driver class for this application. It creates the model, view and
 * controller objects. It them binds them to the primary stage.
 * 
 * @author Elmer Landaverde
 *
 */
public class HW1 extends Application {

	/**
	 * Create the necessary model, view, controller objects and bind them to the
	 * primary stage object.
	 */
	@Override
	public void start(Stage primaryStage) {
		HW1Model model = new HW1Model();
		HW1View view = new HW1View();
		new HW1Controller(model, view);
		try {
			Scene scene = new Scene(view, 400, 500);
			scene.getStylesheets().add(
					getClass().getResource("hw1.css").toExternalForm());
			primaryStage.setTitle("HW1: elmer");
			primaryStage.setScene(scene);
			primaryStage.show();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * Main method to launch this application.
	 * 
	 * @param args
	 *            Arguments.
	 */
	public static void main(String[] args) {
		launch(args);
	}
}

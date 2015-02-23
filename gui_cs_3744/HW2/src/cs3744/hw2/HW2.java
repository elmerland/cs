package cs3744.hw2;

import javafx.application.Application;
import javafx.stage.Stage;
import javafx.scene.Scene;

/**
 * Main class for this application. Launches the model, view and controller. And
 * attaches view to the stage.
 * 
 * @author Elmer Landaverde
 *
 */
public class HW2 extends Application {
	@Override
	public void start(Stage primaryStage) {
		HW2Model model = new HW2Model();
		HW2View view = new HW2View(primaryStage);
		@SuppressWarnings("unused")
		HW2Controller controller = new HW2Controller(view, model);
		Scene scene = new Scene(view, 800, 600);
		try {
			scene.getStylesheets().add(
					getClass().getResource("hw2.css").toExternalForm());
		} catch (Exception e) {
			e.printStackTrace();
		}
		primaryStage.setTitle("HW2: elmer");
		primaryStage.setScene(scene);
		primaryStage.show();
	}

	public static void main(String[] args) {
		launch(args);
	}
}

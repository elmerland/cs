package cs3744.pr1;

import java.io.IOException;
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.stage.Stage;
import javafx.scene.Parent;
import javafx.scene.Scene;

/**
 * Project 1 main class.
 * 
 * Code was adapted from homework 2 solution.
 * 
 * @author Elmer Landaverde
 * @version 1
 */
public class PR1 extends Application {
	
	private Stage stage;
	private final static String TITLE = "PR1: elmer";

	/**
	 * Creates the MVC components and shows the application window.
	 * 
	 * @param stage Top level container.
	 * @throws IOException Could not load fxml file.
	 */
	@Override
	public void start(Stage primaryStage) throws IOException {
		stage = primaryStage;
		FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("PR1.fxml"));
		Parent root = (Parent)fxmlLoader.load();
		PR1Controller controller = fxmlLoader.<PR1Controller>getController();
		Scene scene = new Scene(root, 800, 600);
		try {
			scene.getStylesheets().add(getClass().getResource("pr1.css").toExternalForm());
		}
		catch (Exception e) {
			e.printStackTrace();
		}
		stage.setTitle(TITLE); // The application window title is <title>HW3: elmer</title>
		stage.setScene(scene);
		stage.show();
		controller.startApp();
	}
	
	public Stage getStage() { return stage; }

	/**
	 * Invokes the program from the command line.
	 * 
	 * @param args Command line arguments.
	 */
	public static void main(String[] args) {
		launch(args);
	}

}

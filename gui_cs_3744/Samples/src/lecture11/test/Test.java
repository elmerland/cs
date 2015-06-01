package lecture11.test;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;


public class Test extends Application {

	@Override
	public void start(Stage stage) throws Exception {
		Parent root = FXMLLoader.load(getClass().getResource("test.fxml"));

		Scene scene = new Scene(root, 200, 100);
		scene.getStylesheets().add(getClass().getResource("test.css").toExternalForm());

		stage.setTitle("Test");
		stage.setScene(scene);
		stage.show();
	}
	
	public static void main(String[] args) {
		launch(args);
	}

}

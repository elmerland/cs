package application;

import javafx.application.Application;
import javafx.stage.Stage;
import javafx.scene.Scene;

public class HW1 extends Application {
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

	public static void main(String[] args) {
		launch(args);
	}
}
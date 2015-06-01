package lecture11.borderpane;

import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.scene.layout.BorderPane;
import javafx.stage.Stage;


public class BorderPaneExample extends Application {
	@Override
	public void start(Stage primaryStage) {
		try {
			BorderPane border = new BorderPane();
			Label top = new Label("Page Title");
			border.setTop(top);
			Label center = new Label("Some data here");
			border.setCenter(center);
			Scene scene = new Scene(border, 300, 200);
			primaryStage.setScene(scene);
			primaryStage.setTitle("BorderPane Example");
			primaryStage.show();
		} catch(Exception e) {
			e.printStackTrace();
		}
	}
	
	public static void main(String[] args) {
		launch(args);
	}

}

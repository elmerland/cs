package listviewsample;

import javafx.application.Application;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.scene.Scene;
import javafx.scene.control.ListView;
import javafx.scene.layout.StackPane;
import javafx.stage.Stage;

public class ListViewBasic extends Application {
	private ObservableList<String> items = null;
	private ListView<String> list = null;

	@Override
	public void start(Stage primaryStage) {
		list = new ListView<String>();
		items = FXCollections.observableArrayList("One", "Two", "Three");
		list.setItems(items);
		list.setPrefWidth(200);
		list.setPrefHeight(70);

		StackPane root = new StackPane();
		root.getChildren().add(list);

		primaryStage.setScene(new Scene(root));
		primaryStage.setTitle("List View Basic");        
		primaryStage.show();
	}

	public static void main(String[] args) {
		launch(args);
	}

}
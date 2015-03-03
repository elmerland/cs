package listviewsample;

import javafx.application.Application;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.scene.Scene;
import javafx.scene.control.ListView;
import javafx.scene.control.cell.ComboBoxListCell;
import javafx.scene.layout.StackPane;
import javafx.stage.Stage;

public class ListViewCombo extends Application {
	private ObservableList<String> names = null;
	private  ObservableList<String> data = null;
	private ListView<String> list = null;

	@Override
	public void start(Stage primaryStage) {
		list = new ListView<String>(data);
		list.setPrefSize(200, 400);
		list.setEditable(true);

		names = FXCollections.observableArrayList();
		names.addAll(
				"Adam", "Alex", "Alfred", "Albert",
				"Brenda", "Connie", "Derek", "Donny", 
				"Lynne", "Myrtle", "Rose", "Rudolph", 
				"Tony", "Trudy", "Williams", "Zach"
				);

		data = FXCollections.observableArrayList();
		for (int i = 0; i < 18; i++) {
			data.add("anonym");
		}

		list.setItems(data);
		list.setCellFactory(ComboBoxListCell.forListView(names));              

		StackPane root = new StackPane();
		root.getChildren().add(list);

		primaryStage.setScene(new Scene(root));
		primaryStage.setTitle("List View Combo");        
		primaryStage.show();
	}

	public static void main(String[] args) {
		launch(args);
	}

}
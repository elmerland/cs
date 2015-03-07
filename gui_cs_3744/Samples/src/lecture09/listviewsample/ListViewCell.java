package lecture09.listviewsample;

import javafx.application.Application;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.scene.Scene;
import javafx.scene.control.ListCell;
import javafx.scene.control.ListView;
import javafx.scene.layout.Priority;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.stage.Stage;
    
public class ListViewCell extends Application {
    private ObservableList<String> data = null;
	private ListView<String> list = null;
 
    @Override
    public void start(Stage stage) {
    	list = new ListView<String>();
    	data = FXCollections.observableArrayList(
                "chocolate", "salmon", "gold", "coral", "darkorchid",
                "darkgoldenrod", "lightsalmon", "black", "rosybrown", "blue",
                "blueviolet", "brown");
    	 
        list.setItems(data);
        list.setCellFactory((ListView<String> l) -> new ColorRectCell());

        VBox box = new VBox();
        box.getChildren().addAll(list);
        VBox.setVgrow(list, Priority.ALWAYS);

        Scene scene = new Scene(box, 200, 200);
        stage.setScene(scene);
        stage.setTitle("List View Cell");
 
        stage.show();
    }
    
    private class ColorRectCell extends ListCell<String> {
        @Override
        public void updateItem(String item, boolean empty) {
            super.updateItem(item, empty);
            Rectangle rect = new Rectangle(100, 20);
            if (item != null) {
                rect.setFill(Color.web(item));
                setGraphic(rect);
            }
        }
    }
    
    public static void main(String[] args) {
        launch(args);
    }
}
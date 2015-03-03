package treeviewsample;

import javafx.application.Application;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.control.TreeItem;
import javafx.scene.control.TreeView;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.StackPane;
import javafx.stage.Stage;
 
public class TreeViewBasic extends Application {
    
    @Override
    public void start(Stage primaryStage) {        
    	Node rootIcon = new ImageView(new Image(getClass().getResourceAsStream("folder_16.png")));
        TreeItem<String> rootItem = new TreeItem<> ("Inbox", rootIcon);
        rootItem.setExpanded(true);
        for (int i = 1; i < 6; i++) {
            TreeItem<String> item = new TreeItem<> ("Message" + i);            
            rootItem.getChildren().add(item);
        }
        TreeView<String> tree = new TreeView<> (rootItem);

        StackPane root = new StackPane();
        root.getChildren().add(tree);

        primaryStage.setScene(new Scene(root, 300, 250));
        primaryStage.setTitle("Tree View Basic");        
        primaryStage.show();
    }
    
    public static void main(String[] args) {
        launch(args);
    }

}
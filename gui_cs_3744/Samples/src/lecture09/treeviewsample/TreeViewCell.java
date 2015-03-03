package treeviewsample;

import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.control.cell.CheckBoxTreeCell;
import javafx.scene.layout.StackPane;
import javafx.stage.Stage;

public class TreeViewCell extends Application {

	@Override
	public void start(Stage primaryStage) {
		CheckBoxTreeItem<String> rootItem = new CheckBoxTreeItem<>("View Source Files");
		rootItem.setExpanded(true);                  

		TreeView<String> tree = new TreeView<String>(rootItem);  
		tree.setEditable(true);

		tree.setCellFactory(CheckBoxTreeCell.<String>forTreeView());    
		for (int i = 0; i < 8; i++) {
			final CheckBoxTreeItem<String> checkBoxTreeItem = new CheckBoxTreeItem<>("Sample" + (i+1));
			rootItem.getChildren().add(checkBoxTreeItem);   
		}

		tree.setRoot(rootItem);
		tree.setShowRoot(true);

		StackPane root = new StackPane();
		root.getChildren().add(tree);

		primaryStage.setScene(new Scene(root, 300, 250));
		primaryStage.setTitle("Tree View Cell");        
		primaryStage.show();
	}

	public static void main(String[] args) {
		launch(args);
	}

}

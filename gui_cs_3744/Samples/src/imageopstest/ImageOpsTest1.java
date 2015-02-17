package imageopstest;
 
import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.layout.StackPane;
import javafx.stage.Stage;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
 
public class ImageOpsTest1 extends Application {
    
    @Override
    public void start(Stage primaryStage) {
     
        // Create Image and ImageView objects
        Image image = new Image("http://docs.oracle.com/javafx/"
        + "javafx/images/javafx-documentation.png");
        ImageView imageView = new ImageView();
        imageView.setImage(image);
      
        // Display image on screen
        StackPane root = new StackPane();
        root.getChildren().add(imageView);
        Scene scene = new Scene(root, 300, 250);
        primaryStage.setTitle("Image Read Test");
        primaryStage.setScene(scene);
        primaryStage.show();
    }
 
    public static void main(String[] args) {
        launch(args);
    }
}
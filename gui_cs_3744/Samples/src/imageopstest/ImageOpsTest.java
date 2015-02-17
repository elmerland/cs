package imageopstest;
 
import java.io.File;
import java.io.IOException;
import java.nio.ByteBuffer;
import javafx.application.Application;
import javafx.embed.swing.SwingFXUtils;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.effect.DropShadow;
import javafx.scene.image.PixelFormat;
import javafx.scene.image.PixelWriter;
import javafx.scene.image.WritableImage;
import javafx.scene.paint.Color;
import javafx.stage.Stage;
import javax.imageio.ImageIO;
 
public class ImageOpsTest extends Application {
 
    // Image Data
    private static final int IMAGE_WIDTH = 10;
    private static final int IMAGE_HEIGHT = 10;
    private byte imageData[] = new byte[IMAGE_WIDTH * IMAGE_HEIGHT * 3];
    // Drawing Surface (Canvas)
    private GraphicsContext gc;
    private Canvas canvas;
    private Group root;
 
    public static void main(String[] args) {
        launch(args);
    }
 
    @Override
    public void start(Stage primaryStage) {
        primaryStage.setTitle("PixelWriter Test");
        root = new Group();
        canvas = new Canvas(200, 200);
        canvas.setTranslateX(100);
        canvas.setTranslateY(100);
        gc = canvas.getGraphicsContext2D();
        createImageData();
        drawImageData();
 
        Scene scene = new Scene(root, 400, 400);
        primaryStage.setScene(scene);
        primaryStage.show();
 
        //Take snapshot of the scene
        WritableImage writableImage = scene.snapshot(null);
 
        // Write snapshot to file system as a .png image
        File outFile = new File("imageops-snapshot.png");
        try {
            ImageIO.write(SwingFXUtils.fromFXImage(writableImage, null),
                    "png", outFile);
        } catch (IOException ex) {
            System.out.println(ex.getMessage());
        }
    }
 
    private void createImageData() {
        int i = 0;
        for (int y = 0; y < IMAGE_HEIGHT; y++) {
            System.out.println("y: " + y);
            int r = y * 255 / IMAGE_HEIGHT;
            for (int x = 0; x < IMAGE_WIDTH; x++) {
                System.out.println("\tx: " + x);
                int g = x * 255 / IMAGE_WIDTH;
                imageData[i] = (byte) r;
                imageData[i + 1] = (byte) g;
                System.out.println("\t\tR: " + (byte) r);
                System.out.println("\t\tG: " + (byte) g);
                i += 3;
            }
        }
        System.out.println("imageData.lengthdrawImageData: " + imageData.length);
    }
 
    private void drawImageData() {
        boolean on = true;
        PixelWriter pixelWriter = gc.getPixelWriter();
        PixelFormat<ByteBuffer> pixelFormat = PixelFormat.getByteRgbInstance();
        for (int y = 50; y < 150; y += IMAGE_HEIGHT) {
            for (int x = 50; x < 150; x += IMAGE_WIDTH) {
                if (on) {
                    pixelWriter.setPixels(x, y, IMAGE_WIDTH,
                    IMAGE_HEIGHT, pixelFormat, 
                    imageData, 0, IMAGE_WIDTH * 3);
                }
                on = !on;
            }
            on = !on;
        }
 
        // Add drop shadow effect
        gc.applyEffect(new DropShadow(20, 20, 20, Color.GRAY));
        root.getChildren().add(canvas);
    }
}
package lecture07;

import javafx.application.Application;
import javafx.scene.Scene;
import javafx.stage.Stage;

/**
 * Lecture 7: <code>BOT</code> class provides an MVC and menu based version of <code>BasicOpsTest</code>.
 * 
 * @author Denis Gracanin
 * @version 1
 */
public class BOT extends Application {

	/**
	 * Invoked when the application starts.
	 * Creates the GUI.
	 * 
	 * @param stage The default application window.
	 */
    @Override
    public void start(Stage stage) {
    	BOTView view = new BOTView(stage);
    	@SuppressWarnings("unused")
		BOTController controller = new BOTController(new BOTModel(), view);
    	Scene scene = new Scene(view, 300, 300);
		try {
			scene.getStylesheets().add(getClass().getResource("bot.css").toExternalForm());
		} catch(Exception e) {
			e.printStackTrace();
		}
        stage.setScene(scene);
        stage.setTitle("Drawing Operations Test: MVC and Menu Support");
        stage.show();
    }
    
	/**
	 * Starts the application when invoked from the command line or from Eclipse IDE.
	 * 
	 * @param args The command line arguments.
	 */
    public static void main(String[] args) {
        launch(args);
    }
    
}
package lecture04;

import javafx.application.Application;
import javafx.stage.Stage;
import javafx.scene.Scene;

/**
 * Lecture 4: <code>DataEntry</code> class provides a data entry application.
 * 
 * @author Denis Gracanin
 * @version 1
 *
 */
public class DataEntry extends Application {

	/**
	 * Invoked when the application starts.
	 * Creates the GUI.
	 * 
	 * @param primaryStage The default application window.
	 */
	@Override
	public void start(Stage primaryStage) {

		DataEntryModel model = new DataEntryModel();
		DataEntryView view = new DataEntryView();
		@SuppressWarnings("unused")
		DataEntryController controller = new DataEntryController(model, view); 


		Scene scene = new Scene(view,600,400);
		try {
			scene.getStylesheets().add(getClass().getResource("dataentry.css").toExternalForm());
		} catch(Exception e) {
			e.printStackTrace();
		}
		primaryStage.setTitle("Data Entry");
		primaryStage.setScene(scene);
		primaryStage.show();
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
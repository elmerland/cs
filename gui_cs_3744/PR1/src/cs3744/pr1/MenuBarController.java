package cs3744.pr1;

import java.io.File;

import cs3744.pr1.PropertiesController.ClipboardState;
import cs3744.pr1.PropertiesController.ViewState;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.MenuItem;
import javafx.stage.FileChooser;
import javafx.stage.Modality;
import javafx.stage.Stage;
import javafx.stage.StageStyle;

/**
 * This class controls the logic of all the menu items and their actions.
 * 
 * @author elmer
 *
 */
public class MenuBarController {
	
	private MenuItem fileNew;
  private MenuItem fileOpen;
  private MenuItem fileClose;
  private MenuItem fileSave;
  private MenuItem fileQuit;
  private MenuItem editCopy;
  private MenuItem editPaste;
  private MenuItem editDelete;
  private MenuItem helpAbout;
  
  private PropertiesController props;
  
	private static final String ABOUT_TITLE = "About Project 1";
	private static final String ABOUT_MESSAGE = "Project 1 Version 1";
	
	public MenuBarController(MenuItem newfile, MenuItem open, MenuItem close, 
			MenuItem save, MenuItem quit, MenuItem copy, MenuItem paste, 
			MenuItem delete, MenuItem about, PropertiesController properties) {
		this.props = properties;
		this.props.menubar = this;
		fileNew = newfile;
		fileOpen = open;
		fileClose = close;
		fileSave = save;
		fileQuit = quit;
		editCopy = copy;
		editPaste = paste;
		editDelete = delete;
		helpAbout = about;
		
		bindObjects();
		setFileMenu(false, false, true, true, false);
		setEditMenu(true, true, true);
	}
	
	private void bindObjects() {
		fileNew.setOnAction((e) -> {
			FileChooser fileChooser = new FileChooser();
			fileChooser.setTitle("Open New File");
			File f = fileChooser.showSaveDialog(props.stage);
			if (f != null) {
				props.file = f;
				props.viewState.set(ViewState.NEW);
			}
		});
		
		fileOpen.setOnAction((e) -> {
			FileChooser fileChooser = new FileChooser();
			fileChooser.setTitle("Open File");
			File f = fileChooser.showOpenDialog(props.stage);
			if (f != null) {
				props.file = f;
				props.viewState.set(ViewState.OPEN);
			}
		});
		
		fileClose.setOnAction((e) -> props.viewState.set(ViewState.CLOSE));
		fileSave.setOnAction((e) -> props.viewState.set(ViewState.SAVE));
		fileQuit.setOnAction((e) -> props.viewState.set(ViewState.QUIT));
		
		editCopy.setOnAction((e) -> props.clipboardState.set(ClipboardState.COPY));
		editPaste.setOnAction((e) -> props.clipboardState.set(ClipboardState.PASTE));
		editDelete.setOnAction((e) -> props.clipboardState.set(ClipboardState.DELETE));
		
		helpAbout.setOnAction((e) -> showMessageDialog(ABOUT_TITLE, ABOUT_MESSAGE));
	}
	
	/**
	 * Enables/disables the File menu items.
	 * 
	 * @param n The File menu New menu item disabled.
	 * @param o The File menu Open menu item disabled.
	 * @param c The File menu Close menu item disabled.
	 * @param s The File menu Save menu item disabled.
	 * @param q The File menu Quit menu item disabled.
	 */
	public void setFileMenu(boolean n, boolean o, boolean c, boolean s, boolean q) {
		fileNew.setDisable(n);
		fileOpen.setDisable(o);
		fileClose.setDisable(c);
		fileSave.setDisable(s);
		fileQuit.setDisable(q);
	}

	/**
	 * Enables/disables the Edit menu items.
	 * 
	 * @param c The Edit menu Copy menu item disabled.
	 * @param p The Edit menu Paste menu item disabled.
	 * @param d The Edit menu Delete menu item disabled.
	 */
	public void setEditMenu(boolean c, boolean p, boolean d) {
		editCopy.setDisable(c);
		editPaste.setDisable(p);
		editDelete.setDisable(d);
	}
	
	/**
	 * Shows the message dialog.
	 * 
	 * @param title The dialog title.
	 */
	public void showMessageDialog(String title, String message) {
		FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("message.fxml"));
		
		Stage dialogStage = new Stage(StageStyle.DECORATED);
		try {
			Parent root = (Parent)fxmlLoader.load();
			Scene scene = new Scene(root, 200, 150);
			MessageDialogController controller = fxmlLoader.<MessageDialogController>getController();
			controller.setText(message);
			scene.getStylesheets().add(getClass().getResource("pr1.css").toExternalForm());
			dialogStage.setTitle(title);
			dialogStage.setScene(scene);
			dialogStage.initModality(Modality.APPLICATION_MODAL);
			dialogStage.show();
		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * Shows the choice dialog.
	 * 
	 * @param title The title.
	 * @param message The message.
	 * @return The choice.
	 */
	public boolean showChoiceDialog(String title, String message) {
		FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("choice.fxml"));
		Stage dialogStage = new Stage(StageStyle.DECORATED);
		boolean result = false;
		try {
			Parent root = (Parent)fxmlLoader.load();
			Scene scene = new Scene(root, 200, 150);
			ChoiceDialogController controller = fxmlLoader.<ChoiceDialogController>getController();
			controller.setText(message);
			scene.getStylesheets().add(getClass().getResource("pr1.css").toExternalForm());
			dialogStage.setTitle(title);
			dialogStage.setScene(scene);
			dialogStage.initModality(Modality.APPLICATION_MODAL);
			dialogStage.showAndWait();;
			result = controller.getResult();
		}
		catch (Exception e) {
			e.printStackTrace();
		}
		return result;
	}

}

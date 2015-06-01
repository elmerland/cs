package cs3744.pr1;

import java.net.URL;
import java.util.ResourceBundle;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.TextArea;
import javafx.stage.Stage;

public class MessageDialogController {

    @FXML
    private ResourceBundle resources;

    @FXML
    private URL location;

    @FXML
    private TextArea textArea;

    @FXML
    private Button okButton;
    
    @FXML
    void handleButtonClick(ActionEvent event) {
    	Stage stage = (Stage)okButton.getScene().getWindow();
    	stage.close();
    }

    @FXML
    void initialize() {
      assert textArea != null : "fx:id=\"textArea\" was not injected: check your FXML file 'about.fxml'.";
      assert okButton != null : "fx:id=\"okButton\" was not injected: check your FXML file 'about.fxml'.";
      
    }
    
    // Set the text area message.
    public void setText(String msg) {
    	textArea.setText(msg);
    }
}

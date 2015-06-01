package cs3744.pr1;

import java.net.URL;
import java.util.ResourceBundle;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.TextArea;
import javafx.stage.Stage;

public class ChoiceDialogController {

    @FXML
    private ResourceBundle resources;

    @FXML
    private URL location;

    @FXML
    private TextArea textArea;

    @FXML
    private Button okButton;

    @FXML
    private Button cancelButton;

    @FXML
    void handleCancelButton(ActionEvent event) {
    	result = false;
    	((Stage) cancelButton.getScene().getWindow()).close();
    }

    @FXML
    void handleOkButton(ActionEvent event) {
    	result = true;
    	((Stage) cancelButton.getScene().getWindow()).close();
    }

    @FXML
    void initialize() {
        assert textArea != null : "fx:id=\"textArea\" was not injected: check your FXML file 'choice.fxml'.";
        assert okButton != null : "fx:id=\"okButton\" was not injected: check your FXML file 'choice.fxml'.";
        assert cancelButton != null : "fx:id=\"cancelButton\" was not injected: check your FXML file 'choice.fxml'.";

    }
    
    private boolean result = false;
    
    // Get the result from user choice.
    public boolean getResult() { return result; }
    
    // Set the text area message.
    public void setText(String msg) {
    	textArea.setText(msg);
    }
}

package cs3744.pr1;

import java.net.URL;
import java.util.ResourceBundle;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.ColorPicker;
import javafx.scene.paint.Color;
import javafx.stage.Stage;

public class ColorDialogController {

    @FXML
    private ResourceBundle resources;

    @FXML
    private URL location;

    @FXML
    private ColorPicker color;

    @FXML
    private Button okButton;

    @FXML
    void handleOkButtonClick(ActionEvent event) {
    	c = color.getValue();
    	((Stage) okButton.getScene().getWindow()).close();
    }

    @FXML
    void initialize() {
        assert color != null : "fx:id=\"color\" was not injected: check your FXML file 'color.fxml'.";
        assert okButton != null : "fx:id=\"okButton\" was not injected: check your FXML file 'color.fxml'.";

    }
    
    private Color c = null;
    
    public Color getColor() { return c; }
    public void setColor(Color c) { this.c = c; color.setValue(c); }
}

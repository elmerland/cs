/**
 * Sample Skeleton for 'test.fxml' Controller Class
 */

package lecture11.test;

import java.net.URL;
import java.util.ResourceBundle;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;

public class TestController {

	@FXML // ResourceBundle that was given to the FXMLLoader
    private ResourceBundle resources;

	@FXML // URL location of the FXML file that was given to the FXMLLoader
    private URL location;

    @FXML
    void test(ActionEvent event) {
    	System.out.println("test");
    }

    @FXML // This method is called by the FXMLLoader when initialization is complete
    void initialize() {
    	System.out.println("initialize");
    }
}

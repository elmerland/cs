package lecture07;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.nio.charset.Charset;
import java.nio.file.Files;

import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.ListChangeListener;


/**
 * Lecture 7: <code>BOTController</code> class implements a controller.
 * 
 * @author Denis Gracanin
 * @version 1
 */
public class BOTController {
	private BOTModel model = null;
	private BOTView view = null;

	/**
	 * Creates a controller object.
	 * 
	 * @param m The model object.
	 * @param v The view object.
	 */
	public BOTController(BOTModel m, BOTView v) {
		model = m;
		view = v;
		model.drawDataProperty().addListener(new ListChangeListener<String>() {

			/**
			 * Handles the model data changes.
			 * Regardless of the change, clears the view and redraws the data from the model.
			 * 
			 * @param event The event object.
			 */
			@Override
			public void onChanged(ListChangeListener.Change<? extends String> c) {
				view.clear();
				for (String s : c.getList()) {
					view.draw(s);
				}
			}
		});
		view.fileProperty().addListener(new ChangeListener<File>() {

			/**
			 * Handles the file changes.
			 * If file is <code>null</code>, clears the model.
			 * Otherwise, read the file and populates the model.
			 * 
			 * @param event The event object.
			 */
			@Override
			public void changed(ObservableValue<? extends File> observable, File oldValue, File newValue) {
				if (newValue == null) {
					model.drawDataProperty().clear();
				}
				else {
					Charset charset = Charset.forName("US-ASCII");
					try (BufferedReader reader = Files.newBufferedReader(newValue.toPath(), charset)) {
						String line = null;
						while ((line = reader.readLine()) != null) {
							model.drawDataProperty().add(line);
						}
					} catch (IOException e) {
						System.err.format("IOException: %s%n", e);
					}
				}
			}
		});
	}

}

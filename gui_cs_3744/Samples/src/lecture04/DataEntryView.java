package lecture04;

import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.control.Button;
import javafx.scene.control.CheckBox;
import javafx.scene.control.Label;
import javafx.scene.control.RadioButton;
import javafx.scene.control.Slider;
import javafx.scene.control.TextArea;
import javafx.scene.control.TextField;
import javafx.scene.control.ToggleGroup;
import javafx.scene.layout.ColumnConstraints;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.RowConstraints;

/**
 * Lecture 4: <code>DataEntryView</code> class implements a view.
 * 
 * @author Denis Gracanin
 * @version 1
 */
public class DataEntryView extends GridPane {
	private TextField firstName = null;
	private TextField lastName = null;
	private RadioButton male = null;
	private RadioButton female = null;
	private CheckBox java = null;
	private CheckBox cpp = null;
	private CheckBox javascript = null;
	private Slider gpa = null;
	private TextArea comment = null;
	private Button update = null;
	private Button revert = null;
	private Button clear = null;

	/**
	 * Creates a view object.
	 */
	public DataEntryView() {
		// Create the root pane and sets the default alignment and spacing.
		setAlignment(Pos.BASELINE_LEFT);
		setHgap(10);
		setVgap(10);
		setPadding(new Insets(25, 25, 25, 25));

		ColumnConstraints column1 = new ColumnConstraints();
		column1.setPercentWidth(25);
		ColumnConstraints column2 = new ColumnConstraints();
		column2.setPercentWidth(75);
		getColumnConstraints().addAll(column1, column2);

		RowConstraints row1 = new RowConstraints();
		row1.setPercentHeight(10);
		RowConstraints row2 = new RowConstraints();
		row2.setPercentHeight(10);
		RowConstraints row3 = new RowConstraints();
		row3.setPercentHeight(10);
		RowConstraints row4 = new RowConstraints();
		row4.setPercentHeight(10);
		RowConstraints row5 = new RowConstraints();
		row5.setPercentHeight(10);
		RowConstraints row6 = new RowConstraints();
		row6.setPercentHeight(40);
		RowConstraints row7 = new RowConstraints();
		row7.setPercentHeight(10);
		getRowConstraints().addAll(row1, row2, row3, row4, row5, row6, row7);

		// Add the labels for the attribute names
		add(new Label("First name"), 0, 0);
		add(new Label("Last name"), 0, 1);
		add(new Label("Gender"), 0, 2);
		add(new Label("Programming Skills"), 0, 3);
		add(new Label("GPA"), 0, 4);
		add(new Label("Comment"), 0, 5);

		// Add the text field for the first name attribute.
		firstName = new TextField();
		add(firstName, 1, 0);

		// Add the text field for the last name attribute.
		lastName = new TextField();
		add(lastName, 1, 1);

		// Add the radio buttons for the gender attribute.
		ToggleGroup gender = new ToggleGroup();
		male = new RadioButton("Male");
		male.setToggleGroup(gender);
		female = new RadioButton("Female");
		female.setToggleGroup(gender);
		HBox hbRBtn = new HBox(10);
		hbRBtn.setAlignment(Pos.BOTTOM_LEFT);
		hbRBtn.getChildren().add(male);
		hbRBtn.getChildren().add(female);
		add(hbRBtn, 1, 2);

		// Add the check boxes for the programming skills attribute.
		java = new CheckBox("Java");
		cpp = new CheckBox("C++");
		javascript = new CheckBox("JavaScript");
		HBox hbCBtn = new HBox(10);
		hbCBtn.setAlignment(Pos.BOTTOM_LEFT);
		hbCBtn.getChildren().add(java);
		hbCBtn.getChildren().add(cpp);
		hbCBtn.getChildren().add(javascript);
		add(hbCBtn, 1, 3);

		// Add the slider for the GPA attribute.
		gpa = new Slider(1, 4, 4);
		gpa.setShowTickLabels(true);
		gpa.setShowTickMarks(true);
		gpa.setMajorTickUnit(1);
		gpa.setMinorTickCount(10);
		add(gpa, 1, 4);

		// Add the text area for the comment attribute.
		comment = new TextArea();
		add(comment, 1, 5);

		// add the buttons.
		update = new Button("Update");
		update.setId("UPDATE");
		revert = new Button("Revert");
		revert.setId("REVERT");
		clear = new Button("Clear");
		clear.setId("CLEAR");
		HBox hbBtn = new HBox(10);
		hbBtn.setAlignment(Pos.BOTTOM_RIGHT);
		hbBtn.getChildren().add(update);
		hbBtn.getChildren().add(revert);
		hbBtn.getChildren().add(clear);
		add(hbBtn, 1, 6);
	}

	/**
	 * Sets the view attributes to default values.
	 */
	public void clear() {
		setFirstName("");
		setLastName("");
		setGender(0);
		setProgrammingSkills(0);
		setGpa(4);
		setComment("");
	}

	/**
	 * Adds the event handler for button action events.
	 * 
	 * @param h The event handler object.
	 */
	public void addMyHandler(EventHandler<ActionEvent> h) {
		update.setOnAction(h);
		revert.setOnAction(h);
		clear.setOnAction(h);
	}

	/**
	 * Gets the value of the first name attribute.
	 * 
	 * @return The first name.
	 */
	public String getFirstName() { return firstName.getText(); }

	/**
	 * Sets the value of the first name attribute.
	 * 
	 * @param f The first name.
	 */
	public void setFirstName(String f) { firstName.setText(f);	}

	/**
	 * Gets the value of the last name attribute.
	 * 
	 * @return The last name.
	 */
	public String getLastName() { return lastName.getText(); }

	/**
	 * Sets the value of the last name attribute.
	 * 
	 * @param l The last name.
	 */
	public void setLastName(String l) { lastName.setText(l); }

	/**
	 * Gets the value of the gender attribute.
	 * 0 - not set.
	 * 1 - male.
	 * 2 - female.
	 * 
	 * @return The gender.
	 */
	public int getGender() {
		int g = 0;
		if (male.isSelected()) {
			g = 1;
		}
		else if (female.isSelected()) {
			g = 2;
		}
		return g;
	}

	/**
	 * Sets the value of the gender attribute.
	 * 0 - not set.
	 * 1 - male.
	 * 2 - female.
	 * 
	 * @param g The gender.
	 */
	public void setGender(int g) {
		male.setSelected(g == 1);
		female.setSelected(g == 2);
	}

	/**
	 * Gets the value of the programming skills attribute.
	 * bit 0 - JavaScript.
	 * bit 1 - C++.
	 * bit 2 - Java.
	 * 
	 * @return The programming skills.
	 */
	public int getProgrammingSkills() {
		int p = 0;
		if (java.isSelected()) {
			p |= 0x4;
		}
		if (cpp.isSelected()) {
			p |= 0x2;
		}
		if (javascript.isSelected()) {
			p |= 0x1;
		}
		return p;
	}

	/**
	 * Sets the value of the programming skills attribute.
	 * bit 0 - JavaScript.
	 * bit 1 - C++.
	 * bit 2 - Java.
	 * 
	 * @param p The programming skills.
	 */
	public void setProgrammingSkills(int p) {
		java.setSelected((p & 0x4) > 0);
		cpp.setSelected((p & 0x2) > 0);
		javascript.setSelected((p & 0x1) > 0);
	}

	/**
	 * Gets the value of the GPA attribute.
	 * 
	 * @return The GPA.
	 */
	public double getGpa() { return gpa.getValue(); }

	/**
	 * Sets the value of the GPA attribute.
	 * 
	 * @param g The GPA.
	 */
	public void setGpa(double g) { gpa.setValue(g); }

	/**
	 * Gets the value of the comment attribute.
	 * 
	 * @return The comment.
	 */
	public String getComment() { return comment.getText(); }

	/**
	 * Sets the value of the comment attribute.
	 * 
	 * @param c The comment.
	 */
	public void setComment(String c) { comment.setText(c); }

}

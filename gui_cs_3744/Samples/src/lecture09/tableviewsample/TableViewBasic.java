package lecture09.tableviewsample;

import javafx.application.Application;
import javafx.beans.property.SimpleStringProperty;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.geometry.Insets;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.layout.VBox;
import javafx.scene.text.Font;
import javafx.stage.Stage;

public class TableViewBasic extends Application {
	private ObservableList<Person> data = null;
	private TableView<Person> table = null;

	@SuppressWarnings("unchecked")
	@Override
	public void start(Stage stage) {
		table = new TableView<Person>();

		TableColumn<Person, String> firstNameCol = new TableColumn<Person, String>("First Name");
		firstNameCol.setMinWidth(100);
		firstNameCol.setCellValueFactory(new PropertyValueFactory<Person, String>("firstName"));

		TableColumn<Person, String> lastNameCol = new TableColumn<Person, String>("Last Name");
		lastNameCol.setMinWidth(100);
		lastNameCol.setCellValueFactory(new PropertyValueFactory<Person, String>("lastName"));

		TableColumn<Person, String> emailCol = new TableColumn<Person, String>("Email");
		emailCol.setMinWidth(200);
		emailCol.setCellValueFactory(new PropertyValueFactory<Person, String>("email"));

		data = FXCollections.observableArrayList(
				new Person("Jacob", "Smith", "jacob.smith@example.com"),
				new Person("Isabella", "Johnson", "isabella.johnson@example.com"),
				new Person("Ethan", "Williams", "ethan.williams@example.com"),
				new Person("Emma", "Jones", "emma.jones@example.com"),
				new Person("Michael", "Brown", "michael.brown@example.com")
		);
		table.setItems(data);
		
		table.getColumns().addAll(firstNameCol, lastNameCol, emailCol);

		Label label = new Label("Address Book");
		label.setFont(new Font("Arial", 20));

		VBox vbox = new VBox();
		vbox.setSpacing(5);
		vbox.setPadding(new Insets(10, 0, 0, 10));
		vbox.getChildren().addAll(label, table);

		Scene scene = new Scene(new Group());
		((Group) scene.getRoot()).getChildren().addAll(vbox);

		stage.setScene(scene);
		stage.setTitle("Table View Basic");
		stage.setWidth(450);
		stage.setHeight(500);
		stage.show();
	}

	public static void main(String[] args) {
		launch(args);
	}

	public static class Person {
		private final SimpleStringProperty firstName;
		private final SimpleStringProperty lastName;
		private final SimpleStringProperty email;

		private Person(String fName, String lName, String email) {
			this.firstName = new SimpleStringProperty(fName);
			this.lastName = new SimpleStringProperty(lName);
			this.email = new SimpleStringProperty(email);
		}

		public String getFirstName() {
			return firstName.get();
		}

		public void setFirstName(String fName) {
			firstName.set(fName);
		}

		public String getLastName() {
			return lastName.get();
		}

		public void setLastName(String fName) {
			lastName.set(fName);
		}

		public String getEmail() {
			return email.get();
		}

		public void setEmail(String fName) {
			email.set(fName);
		}
	}
} 
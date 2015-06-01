package cs3744.pr1;

import javafx.beans.value.ObservableValue;
import javafx.event.ActionEvent;
import javafx.scene.control.ColorPicker;
import javafx.scene.control.ContentDisplay;
import javafx.scene.control.TableCell;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.TableColumn.CellEditEvent;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.control.cell.TextFieldTableCell;
import javafx.scene.paint.Color;
import javafx.util.Callback;
import javafx.util.converter.DefaultStringConverter;
import javafx.util.converter.DoubleStringConverter;

/**
 * This class controls the table view. It creates the custom table cells and 
 * overall table functioning.
 *
 */
public class TableController {
	
	private PropertiesController props = null;
	
	public TableController(TableView<Shape> table, 
			TableColumn<Shape, String> columnShape, 
			TableColumn<Shape, Double> columnCenterX, 
			TableColumn<Shape, Double> columnCenterY, 
			TableColumn<Shape, Double> columnRadius, 
			TableColumn<Shape, Double> columnWidth, 
			TableColumn<Shape, Double> columnHeight, 
			TableColumn<Shape, Double> columnArcWidth, 
			TableColumn<Shape, Double> columnArcHeight, 
			TableColumn<Shape, Color> columnColor, 
			TableColumn<Shape, String> columnText, 
			PropertiesController properties) {
		this.props = properties;
		props.table = table;
		
		columnShape.setCellValueFactory(new PropertyValueFactory<>("shapeTypeString"));
		columnShape.setCellFactory(TextFieldTableCell
				.<Shape, String> forTableColumn(new DefaultStringConverter()));
		columnShape.setOnEditCommit((CellEditEvent<Shape, String> t) -> {
			Shape c = (Shape) t.getTableView().getItems()
					.get(t.getTablePosition().getRow());
			c.setShape(t.getNewValue());
		});
		
		columnCenterX.setCellValueFactory(new PropertyValueFactory<>("centerX"));
		columnCenterX.setCellFactory(TextFieldTableCell
				.<Shape, Double> forTableColumn(new DoubleStringConverter()));
		columnCenterX.setOnEditCommit((CellEditEvent<Shape, Double> t) -> {
			Shape c = (Shape) t.getTableView().getItems()
					.get(t.getTablePosition().getRow());
			c.setCenterX(t.getNewValue());
		});

		columnCenterY.setCellValueFactory(new PropertyValueFactory<>("centerY"));
		columnCenterY.setCellFactory(TextFieldTableCell
				.<Shape, Double> forTableColumn(new DoubleStringConverter()));
		columnCenterY.setOnEditCommit((CellEditEvent<Shape, Double> t) -> {
			Shape c = (Shape) t.getTableView().getItems()
					.get(t.getTablePosition().getRow());
			c.setCenterY(t.getNewValue());
		});

		columnRadius.setCellValueFactory(new PropertyValueFactory<>("radius"));
		columnRadius.setCellFactory(TextFieldTableCell
				.<Shape, Double> forTableColumn(new DoubleStringConverter()));
		columnRadius.setOnEditCommit((CellEditEvent<Shape, Double> t) -> {
			Shape c = (Shape) t.getTableView().getItems()
					.get(t.getTablePosition().getRow());
			c.setRadius(t.getNewValue());
		});
		
		columnWidth.setCellValueFactory(new PropertyValueFactory<>("width"));
		columnWidth.setCellFactory(TextFieldTableCell
				.<Shape, Double> forTableColumn(new DoubleStringConverter()));
		columnWidth.setOnEditCommit((CellEditEvent<Shape, Double> t) -> {
			Shape c = (Shape) t.getTableView().getItems()
					.get(t.getTablePosition().getRow());
			c.setWidth(t.getNewValue());
		});
		
		columnHeight.setCellValueFactory(new PropertyValueFactory<>("height"));
		columnHeight.setCellFactory(TextFieldTableCell
				.<Shape, Double> forTableColumn(new DoubleStringConverter()));
		columnHeight.setOnEditCommit((CellEditEvent<Shape, Double> t) -> {
			Shape c = (Shape) t.getTableView().getItems()
					.get(t.getTablePosition().getRow());
			c.setHeight(t.getNewValue());
		});
		
		columnArcWidth.setCellValueFactory(new PropertyValueFactory<>("arcWidth"));
		columnArcWidth.setCellFactory(TextFieldTableCell
				.<Shape, Double> forTableColumn(new DoubleStringConverter()));
		columnArcWidth.setOnEditCommit((CellEditEvent<Shape, Double> t) -> {
			Shape c = (Shape) t.getTableView().getItems()
					.get(t.getTablePosition().getRow());
			c.setArcWidth(t.getNewValue());
		});
		
		columnArcHeight.setCellValueFactory(new PropertyValueFactory<>("arcHeight"));
		columnArcHeight.setCellFactory(TextFieldTableCell
				.<Shape, Double> forTableColumn(new DoubleStringConverter()));
		columnArcHeight.setOnEditCommit((CellEditEvent<Shape, Double> t) -> {
			Shape c = (Shape) t.getTableView().getItems()
					.get(t.getTablePosition().getRow());
			c.setArcHeight(t.getNewValue());
		});

		columnColor.setCellValueFactory(new PropertyValueFactory<>("color"));
		columnColor
				.setCellFactory(new Callback<TableColumn<Shape, Color>, TableCell<Shape, Color>>() {

					@Override
					public TableCell<Shape, Color> call(
							TableColumn<Shape, Color> param) {
						return new ColorPickerTableCell();
					}
				});
		columnColor.setOnEditCommit((CellEditEvent<Shape, Color> t) -> {
			Shape c = (Shape) t.getTableView().getItems()
					.get(t.getTablePosition().getRow());
			c.setColor(t.getNewValue());
		});
		
		columnText.setCellValueFactory(new PropertyValueFactory<>("text"));
		columnText.setCellFactory(TextFieldTableCell
				.<Shape, String> forTableColumn(new DefaultStringConverter()));
		columnText.setOnEditCommit((CellEditEvent<Shape, String> t) -> {
			Shape c = (Shape) t.getTableView().getItems()
					.get(t.getTablePosition().getRow());
			c.setText(t.getNewValue());
		});
		
		props.table.setItems(props.model.drawDataProperty());
		props.table
		.getSelectionModel()
		.selectedItemProperty()
		.addListener(
				(ObservableValue<? extends Shape> observable,
						Shape oldValue, Shape newValue) -> {
					if (newValue != null) {
						props.canvas.setSelection(newValue);
					}
				});
	}
	
	/**
	 * Custom color picker table cell.
	 * 
	 * @author Elmer Landaverde
	 *
	 */
	public class ColorPickerTableCell extends TableCell<Shape, Color> {
		private ColorPicker cp;

		public ColorPickerTableCell() {
			cp = new ColorPicker();
			cp.setOnAction((ActionEvent e) -> {
				commitEdit(cp.getValue());
				updateItem(cp.getValue(), isEmpty());
				Shape c = (Shape) getTableView().getItems().get(
						getTableRow().getIndex());
				c.setColor(cp.getValue());
			});
			setGraphic(cp);
			setContentDisplay(ContentDisplay.GRAPHIC_ONLY);
			setEditable(true);
		}

		protected void updateItem(Color item, boolean empty) {
			super.updateItem(item, empty);
			cp.setVisible(!empty);
			this.setItem(item);
			cp.setValue(item);
		}
	}

}

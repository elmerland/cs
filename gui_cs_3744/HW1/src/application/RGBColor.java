package application;

public class RGBColor {
	final double red;
	final double blue;
	final double green;

	public RGBColor(double r, double g, double b) {
		red = r;
		green = g;
		blue = b;
	}

	public String toRGBString() {
		StringBuilder str = new StringBuilder();
		str.append("-fx-background-color: rgb(")
				.append(String.format("%.2f", red)).append("%, ")
				.append(String.format("%.2f", green)).append("%, ")
				.append(String.format("%.2f", blue)).append("%);");
		return str.toString();
	}
}

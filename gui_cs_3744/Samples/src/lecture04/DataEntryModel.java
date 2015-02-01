package lecture04;

/**
 * Lecture 4: <code>DataEntryModel</code> class implements a model.
 * 
 * @author Denis Gracanin
 * @version 1
 */
public class DataEntryModel {
	private String firstName;
	private String lastName;
	private int gender;
	private int programmingSkills;
	private double gpa;
	private String comment;

	/**
	 * Creates a model object.
	 */
	public DataEntryModel() {
		clear();
	}

	/**
	 * Sets the model attributes to default values.
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
	 * Converts the model object to a string.
	 * 
	 * @return The string representation of the model object.
	 */
	public String toString() {
		return "First name: " + firstName + "\n"
				+ "Last name: " + lastName + "\n"
				+ "Gender: " + (gender == 1 ? "Male" : "") + (gender == 2 ? "Female" : "") + "\n"
				+ "Programming Skills:" + ((programmingSkills & 0x4) > 0 ? " Java" : "") + ((programmingSkills & 0x2) > 0 ? " C++" : "") + ((programmingSkills & 0x1) > 0 ? " JavaScript" : "") + "\n"
				+ "GPA: " + gpa + "\n"
				+ "Comment: " + comment + "\n";
	}

	/**
	 * Gets the value of the first name attribute.
	 * 
	 * @return The first name.
	 */
	public String getFirstName() { return firstName; }

	/**
	 * Sets the value of the first name attribute.
	 * 
	 * @param f The first name.
	 */
	public void setFirstName(String f) { firstName = f; }

	/**
	 * Gets the value of the last name attribute.
	 * 
	 * @return The last name.
	 */
	public String getLastName() { return lastName; }

	/**
	 * Sets the value of the last name attribute.
	 * 
	 * @param l The last name.
	 */
	public void setLastName(String l) {
		lastName = l;
	}

	/**
	 * Gets the value of the gender attribute.
	 * 0 - not set.
	 * 1 - male.
	 * 2 - female.
	 * 
	 * @return The gender.
	 */
	public int getGender() { return gender; }

	/**
	 * Sets the value of the gender attribute.
	 * 0 - not set.
	 * 1 - male.
	 * 2 - female.
	 * 
	 * @param g The gender.
	 */
	public void setGender(int g) { gender = g; }

	/**
	 * Gets the value of the programming skills attribute.
	 * bit 0 - JavaScript.
	 * bit 1 - C++.
	 * bit 2 - Java.
	 * 
	 * @return The programming skills.
	 */
	public int getProgrammingSkills() { return programmingSkills; }

	/**
	 * Sets the value of the programming skills attribute.
	 * bit 0 - JavaScript.
	 * bit 1 - C++.
	 * bit 2 - Java.
	 * 
	 * @param p The programming skills.
	 */
	public void setProgrammingSkills(int p) { programmingSkills = p; }

	/**
	 * Gets the value of the GPA attribute.
	 * 
	 * @return The GPA.
	 */
	public double getGpa() { return gpa; }

	/**
	 * Sets the value of the GPA attribute.
	 * 
	 * @param g The GPA.
	 */
	public void setGpa(double g) { gpa = g; }

	/**
	 * Gets the value of the comment attribute.
	 * 
	 * @return The comment.
	 */
	public String getComment() { return comment; }

	/**
	 * Sets the value of the comment attribute.
	 * 
	 * @param c The comment.
	 */
	public void setComment(String c) { comment = c; }

}

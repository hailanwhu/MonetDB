package nl.cwi.monetdb.jdbc;

import java.sql.*;

/**
 * The representation of a savepoint, which is a point within the current
 * transaction that can be referenced from the Connection.rollback method.
 * When a transaction is rolled back to a savepoint all changes made after
 * that savepoint are undone.
 * Savepoints can be either named or unnamed. Unnamed savepoints are
 * identified by an ID generated by the underlying data source.
 *
 * This little class is nothing more than a container for a name and/or
 * an id. By using some static variables in this class the validity of
 * the various savepoints can simply be checked.
 *
 * @author Fabian Groffen <Fabian.Groffen@cwi.nl>
 */
public class MonetSavepoint implements Savepoint {
	private static int highestId = 0;

	private String name;
	private int id;
	private boolean valid;

	public MonetSavepoint(String name) throws IllegalArgumentException {
		if (name == null) throw new IllegalArgumentException("null not allowed");

		this.id = getNextId();
		this.name = name;

		valid = true;
	}

	public MonetSavepoint() {
		this.id = getNextId();
		this.name = null;

		valid = true;
	}


	/**
	 * Retrieves the generated ID for the savepoint that this Savepoint object
	 * represents.
	 *
	 * @return the numeric ID of this savepoint
	 * @throws SQLException if this is a named savepoint
	 */
	public int getSavepointId() throws SQLException {
		if (!valid) throw new SQLException("Savepoint invalidated!");
		if (name != null) throw
			new SQLException("Access to ID denied in named savepoint.");

		return(getId());
	}

	/**
	 * Retrieves the name of the savepoint that this Savepoint object
	 * represents.
	 *
	 * @return the name of this savepoint
	 * @throws SQLException if this is an un-named savepoint
	 */
	public String getSavepointName() throws SQLException {
		if (!valid) throw new SQLException("Savepoint invalidated!");
		if (name == null) throw
			new SQLException("Unable to retrieve name of unnamed savepoint");

		return(name);
	}

	//== end of methods froom Savepoint interface

	/**
	 * Retrieves the savepoint id, like the getSavepointId method with the only
	 * difference that this method will always return the id, regardless of
	 * whether it is named or not.
	 *
	 * @return the numeric ID of this savepoint
	 */
	int getId() {
		return(id);
	}

	/**
	 * Invalidates this savepoint object, so it's internal data can't be
	 * retrieved anymore.
	 */
	void invalidate() {
		valid = false;
	}

	/**
	 * Returns whether this savepoint is still valid or that it has been
	 * invalidated in the past.
	 *
	 * @return boolean indicating if this Savepoint is valid or not
	 */
	boolean isValid() {
		return(valid);
	}

	/**
	 * Returns the next id, which is larger than the last returned id. This
	 * method is synchronized to prevent race conditions. Since this is static
	 * code, this method is shared by requests from multiple Connection objects.
	 * Therefore two successive calls to this method need not to have a
	 * difference of 1.
	 *
	 * @return the next int which is guaranteed to be higher than the one
	 *         at the last call to this method.
	 */
	private static synchronized int getNextId() {
		return(highestId++);
	}

	/**
	 * Returns the highest id returned by getNextId(). This method is also
	 * synchronized to prevent race conditions and thus guaranteed to be
	 * thread-safe.
	 *
	 * @return the highest id returned by a call to getNextId()
	 */
	private static synchronized int getHighestId() {
		return(highestId);
	}
}
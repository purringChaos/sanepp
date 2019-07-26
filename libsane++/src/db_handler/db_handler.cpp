#include <iostream>
#include <list>
#include <vector>
#include <sqlite3.h>
#include <functional>
#include <type_traits>

#include <db_handler/db_handler.hpp>
namespace sane {
    // A way to store the callback results. // TODO: Make a callback object with an incrementing ID to avoid copypasta
    std::vector<std::string> callbackResults;

    void addCallbackResult(const std::string &result) {
        callbackResults.push_back(result);
    }

    std::vector<std::string> getCallbackResults() {
        return callbackResults;
    }

    void clearCallbackResults() {
        callbackResults.clear();
    }

    /**
     * Callback function for sqlite3_exec() to call for every result row that it finds.
     *
     * @param t_unusedFunction      An open sqlite3* database, in this case not used but declared to match signature.
     * @param resultSetColumnCount  The number of columns in the result set.
     * @param rowData               The row's data.
     * @param columnNames           The column names.
     * @return                      0.
     */
    static int callback(void *t_unused, int t_columnCount, char** t_rowData, char **t_columnNames) {
        int index;

        // "Use" the unused function and other parameters to clear irrelevant unused warnings.
        if (t_unused or t_columnNames) {}

        for (index = 0; index < t_columnCount; index++) {
//            std::cout <<  "Default callback: The data in column " << t_columnNames[index] << " is: " << t_rowData[index] << std::endl;
            if (t_rowData[index]) {
//                std::cout << t_columnNames[index] << " = " << t_rowData[index] << std::endl;
                addCallbackResult(t_rowData[index]);
            } else {
//                std::cout << t_columnNames[index] << " = " << "NULL" << std::endl;
                addCallbackResult("NULL");
            }
        }

        printf("\n");

        return 0;
    }

    /**
     * Dummy function to provide in-place for callback function when one is not wanted.
     *
     * @param t_sqlite3PreparedStatement  Unused param for a caller to pass params to.
     * @return                            Always returns SQLITE_DONE
     */
    static int dummyCallback(void *t_unused, int t_columnCount, char** t_rowData, char **t_columnNames) {
        // "Use" the unused function and other parameters to clear irrelevant unused warnings.
        if (t_unused or t_columnCount or t_rowData or t_columnNames) {}

        return SQLITE_DONE;
    }

    /**
     * **DISABLED**: Works for one single query and then SQLite throws "query aborted".
     *
     * Performs a SQLite statement using sqlite3_exec, but with a custom callback function.
     *
     * @param t_dbName         Filename for the database.
     * @param t_sqlStatement   A SQLite statement.
     * @return
     */
    #if generic_false_statement
    int execSqlStatementCustom(sqlite3_callback_ t_callbackFunction, const std::string &t_dbName, const std::string &t_sqlStatement) {
        sqlite3 *db;
        char *errorMessages = nullptr;
        int rc;

        // Open the database file.
        rc = sqlite3_open(t_dbName.c_str(), &db);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return (SQLITE_CANTOPEN);
        }

        // Execute the passed SQL statement and for every result row that it finds it will call callback.
        rc = sqlite3_exec(db, t_sqlStatement.c_str(), t_callbackFunction, nullptr, &errorMessages);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", errorMessages);
            std::cerr << "SQL Error ver2: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_free(errorMessages);
        }

        // Close database after all successful operations.
        sqlite3_close(db);

        return SQLITE_OK;
    }
    #endif

    /**
     * Performs a SQLite statement using sqlite3_exec, with the default callback function.
     *
     * @param t_dbName         Filename for the database.
     * @param t_sqlStatement   A SQLite statement.
     * @return
     */
    int execSqlStatement(const std::string &t_dbName, const std::string &t_sqlStatement) {
        sqlite3 *db;
        char *errorMessages = nullptr;
        int rc;

        // Open the database file.
        rc = sqlite3_open(t_dbName.c_str(), &db);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return (SQLITE_CANTOPEN);
        }

        // Execute the passed SQL statement and for every result row that it finds it will call callback.
        rc = sqlite3_exec(db, t_sqlStatement.c_str(), callback, nullptr, &errorMessages);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", errorMessages);
            sqlite3_free(errorMessages);
        }

        // Close database after all successful operations.
        sqlite3_close(db);

        return SQLITE_OK;
    }

    /**
 * Performs a SQLite statement using sqlite3_exec, with a DUMMY callback function.
 *
 * @param t_dbName         Filename for the database.
 * @param t_sqlStatement   A SQLite statement.
 * @return
 */
    int execSqlStatementNoCallback(const std::string &t_dbName, const std::string &t_sqlStatement) {
        sqlite3 *db;
        char *errorMessages = nullptr;
        int rc;

        // Open the database file.
        rc = sqlite3_open(t_dbName.c_str(), &db);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return (SQLITE_CANTOPEN);
        }

        // Execute the passed SQL statement and for every result row that it finds it will call callback.
        rc = sqlite3_exec(db, t_sqlStatement.c_str(), dummyCallback, nullptr, &errorMessages);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", errorMessages);
            sqlite3_free(errorMessages);
        }

        // Close database after all successful operations.
        sqlite3_close(db);

        return SQLITE_OK;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // A way to store the callback results.
    std::vector<std::string> getYoutubeChannelsCallbackResults;

    void addGetYoutubeChannelsCallbackResult(const std::string &result) {
        callbackResults.push_back(result);
    }

    std::vector<std::string> getGetYoutubeChannelsCallbackResults() {
        return callbackResults;
    }

    void clearGetYoutubeChannelsCallbackResults() {
        callbackResults.clear();
    }

    /**
     * Callback function for sqlite3_exec() to call for every result row that it finds.
     *
     * @param t_unusedFunction      An open sqlite3* database, in this case not used but declared to match signature.
     * @param resultSetColumnCount  The number of columns in the result set.
     * @param rowData               The row's data.
     * @param columnNames           The column names.
     * @return                      0.
     */
    static int getYoutubeChannelsCallback(void *t_unused, int t_columnCount, char** t_rowData, char **t_columnNames) {
        int index;

        // "Use" the unused function to clear irrelevant unused warnings.
        if (t_unused) {}

        for (index = 0; index < t_columnCount; index++) {
            std::cout <<  "Default callback: The data in column " << t_columnNames[index] << " is: " << t_rowData[index] << std::endl;
            if (t_rowData[index]) {
                std::cout << t_columnNames[index] << " = " << t_rowData[index] << std::endl;
                addCallbackResult(t_rowData[index]);
            } else {
                std::cout << t_columnNames[index] << " = " << "NULL" << std::endl;
                addCallbackResult("NULL");
            }
        }

        printf("\n");

        return 0;
    }

    /**
     * Performs a SQLite statement using sqlite3_exec, with the default callback function.
     *
     * @param t_dbName         Filename for the database.
     * @param t_sqlStatement   A SQLite statement.
     * @return
     */
    int execSqlStatementGetYoutubeChannels(const std::string &t_dbName, const std::string &t_sqlStatement) {
        sqlite3 *db;
        char *errorMessages = nullptr;
        int rc;

        // Open the database file.
        rc = sqlite3_open(t_dbName.c_str(), &db);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return (SQLITE_CANTOPEN);
        }

        // Execute the passed SQL statement and for every result row that it finds it will call callback.
        rc = sqlite3_exec(db, t_sqlStatement.c_str(), getYoutubeChannelsCallback, nullptr, &errorMessages);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", errorMessages);
            sqlite3_free(errorMessages);
        }

        // Close database after all successful operations.
        sqlite3_close(db);

        return SQLITE_OK;
    }

    ///////////////////

    /**
    * NB: This function seemingly can't CREATE TABLE, for that please use doRawSqlite3ExecStatement.
    *
    * Perform an SQLite3 statement using sqlite3_prepare_v2()/sqlite3_step()/sqlite3_column_*()/sqlite3_finalize()
    * calls so that you can read the data in the same place where you actually need to handle it.
    *
    * Based on https://stackoverflow.com/a/31168999
    *
    * @param t_callbackFunction    Function to be called with prepared SQLite3 statement as parameter.
    *                              This is to let the function step through the statement and perform
    *                              its own set of code. This function *MUST* return SQLITE_DONE if successful.
    *
    *                              Suggested step-method:
    *                                  while ((rc = sqlite3_step(t_sqlite3PreparedStatment)) == SQLITE_ROW) { //code }
    *
    * @param t_dbName              Filename of database.
    * @param t_sql                 An SQL statement.
    * @return
    */
    int prepareAndRunSqlStatement(std::function<int(sqlite3_stmt *sqlite3PreparedStatement)> t_callbackFunction,
                                  const std::string &t_dbName, const std::string &t_sql) {
        sqlite3 *db;
        sqlite3_stmt *sqlite3PreparedStatement;

        int rc;

        // Open the database file.
        rc = sqlite3_open(t_dbName.c_str(), &db);
        std::cout << "sqlite3_open: " << rc << std::endl;
        if (rc != SQLITE_OK) {
            std::cerr << "Error opening database '" << t_dbName << "': " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return (SQLITE_CANTOPEN);
        }

        rc = sqlite3_prepare_v2(db, t_sql.c_str(), -1, &sqlite3PreparedStatement, nullptr);
        std::cout << "sqlite3_prepare_v2: " << rc << std::endl;
        if (rc != SQLITE_OK) {
            std::cerr << "Error preparing SQLite3 statement: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return SQLITE_ERROR;
        }
        // Send the prepared statement to the callback function to let it step through the statements and run own code.
        rc = t_callbackFunction(sqlite3PreparedStatement);
        std::cout << "t_callbackFunction: " << rc << std::endl;
        if (rc != SQLITE_DONE) {
            std::cerr << "Error sqlite3_step ended but does not have status SQLITE_DONE: " << sqlite3_errmsg(db) <<
                      std::endl;
        }

        // Destroy the prepared statement object.
        sqlite3_finalize(sqlite3PreparedStatement);

        // Close db handle
        sqlite3_close(db);

        return SQLITE_OK;
    }
} // namespace sane
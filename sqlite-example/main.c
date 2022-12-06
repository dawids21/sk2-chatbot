#include <stdio.h>
#include <sqlite3.h> // require SQLite installed on a system

// FROM: https://zetcode.com/db/sqlitec/

void insert_data(sqlite3 *db)
{
    char *err_msg;
    char *sql = "DROP TABLE IF EXISTS Cars;"
                "CREATE TABLE Cars(Id INT, Name TEXT, Price INT);"
                "INSERT INTO Cars VALUES(1, 'Audi', 52642);"
                "INSERT INTO Cars VALUES(2, 'Mercedes', 57127);"
                "INSERT INTO Cars VALUES(3, 'Skoda', 9000);";

    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
    }
}

void select_data(sqlite3 *db)
{
    sqlite3_stmt *res;

    int rc = sqlite3_prepare_v2(db, "SELECT Id, Name, Price FROM Cars;", -1, &res, 0);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    }

    while (sqlite3_step(res) != SQLITE_DONE)
    {
        printf("Id: %d, Name: %s, Price: %d\n", sqlite3_column_int(res, 0), sqlite3_column_text(res, 1), sqlite3_column_int(res, 2));
    }
    sqlite3_finalize(res);
}

int main(int argc, char const *argv[])
{
    sqlite3 *db;

    int rc = sqlite3_open("example.db", &db);

    if (rc != SQLITE_OK)
    {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return 1;
    }

    insert_data(db);
    select_data(db);

    sqlite3_close(db);

    return 0;
}

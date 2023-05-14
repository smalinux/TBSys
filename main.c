#include <sqlite3.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "log.h"

typedef struct Record {
   char name[50];      // Client name
   char phone[20];     // Phone number
   double price;       // Price
   char complain[502]; // Complain
} Record;

static struct config {
   char db_name[10];
} config = {
   .db_name = "tbsys.db",
};

void tbs_welcome_mgs(void);
void tbs_help_mgs(void);
void tbs_print_options_menu(void);

sqlite3* tbs_db_connect(void);
void tbs_db_disconnect(sqlite3 *db);

int tbs_db_bill_new(sqlite3 *db);
int tbs_db_bill_add(sqlite3 *db, Record *record);
int tbs_db_bill_remove(sqlite3 *db, int id);
int tbs_db_bill_select_all(sqlite3 *db);
int tbs_db_bill_complain(sqlite3 *db, int id, char *complain);
int tbs_db_bill_search(sqlite3 *db, char *keyword);
int tbs_db_bill_modify(sqlite3 *db, int id);

void tbs_print(const char *format, ...);


int main(int argc, char **argv) { // FIXME argc & argv

   char input;
   sqlite3 *db;
   Record record;

   tbs_welcome_mgs();
   tbs_help_mgs();

   // Open Connect db
   db = tbs_db_connect();
   log_info("DB conection opened");

   // Create DB Table: Bill
   tbs_db_bill_new(db);
   log_info("DB Table Bill is ready");

   tbs_print_options_menu();
   scanf(" %c",&input);

   /*
    * The main loop
    */
   while (input != 'q') {
      if (input == 'a') { // 'a' key
         tbs_print("> Add new record...\n");

         tbs_print("> Name: ");
         scanf(" %s", record.name);
         tbs_print("> Phone: ");
         // TODO code validation for logical phone inputs
         scanf(" %s", record.phone);
         tbs_print("> price: ");
         scanf(" %lf", &record.price);

         // TODO Print pretty table in terminal (add pretty table func)
         // and Sure? (Y/n)
         tbs_db_bill_add(db, &record);
         log_info("%s() function called", __func__);
      } else if (input == 'c') { // 'c' key
         int id;
         char complain[500];
         tbs_print("> ID: ");
         scanf(" %d", &id);
         tbs_print("> Complain (max 500 char): ");
         scanf(" %500[^\n]", complain); // FIXME 500 hardcode
         tbs_db_bill_complain(db, id, complain);
         log_info("%s() function called", __func__);
         // TODO Print pretty table in terminal (add pretty table func)
      } else if (input == 'd') { // 'd' key
         int id = 0;
         tbs_print("> Delete record... id: ");
         scanf(" %d", &id);
         tbs_db_bill_remove(db, id);
         log_info("%s() function called", __func__);

         // TODO Print pretty table in terminal (add pretty table func)
         // and Sure? (Y/n)
      } else if (input == 'h') { // 'h' key
         tbs_help_mgs();
      } else if (input == 'l') { // 'l' key
         tbs_db_bill_select_all(db);
         log_info("%s() function called", __func__);
         // TODO Print pretty table in terminal (add pretty table func)
         // and Sure? (Y/n)
      } else if (input == 'm') { // 'm' key
         int id = 0;
         tbs_print("> Modify record... id: ");
         scanf(" %d", &id);
         tbs_db_bill_modify(db, id);
         log_info("%s() function called", __func__);
      } else if (input == 's') { // 's' key
         tbs_print("> Search by: ");
         char keyword[20];
         scanf(" %s", keyword);
         tbs_db_bill_search(db, keyword);
         log_info("%s() function called", __func__);
      } else {
         tbs_print("Option (%c) not exist!\n", input);
         tbs_help_mgs();
      }

      tbs_print_options_menu();
      scanf(" %c", &input);
   }

   // Close DB Connect
   tbs_db_disconnect(db);
   log_info("DB conection closed");

   return 0;
}

void tbs_welcome_mgs(void) {
   tbs_print(
         "  _____  ____  ____\n"
         " |_   _|| __ )/ ___|  _   _  ___\n"
         "   | |  |  _ \\\\___ \\ | | | |/ __|\n"
         "   | |  | |_) |___) || |_| |\\__ \\\n"
         "   |_|  |____/|____/  \\__, ||___/\n"
         "                      |___/\n"
         "\n"
         );
}

void tbs_help_mgs(void) { // FIXME Is this optimal? like Htop?
   tbs_print(
         "l - list\n"
         "m - modify\n"
         "s - search\n"
         "d - delete\n"
         );
}

sqlite3* tbs_db_connect(void) {
   sqlite3 *db;
   char *err_msg = 0;
   int rc = sqlite3_open(config.db_name, &db);

   if (rc != SQLITE_OK) {
      log_error("SQL error: %s: %s\n", __func__, sqlite3_errmsg(db));

      sqlite3_free(err_msg);
      sqlite3_close(db);
      return NULL;
   }

   log_info("DB connection successfully opened");

   return db;
}

void tbs_db_disconnect(sqlite3* db) {
   sqlite3_close(db);
}

// Tolerant function, create Bill table if not already exit
int tbs_db_bill_new(sqlite3 *db) {
   char *err_msg = 0;
   int rc;
   char *sql =
      "CREATE TABLE IF NOT EXISTS Bill(Id INTEGER PRIMARY KEY AUTOINCREMENT, Name TEXT, Phone varchar(15) ,Price INT, Complain TEXT);";

   rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

   if (rc != SQLITE_OK ) {
      log_error("SQL error: %s: %s\n", __func__, err_msg);

      sqlite3_free(err_msg);
      sqlite3_close(db);
      return 1;
   }

   return 0;
}

int tbs_db_bill_add(sqlite3 *db, Record *record) {
   char *err_msg = 0;
   int rc;
   char sql[500];

   sprintf(sql, "INSERT INTO Bill (Name, Phone, Price) VALUES('%s', '%s', %f);",
         record->name,
         record->phone,
         record->price);

   rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

   // Print the last Id added to DB
   int last_id = sqlite3_last_insert_rowid(db);
   log_info("The last Id of the inserted row is %d\n", last_id);

   // FIXME print the info of last added item here

   if (rc != SQLITE_OK ) {
      log_error("SQL error: %s: %s\n", __func__, err_msg);

      sqlite3_free(err_msg);
      sqlite3_close(db);
      return 1;
   }

   return 0;
}

int tbs_db_bill_remove(sqlite3 *db, int id) {
   char *err_msg = 0;
   int rc;
   char sql[500];

   sprintf(sql, "DELETE FROM Bill WHERE id=%d;", id);

   rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

   if (rc != SQLITE_OK ) {
      log_error("SQL error: %s: %s\n", __func__, err_msg);

      sqlite3_free(err_msg);
      sqlite3_close(db);
      return 1;
   }

   return 0;
}

static int select_all_callback(void *NotUsed, int argc, char **argv, char **azColName) {

   for (int i = 0; i < argc; i++) {
      tbs_print("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   tbs_print("\n");

   return 0;
}

int tbs_db_bill_select_all(sqlite3 *db) {
   char *err_msg = 0;
   int rc;
   char *sql = "SELECT * FROM Bill";

   rc = sqlite3_exec(db, sql, select_all_callback, 0, &err_msg);

   if (rc != SQLITE_OK ) {
      log_error("SQL error: %s: %s\n", __func__, err_msg);

      sqlite3_free(err_msg);
      sqlite3_close(db);
      return 1;
   }

   return 0;
}

int tbs_db_bill_complain(sqlite3 *db, int id, char* complain) {
   char *err_msg = 0;
   int rc;
   char sql[500];

   sprintf(sql, "UPDATE Bill SET Complain = '%s' WHERE id = %d;",
         complain,
         id);

   rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

   if (rc != SQLITE_OK ) {
      log_error("SQL error: %s: %s\n", __func__, err_msg);

      sqlite3_free(err_msg);
      sqlite3_close(db);
      return 1;
   }

   return 0;
}

static int search_callback(void *NotUsed, int argc, char **argv, char **azColName) {

   for (int i = 0; i < argc; i++) {
      tbs_print("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   tbs_print("\n");

   return 0;
}

// TODO Scale the search to the whole table intead of 'Name' column only
int tbs_db_bill_search(sqlite3 *db, char* keyword) {
   char *err_msg = 0;
   int rc;
   char sql[100];

   sprintf(sql, "SELECT * FROM Bill WHERE Name LIKE '\%%%s\%%';", keyword);

   rc = sqlite3_exec(db, sql, search_callback, 0, &err_msg);

   if (rc != SQLITE_OK ) {
      log_error("SQL error: %s: %s\n", __func__, err_msg);

      sqlite3_free(err_msg);
      sqlite3_close(db);
      return 1;
   }

   return 0;
}

// catch the new values from user and send it back to tbs_db_bill_modify via userdata
static int modify_view_callback(void *userdata, int argc, char **argv, char **azColName) {
   // Retrieve the user data pointer
   Record *data = (Record *)userdata;

   // TODO accept empty input using Enter key.
   for (int i = 0; i < argc; i++) {
      tbs_print("%s (%s):\n", azColName[i], argv[i] ? argv[i] : "NULL");
      if ((strcmp(azColName[i], "Name") == 0)) {
         scanf("%s", data->name);
      } else if ((strcmp(azColName[i], "Phone") == 0)) {
         scanf(" %s", data->phone);
      } else if ((strcmp(azColName[i], "Price") == 0)) {
         scanf(" %lf", &data->price);
      } else if ((strcmp(azColName[i], "Complain") == 0)) {
         scanf(" %500[^\n]", data->complain);
      }
   }

   return 0;
}

int tbs_db_bill_modify(sqlite3 *db, int id) {
   char *err_msg = 0;
   int rc;
   char sql[1000];
   Record record;

   sprintf(sql, "SELECT * FROM Bill WHERE id = %d;", id);

   rc = sqlite3_exec(db, sql, modify_view_callback, &record, &err_msg);

   if (rc != SQLITE_OK ) {
      log_error("SQL error: %s: %s\n", __func__, err_msg);

      sqlite3_free(err_msg);
      sqlite3_close(db);

      return 1;
   }

   sprintf(sql, "UPDATE Bill SET Name='%s', Phone='%s', Price=%f, Complain='%s' WHERE id=%d;",
         record.name,
         record.phone,
         record.price,
         record.complain,
         id);

   rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

   if (rc != SQLITE_OK ) {
      log_error("SQL error: %s: %s\n", __func__, err_msg);

      sqlite3_free(err_msg);
      sqlite3_close(db);

      return 1;
   }

   return 0;
}

// use tbs_print for consistance instead of printf
void tbs_print(const char *format, ...) {
   va_list args;
   va_start(args, format);

   vprintf(format, args);

   va_end(args);
}

void tbs_print_options_menu(void) {
   tbs_print("Hit (a/c/d/h/m/s), h for help or q for exit: ");
}

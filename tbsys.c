/*
tbsys - tbsys.c
(C) 2023 Sohaib Mohamed
Released under the GNU GPLv2+,
Everyone is permitted to copy and distribute verbatim copies
of this license document, but changing it is not allowed.
*/

#include <sqlite3.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "tbsys.h"


void tbs_welcome_mgs(void) {
   tbs_print(
         "  _____  ____  ____\n"
         " |_   _|| __ )/ ___|  _   _  ___\n"
         "   | |  |  _ \\\\___ \\ | | | |/ __|\n"
         "   | |  | |_) |___) || |_| |\\__ \\\n"
         "   |_|  |____/|____/  \\__, ||___/\n"
         "                      |___/\n"
         "Telecom Billing System...\n"
         "\n"
         );
}

void tbs_help_mgs(void) { // FIXME Is this optimal? like Htop?
   tbs_print(
         "a - (a) add new record\n"
         "c - (c) add a complain\n"
         "d - (d) delete a record\n"
         "h - (h) help. prints this message\n"
         "l - (l) list all record\n"
         "m - (m) modify a record\n"
         "s - (s) search by name\n"
         );
}

sqlite3* tbs_db_connect(char *db_name) {
   sqlite3 *db;
   char *err_msg = 0;
   int rc = sqlite3_open(db_name, &db);

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

// Tolerant function, creates Bill table if not already exit.
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

   // TODO accept empty inputs... to skip
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
   tbs_print("Hit (a/c/d/h/l/m/s), h for help or q for exit: ");
}


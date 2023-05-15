/**
 * @file tbsys.c
 * @author Sohaib Mohamed (sohaib.amhmd@gmail.com)
 * @brief TBsys.c file
 * @version 0.1
 * @date 2023-05-15
 * @page this file is tbsys.c
 *
 * @copyright Copyright (c) 2023
 * Released under the GNU GPLv2+,
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
 *
 */
#include <sqlite3.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fort.h" // Pretty Print Tables
#include "log.h"

#include "tbsys.h"


static int row_counter;

void tbs_welcome_mgs(void) {
   tbs_print(
      "  _____  ____  ____\n"
      " |_   _|| __ )/ ___|  _   _  ___\n"
      "   | |  |  _ \\\\___ \\ | | | |/ __|\n"
      "   | |  | |_) |___) || |_| |\\__ \\\n"
      "   |_|  |____/|____/  \\__, ||___/\n"
      "                      |___/\n"
      " Telecom Billing System...\n"
      "\n"
   );
}

void tbs_help_mgs(void) {
   tbs_print(
      "(a) Add new record\n"
      "(c) Add a complain\n"
      "(d) Delete a record\n"
      "(h) Help. prints this message\n"
      "(l) List all record\n"
      "(m) Modify a record\n"
      "(s) Search by name\n"
   );
   log_info("%s() function called", __func__);
}

sqlite3* tbs_db_connect(char* db_name) {
   sqlite3* db;
   char* err_msg = 0;
   int rc = sqlite3_open(db_name, &db);

   log_info("%s() function called", __func__);

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
   log_info("%s() function called", __func__);
   sqlite3_close(db);
}

// Tolerant function, creates Bill table if not already exit.
int tbs_db_bill_new(sqlite3* db) {
   char* err_msg = 0;
   int rc;
   char* sql =
      "CREATE TABLE IF NOT EXISTS Bill(Id INTEGER PRIMARY KEY AUTOINCREMENT, Name TEXT, Phone varchar(15) ,Price INT, Complain TEXT);";

   log_info("%s() function called", __func__);

   rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

   if (rc != SQLITE_OK ) {
      log_error("SQL error: %s: %s\n", __func__, err_msg);

      sqlite3_free(err_msg);
      sqlite3_close(db);
      return 1;
   }

   return 0;
}

int tbs_db_bill_add(sqlite3* db, Record* record) {
   char* err_msg = 0;
   int rc;
   char sql[500];

   log_info("%s() function called", __func__);

   sprintf(sql, "INSERT INTO Bill (Name, Phone, Price) VALUES('%s', '%s', %f);",
           record->name,
           record->phone,
           record->price);

   rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

   // Print the last Id added to DB
   int last_id = sqlite3_last_insert_rowid(db);
   log_info("The last Id of the inserted row is %d\n", last_id);

   /* Print one row table */
   tbs_print_table_onerow(last_id, record);

   if (rc != SQLITE_OK ) {
      log_error("SQL error: %s: %s\n", __func__, err_msg);

      sqlite3_free(err_msg);
      return 1;
   }

   return 0;
}

int tbs_db_bill_remove(sqlite3* db, int id) {
   char* err_msg = 0;
   int rc;
   char sql[500];

   log_info("%s() function called", __func__);

   sprintf(sql, "DELETE FROM Bill WHERE id=%d;", id);

   rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

   if (rc != SQLITE_OK ) {
      log_error("SQL error: %s: %s\n", __func__, err_msg);

      sqlite3_free(err_msg);
      return 1;
   }

   return 0;
}

static int select_all_callback(void* userdata, int argc, char** argv, char** col_name) {
   // Retrieve the user data pointer
   Record* data = (Record*)userdata;

   for (int i = 0; i < argc; i++) {
      if ((strcmp(col_name[i], "Name") == 0)) {
         strcpy(data[row_counter].name, argv[i]);
      } else if ((strcmp(col_name[i], "Phone") == 0)) {
         strcpy(data[row_counter].phone, argv[i]);
      } else if ((strcmp(col_name[i], "Price") == 0)) {
         strcpy(data[row_counter].price_buff, argv[i]);
      } else if ((strcmp(col_name[i], "Complain") == 0)) {
         //strcpy(data[row_counter].complain, argv[i]); // FIXME
      } else if ((strcmp(col_name[i], "Id") == 0)) {
         strcpy(data[row_counter].id_buff, argv[i]);
      }
   }
   row_counter++;

   return 0;
}

static int tbs_db_bill_row_count(sqlite3* db) {
   char* err_msg = 0;
   int rc;
   sqlite3_stmt* stmt;
   char* sql = "select count(*) from Bill";

   log_info("%s() function called", __func__);

   rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

   if (rc != SQLITE_OK ) {
      log_error("SQL error: %s: %s\n", __func__, err_msg);

      sqlite3_free(err_msg);
      return 1;
   }

   rc = sqlite3_step(stmt);
   if (rc != SQLITE_ROW) {
      // error handling -> no rows returned, or an error occurred
   }
   return sqlite3_column_int(stmt, 0);
}

int tbs_db_bill_select_all(sqlite3* db) {
   char* err_msg = 0;
   int rc;
   char* sql = "SELECT * FROM Bill";
   row_counter = 0;

   /* get the total number of rows in DB */
   printf("%d\n", tbs_db_bill_row_count(db));
   int total_rows = tbs_db_bill_row_count(db);

   // Alloc records FIXME free it
   Record* records = malloc(total_rows * sizeof(Record));

   log_info("%s() function called", __func__);

   rc = sqlite3_exec(db, sql, select_all_callback, records, &err_msg);

   if (rc != SQLITE_OK ) {
      log_error("SQL error: %s: %s\n", __func__, err_msg);

      sqlite3_free(err_msg);
      return 1;
   }

   // Print Table
   tbs_print_table(total_rows, records);

   return 0;
}

int tbs_db_bill_complain(sqlite3* db, int id, char* complain) {
   char* err_msg = 0;
   int rc;
   char sql[500];

   log_info("%s() function called", __func__);

   sprintf(sql, "UPDATE Bill SET Complain = '%s' WHERE id = %d;",
           complain,
           id);

   rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

   if (rc != SQLITE_OK ) {
      log_error("SQL error: %s: %s\n", __func__, err_msg);

      sqlite3_free(err_msg);
      return 1;
   }

   return 0;
}

static int search_callback(void* NotUsed, int argc, char** argv, char** azColName) {

   for (int i = 0; i < argc; i++) {
      tbs_print("%s = %s\t ", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   tbs_print("\n");

   return 0;
}

// TODO Scale the search to the whole table intead of 'Name' column only
int tbs_db_bill_search(sqlite3* db, char* keyword) {
   char* err_msg = 0;
   int rc;
   char sql[100];

   log_info("%s() function called", __func__);

   sprintf(sql, "SELECT * FROM Bill WHERE Name LIKE '\%%%s\%%';", keyword);

   rc = sqlite3_exec(db, sql, search_callback, 0, &err_msg);

   if (rc != SQLITE_OK ) {
      log_error("SQL error: %s: %s\n", __func__, err_msg);

      sqlite3_free(err_msg);
      return 1;
   }

   return 0;
}

// catch the new values from user and send it back to tbs_db_bill_modify via userdata
static int modify_view_callback(void* userdata, int argc, char** argv, char** col_name) {
   // Retrieve the user data pointer
   Record* data = (Record*)userdata;

   // TODO accept empty inputs... to skip
   for (int i = 0; i < argc; i++) {
      tbs_print("%s (%s):\n", col_name[i], argv[i] ? argv[i] : "NULL");
      if ((strcmp(col_name[i], "Name") == 0)) {
         scanf("%s", data->name);
      } else if ((strcmp(col_name[i], "Phone") == 0)) {
         scanf(" %s", data->phone);
      } else if ((strcmp(col_name[i], "Price") == 0)) {
         scanf(" %lf", &data->price);
      } else if ((strcmp(col_name[i], "Complain") == 0)) {
         scanf(" %500[^\n]", data->complain);
      }
   }

   return 0;
}

int tbs_db_bill_modify(sqlite3* db, int id) {
   char* err_msg = 0;
   int rc;
   char sql[1000];
   Record record;

   log_info("%s() function called", __func__);

   sprintf(sql, "SELECT * FROM Bill WHERE id = %d;", id);

   rc = sqlite3_exec(db, sql, modify_view_callback, &record, &err_msg);

   if (rc != SQLITE_OK ) {
      log_error("SQL error: %s: %s\n", __func__, err_msg);

      sqlite3_free(err_msg);

      return 1;
   }

   sprintf(sql, "UPDATE Bill SET Name='%s', Phone='%s', Price=%f, Complain='%s' WHERE id=%d;",
           record.name,
           record.phone,
           record.price,
           record.complain,
           id);

   // Print the last Id added to DB
   int last_id = sqlite3_last_insert_rowid(db);

   /* Print one row table */
   tbs_print_table_onerow(last_id, &record);

   rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

   if (rc != SQLITE_OK ) {
      log_error("SQL error: %s: %s\n", __func__, err_msg);

      sqlite3_free(err_msg);

      return 1;
   }

   return 0;
}

// use tbs_print for consistance instead of printf
void tbs_print(const char* format, ...) {
   va_list args;
   va_start(args, format);

   vprintf(format, args);

   va_end(args);
}

void tbs_print_options_menu(void) {
   tbs_print("Hit (a/c/d/h/l/m/s), h for help or q for exit: ");
}

void tbs_print_table_onerow(int id, Record* record) {
   char price_buff[100];
   char id_buff[100];

   log_info("%s() function called", __func__);

   ft_table_t* table = ft_create_table();
   ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
   ft_write_ln(table, "ID", "Name", "Phone", "Price", "Complain");

   sprintf(price_buff, "%f", record->price);
   sprintf(id_buff, "%d", id);

   ft_write_ln(table, id_buff, record->name, record->phone, price_buff, record->complain);
   printf("%s\n", ft_to_string(table));
   ft_destroy_table(table);
}

void tbs_print_table(int total_rows, Record* records) {
   log_info("%s() function called", __func__);

   ft_table_t* table = ft_create_table();
   ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
   ft_write_ln(table, "ID", "Name", "Phone", "Price", "Complain");

   for (size_t i = 0; i < total_rows; i++) {
      ft_write_ln(table, records[i].id_buff, records[i].name, records[i].phone,
                  records[i].price_buff, records[i].complain);
   }
   printf("%s\n", ft_to_string(table));
   ft_destroy_table(table);
}

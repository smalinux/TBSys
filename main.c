#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Record {
   char name[50];      // Client name
   char phone[20];     // Phone number
   double price;       // Price
   char complain[502]; // Complain
} Record;

// REMOVEME Print SQLite C version
void tbs_sqlite3_version(void) {
   printf("%s\n", sqlite3_libversion());
}

// FIXME move these functions to .h & doc them
void tbs_welcome_mgs(void);
void tbs_help_mgs(void);

sqlite3* tbs_db_connect(void);
void tbs_db_disconnect(sqlite3 *db);
int tbs_db_bill_new(sqlite3 *db);
int tbs_db_bill_add(sqlite3 *db, Record *record); // FIXME add param Table
int tbs_db_bill_remove(sqlite3 *db, int id); //FIXME add param Table
int tbs_db_bill_select_all(sqlite3 *db);
int tbs_db_bill_complain(sqlite3 *db, int id, char *complain); // FIXME add param Table
int tbs_db_bill_search(sqlite3 *db, char *keyword);
int tbs_db_bill_modify(sqlite3 *db, int id); // FIXME add param Table


int main(void) { // FIXME argc & argv

   char c;
   sqlite3 *db; // FIXME close it
   Record record;

   tbs_welcome_mgs();
   tbs_help_mgs();

   // Connect db
   db = tbs_db_connect();
   tbs_db_bill_new(db);


   printf("Hit (l/s/d/a/m/c), h for help or q for exit: ");
   scanf(" %c",&c);

   while (c != 'q') { // main loop
      // Sorted alphabetically
      if (c == 'a') {
         printf("> Add new record...\n");

         printf("> Name: ");
         scanf(" %s", record.name);
         printf("> Phone: ");
         scanf(" %s", record.phone);
         printf("> price: ");
         scanf(" %lf", &record.price);
         //printf("=======================================\n");
         //printf("%s, %s, %f\n", record.name, record.phone, record.price);
         //printf("=======================================\n");

         // TODO Print pretty table in terminal (add pretty table func)
         // and Sure? (Y/n)
         tbs_db_bill_add(db, &record);
      } else if (c == 'c') {
         int id;
         char complain[500];
         printf("> ID: ");
         scanf(" %d", &id);
         printf("> Complain (max 500 char): ");
         scanf(" %500[^\n]", complain); // FIXME 500 hardcode
         tbs_db_bill_complain(db, id, complain);
         // TODO Print pretty table in terminal (add pretty table func)
      } else if (c == 'd') {
         int id = 0;
         printf("> Delete record... id: ");
         scanf(" %d", &id);
         tbs_db_bill_remove(db, id);

         // TODO Print pretty table in terminal (add pretty table func)
         // and Sure? (Y/n)
      } else if (c == 'h') {
         tbs_help_mgs();
      } else if (c == 'l') {
         tbs_db_bill_select_all(db);
         // TODO Print pretty table in terminal (add pretty table func)
         // and Sure? (Y/n)
      } else if (c == 'm') {
         int id = 0;
         printf("> Modify record... id: ");
         scanf(" %d", &id);
         tbs_db_bill_modify(db, id);
      } else if (c == 's') {
         printf("> Search by: ");
         char keyword[20];
         scanf(" %s", keyword);
         tbs_db_bill_search(db, keyword);
      } else {
         printf("Option (%c) not exist!\n", c);
         tbs_help_mgs();
      }

      printf("Hit (l/s/d/a/m/c), h for help or q for exit: ");
      scanf(" %c", &c);

   }

   tbs_db_disconnect(db);
   return 0;
}

void tbs_welcome_mgs(void) {
   printf(
         " ===============================================\n"
         " Welcome .......\n" // FIXME fancy ascii mgs
         " ===============================================\n"
         );
}

void tbs_help_mgs(void) { // FIXME Is this optimal? like Htop?
   printf(
         "l - list\n"
         "m - modify\n"
         "s - search\n"
         "d - delete\n"
         );
}

sqlite3* tbs_db_connect(void) {
   sqlite3 *db;
   char *err_msg = 0;

   // FIXME: move db name as const at the top of file
   int rc = sqlite3_open("tbsys.db", &db);

   if (rc != SQLITE_OK) {

      fprintf(stderr, "TBSys: Cannot open database: %s\n", sqlite3_errmsg(db));
      sqlite3_free(err_msg);
      sqlite3_close(db);

      return NULL;
   }

   // FIXME Logging: connected successfully
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

   // SQLite C last inserted row id
   // REMOVEME rm this block
   int last_id = sqlite3_last_insert_rowid(db);
   printf("The last Id of the inserted row is %d\n", last_id);

   if (rc != SQLITE_OK ) {
      // FIXME Logging system
      fprintf(stderr, "TBSys: SQL error: %s: %s\n", __func__, err_msg);

      sqlite3_free(err_msg);
      sqlite3_close(db);

      return 1;
   }

   return 0;
}

// FIXME add param for Table name
int tbs_db_bill_add(sqlite3 *db, Record *record) {
   char *err_msg = 0;
   int rc;
   char sql[500];

   sprintf(sql, "INSERT INTO Bill (Name, Phone, Price) VALUES('%s', '%s', %f);",
         record->name,
         record->phone,
         record->price);
   //printf("-----------------------------------\n");
   //printf("%s", sql);
   //printf("-----------------------------------\n");
   //
   //char *sql = "INSERT INTO Bill (Name, Phone, Price) VALUES('Mercedes', 01123456789, 57127);";

   rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

   // SQLite C last inserted row id
   // REMOVEME rm this block
   int last_id = sqlite3_last_insert_rowid(db);
   printf("The last Id of the inserted row is %d\n", last_id);

   if (rc != SQLITE_OK ) {
      // FIXME Logging system
      fprintf(stderr, "TBSys: SQL error: %s: %s\n", __func__, err_msg);

      sqlite3_free(err_msg);
      sqlite3_close(db);

      return 1;
   }

   return 0;
}

int tbs_db_bill_remove(sqlite3 *db, int id) { //FIXME add param Table
   char *err_msg = 0;
   int rc;
   char sql[500];

   sprintf(sql, "DELETE FROM Bill WHERE id=%d;", id); // FIXME Bill hard coded, param?

   rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

   if (rc != SQLITE_OK ) {
      // FIXME Logging system
      fprintf(stderr, "TBSys: SQL error: %s: %s\n", __func__, err_msg);

      sqlite3_free(err_msg);
      sqlite3_close(db);

      return 1;
   }

   return 0;
}

static int select_all_callback(void *NotUsed, int argc, char **argv, char **azColName) {

   for (int i = 0; i < argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");

   return 0;
}

int tbs_db_bill_select_all(sqlite3 *db) {
   char *err_msg = 0;
   int rc;
   char *sql = "SELECT * FROM Bill"; // FIXME add MACRO Bill

   rc = sqlite3_exec(db, sql, select_all_callback, 0, &err_msg);

   if (rc != SQLITE_OK ) {
      fprintf(stderr, "TBSys: SQL error: %s: %s\n", __func__, err_msg);

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
      // FIXME Logging system
      fprintf(stderr, "TBSys: SQL error: %s: %s\n", __func__, err_msg);

      sqlite3_free(err_msg);
      sqlite3_close(db);

      return 1;
   }

   return 0;
}

static int search_callback(void *NotUsed, int argc, char **argv, char **azColName) {

   for (int i = 0; i < argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");

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
      fprintf(stderr, "TBSys: SQL error: %s: %s\n", __func__, err_msg);

      sqlite3_free(err_msg);
      sqlite3_close(db);

      return 1;
   }

   return 0;
}

// get the new values
static int modify_view_callback(void *userdate, int argc, char **argv, char **azColName) {
   // Retrieve the user data pointer
   Record *data = (Record *)userdate;

   // TODO accept empty input using Enter key.
   for (int i = 0; i < argc; i++) {
      printf("%s (%s):\n", azColName[i], argv[i] ? argv[i] : "NULL");
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

   sprintf(sql, "SELECT * FROM Bill WHERE id = %d;", id); // FIXME add MACRO Bill

   rc = sqlite3_exec(db, sql, modify_view_callback, &record, &err_msg);

   if (rc != SQLITE_OK ) {
      fprintf(stderr, "TBSys: SQL error: %s: %s\n", __func__, err_msg);

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
      // FIXME Logging system
      fprintf(stderr, "TBSys: SQL error: %s: %s\n", __func__, err_msg);

      sqlite3_free(err_msg);
      sqlite3_close(db);

      return 1;
   }

   return 0;
}

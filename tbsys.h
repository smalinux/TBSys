#ifndef HEADER_tbsys
#define HEADER_tbsys
/*
tbsys - tbsys.h
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

typedef struct Record {
   char name[56];      // Client name - 50
   char phone[24];     // Phone number - 20
   double price;       // Price
   char complain[502]; // Complain - 500
} Record;


void tbs_welcome_mgs(void);
void tbs_help_mgs(void);
void tbs_print_options_menu(void);

sqlite3* tbs_db_connect(char *db_name);
void tbs_db_disconnect(sqlite3 *db);

int tbs_db_bill_new(sqlite3 *db);
int tbs_db_bill_add(sqlite3 *db, Record *record);
int tbs_db_bill_remove(sqlite3 *db, int id);
int tbs_db_bill_select_all(sqlite3 *db);
int tbs_db_bill_complain(sqlite3 *db, int id, char *complain);
int tbs_db_bill_search(sqlite3 *db, char *keyword);
int tbs_db_bill_modify(sqlite3 *db, int id);

void tbs_print(const char *format, ...);
void tbs_print_table_onerow(int id, Record *record);

#endif

/**
 * @file tbsys.h
 * @author Sohaib Mohamed (sohaib.amhmd@gmail.com)
 * @brief Header file to connect db and apply basic tbs funcs.
 * @version 0.1
 * @date 2023-05-15
 * @page this file is tbsys.h
 *
 * @copyright Copyright (c) 2023
 * Released under the GNU GPLv2+,
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
 *
 */
#ifndef HEADER_tbsys
#define HEADER_tbsys

#include <sqlite3.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Record Object. each element represent one DB column
 *
 */
typedef struct Record {
   int id;               // Recodrd id
   char id_buff[100];    // used for Convert & print
   char name[56];        // Client name - 50
   char phone[24];       // Phone number - 20
   double price;         // Price
   char price_buff[100]; // used for Convert & print
   char complain[502];   // Complain - 500
} Record;


/**
 * @brief Print welcome ascii message
 *
 */
void tbs_welcome_mgs(void);
/**
 * @brief Print detailed help list
 *
 */
void tbs_help_mgs(void);
/**
 * @brief Print one line options
 *
 */
void tbs_print_options_menu(void);
/**
 * @brief Connect to the Database
 *
 * @param db_name database name
 * @return sqlite3* sqlite obj return value
 */
sqlite3* tbs_db_connect(char* db_name);
/**
 * @brief Close the database connection
 *
 * @param db databse name
 */
void tbs_db_disconnect(sqlite3* db);
/**
 * @brief Create new table Called Bill
 * @details This function is too tolerant. will not overwrite the table if not exist
 *
 * @param db database name
 * @return int FIXME void?
 */
int tbs_db_bill_new(sqlite3* db);
/**
 * @brief Add new record to the Bill Table
 *
 * @param db databse name
 * @param record Record object
 * @return int return 0 if successed
 */
int tbs_db_bill_add(sqlite3* db, Record* record);
/**
 * @brief Remove record from database
 *
 * @param db database name
 * @param id the ID to delete it
 * @return int return 0 if successed
 */
int tbs_db_bill_remove(sqlite3* db, int id);
/**
 * @brief list all Bill table records
 *
 * @param db
 * @return int
 */
int tbs_db_bill_select_all(sqlite3* db);
/**
 * @brief Add a complain to specific client
 *
 * @param db database handleer
 * @param id the id you want to attach
 * @param complain add your comment here
 * @return int return value!
 */
int tbs_db_bill_complain(sqlite3* db, int id, char* complain);
/**
 * @brief Search by name or part of name
 *
 * @param db database handler
 * @param keyword the word to search by...
 * @return int return value!
 */
int tbs_db_bill_search(sqlite3* db, char* keyword);
/**
 * @brief Modify a record
 *
 * @param db database handler
 * @param id the id you want to change
 * @return int the return value!
 */
int tbs_db_bill_modify(sqlite3* db, int id);
/**
 * @brief tbs print routine acts like printf
 *
 * @param format
 * @param ...
 */
void tbs_print(const char* format, ...);
/**
 * @brief Print table contains just one row
 *
 * @param id
 * @param record
 */
void tbs_print_table_onerow(int id, Record* record);
/**
 * @brief Print complete table with many rows
 *
 * @param total_rows integer number represt the total number of row
 * @param records records!
 */
void tbs_print_table(int total_rows, Record* records);

#endif

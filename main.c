/**
 * @file main.c
 * @author Sohaib Mohamed (sohaib.amhmd@gmail.com)
 * @brief Main.c file
 * @version 0.1
 * @date 2023-05-15
 * @page this file is main.c
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


static struct config {
   char db_name[10];  /* DB Name */
   bool db_connected; /* Keep track the status of conn open/close? */
} config = {
   .db_name = "tbsys.db",
   .db_connected = 0,
};

int main(int argc, char** argv) { // TODO argc & argv support
   char input;
   sqlite3* db;
   Record record;
   int ok;

   tbs_welcome_mgs();
   tbs_help_mgs();

   // Open Connect db
   db = tbs_db_connect(config.db_name);
   log_info("DB conection opened");

   // Create DB Table: Bill
   tbs_db_bill_new(db);
   log_info("DB Table Bill is ready");

   tbs_print_options_menu();
   ok = scanf(" %c", &input);
   if (!ok) {
      log_error("%s: Invalid input!", __func__);
   }

   /*
    * The main loop
    */
   while (input != 'q') {
      /*
       * (a) Add new Record
       */
      if (input == 'a') {
         tbs_print("> Add new record...\n");

         tbs_print("> Name: ");
         ok = scanf(" %50[^\n]", record.name);
         if (!ok) {
            log_error("%s: Invalid input!", __func__);
            continue;
         }

         tbs_print("> Phone: ");
         // TODO code validation for logical phone inputs (Leave it now)
         ok = scanf(" %s", record.phone);
         if (!ok) {
            log_error("%s: Invalid input!", __func__);
            continue;
         }

         tbs_print("> price: ");
         ok = scanf(" %lf", &record.price);
         tbs_db_bill_add(db, &record);

         /*
          * (c) Add a complain
          */
      } else if (input == 'c') {
         int id;
         char complain[500];
         tbs_print("> ID: ");
         ok = scanf(" %d", &id);
         if (!ok) {
            log_error("%s: Invalid input!", __func__);
            continue;
         }

         tbs_print("> Complain (max 500 char): ");
         ok = scanf(" %500[^\n]", complain);
         if (!ok) {
            log_error("%s: Invalid input!", __func__);
            continue;
         }

         tbs_db_bill_complain(db, id, complain);
         // TODO Print pretty table in terminal (add pretty table func)

         /*
          * (d) Delete a record
          */
      } else if (input == 'd') {
         int id = 0;
         tbs_print("> Delete record... id: ");
         ok = scanf(" %d", &id);
         if (!ok) {
            log_error("%s: Invalid input!", __func__);
            continue;
         }

         tbs_db_bill_remove(db, id);

         // TODO Print pretty table in terminal (add pretty table func)
         // and Sure? (Y/n)

         /*
          * (h) help message
          */
      } else if (input == 'h') { // 'h' key
         tbs_help_mgs();

         /*
          * (l) List all records
          */
      } else if (input == 'l') { // 'l' key
         tbs_db_bill_select_all(db);
         // TODO Print pretty table in terminal (add pretty table func)
         // and Sure? (Y/n)

         /*
          * (m) Modify a record
          */
      } else if (input == 'm') { // 'm' key
         int id = 0;
         tbs_print("> Modify record... id: ");
         ok = scanf(" %d", &id);
         if (!ok) {
            log_error("%s: Invalid input!", __func__);
            continue;
         }

         tbs_db_bill_modify(db, id);

         /*
          * (s) Search by name
          */
      } else if (input == 's') { // 's' key
         tbs_print("> Search by: ");
         char keyword[20];
         ok = scanf(" %s", keyword);
         if (!ok) {
            log_error("%s: Invalid input!", __func__);
            continue;
         }

         tbs_db_bill_search(db, keyword);

         /*
          * Command not found
          */
      } else {
         tbs_print("Option (%c) not exist!\n", input);
         tbs_help_mgs();
      }

      tbs_print_options_menu();
      ok = scanf(" %c", &input);
      if (!ok) {
         log_error("%s: Invalid input!", __func__);
         continue;
      }
   }

   // Close DB Connect
   tbs_db_disconnect(db);
   log_info("DB conection closed");

   return 0;
}

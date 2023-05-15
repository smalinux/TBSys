/*
tbsys - main.c
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


static struct config {
   char db_name[10];
} config = {
   .db_name = "tbsys.db",
};

int main(int argc, char **argv) { // FIXME argc & argv

   char input;
   sqlite3 *db;
   Record record;

   tbs_welcome_mgs();
   tbs_help_mgs();

   // Open Connect db
   db = tbs_db_connect(config.db_name);
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

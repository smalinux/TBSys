/**
 * @file tests.c
 * @author Sohaib Mohamed (sohaib.amhmd@gmail.com)
 * @brief Unit Test file.
 * @version 0.1
 * @date 2023-05-15
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

#include <criterion/criterion.h>

Test(misc, failing) {
   cr_assert(0);
}

Test(misc, passing) {
   cr_assert(1);
}

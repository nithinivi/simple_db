#include "../include/utils.h"
#include <stdio.h>

void print_promt() { printf("db > "); }

void print_row(Row* row) { 
    printf("(%d, %s, %s) \n", row->id, row->username, row->email);
 }
#if !defined(TABLE_H)
#define TABLE_H

#include <stdint.h>
#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255

typedef struct
{
  uint32_t id;
  char username[COLUMN_USERNAME_SIZE];
  char email[COLUMN_EMAIL_SIZE];
} Row;

#define size_of_attribute(Struct, Attribute) sizeof(((Struct *)0)->Attribute)

const uint32_t ID_SIZE = size_of_attribute(Row, id);
const uint32_t USERNAME_SIZE = size_of_attribute(Row, username);
const uint32_t EMAIL_SIZE = size_of_attribute(Row, email);
const uint32_t OFFSET_ID = 0;
const uint32_t OFFSET_USERNAME = OFFSET_ID + ID_SIZE;
const uint32_t OFFSET_EMAIL = OFFSET_USERNAME + USERNAME_SIZE;
const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

#define TABLE_MAX_PAGES 100
const uint32_t PAGE_SIZE = 4096;
const uint32_t ROW_PRE_PAGE = PAGE_SIZE / ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = ROW_PRE_PAGE * TABLE_MAX_PAGES;

typedef struct
{
  uint32_t num_rows;
  void *pages[TABLE_MAX_PAGES];
} Table;

#endif // TABLE_H

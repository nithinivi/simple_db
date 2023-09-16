#ifndef SQLCOMMAND_PROCESSOR
#define SQLCOMMAND_PROCESSOR


#include "table.h"

typedef enum{
  EXECUTE_SUCCESS, EXCUTE_TABLE_FULL
}ExecuteResult;

typedef enum {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGINZED_COMMAND
} MetaCommandResult;

typedef enum{
  PREPARE_SUCCESS,
  PREPARE_NEGATIVE_ID,
  PREPARE_STRING_TOO_LONG,
  PREPARE_SYNTAX_ERROR,
  PREPARE_UNRECOGINZED_STATEMENT
}PrepareResult;

typedef enum {
  STATEMENT_INSERT,STATEMENT_SELECT
}StatementType;

typedef struct {
  StatementType state;
  Row row_to_insert;
}Statement;


#endif // SQLCOMMAND_PROCESSOR

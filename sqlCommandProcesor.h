typedef enum {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGINZED_COMMAND
} MetaCommandResult;

typedef enum{
  PREPARE_SUCCESS,
  PREPARE_UNRECOGINZED_STATEMENT
}PrepareResult;

typedef enum {
  STATEMENT_INSERT,STATEMENT_SELECT
}StatementType;

typedef struct {
  StatementType state;
}Statement;

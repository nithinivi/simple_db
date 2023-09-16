#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/interface.h"
#include "../include/sqlCommandProcesor.h"

void print_promt() { printf("db > "); }

void print_row(Row *row) {
  printf("(%d, %s, %s)\n", row->id, row->username, row->email);
}

void read_input(InputBuffer *input_buffer) {
  // getline uses realloc the resize of buffer
  ssize_t bytes_read =
      getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);
  if (bytes_read <= 0) {
    printf("Error reading input \n");
    exit(EXIT_FAILURE);
  }
  input_buffer->input_length = bytes_read - 1;
  input_buffer->buffer[bytes_read - 1] = 0;
}

void close_input_buffer(InputBuffer *input_buffer) {
  free(input_buffer->buffer);
  free(input_buffer);
}

void free_table(Table *table) {
  for (int i = 0; table->pages[i]; i++) {
    free(table->pages[i]);
  }
}

MetaCommandResult do_meta_command(InputBuffer *input_buffer, Table *table) {
  if (strcmp(input_buffer->buffer, ".exit") == 0) {
    close_input_buffer(input_buffer);
    free_table(table);
    exit(EXIT_SUCCESS);
  } else {
    return META_COMMAND_UNRECOGINZED_COMMAND;
  }
}


PrepareResult prepare_insert(InputBuffer *input_buffer, Statement *statement) {
  statement->state = STATEMENT_INSERT;
  char *keyword = strtok(input_buffer->buffer, " ");
  char *id_string = strtok(NULL, " ");
  char *username = strtok(NULL, " ");
  char *email = strtok(NULL, " ");

  if (id_string == NULL | username == NULL | email == NULL) {
    return PREPARE_SYNTAX_ERROR;
  }

  int id = atoi(id_string);
  if (id < 0) {
    return PREPARE_NEGATIVE_ID;
  }

  if (strlen(username) > COLUMN_USERNAME_SIZE) {
    return PREPARE_STRING_TOO_LONG;
  }

  if (strlen(email) > COLUMN_EMAIL_SIZE) {
    return PREPARE_STRING_TOO_LONG;
  }

  statement->row_to_insert.id = id;
  strcpy(statement->row_to_insert.username, username);
  strcpy(statement->row_to_insert.email, email);

  return PREPARE_SUCCESS;
}

PrepareResult prepare_statement(InputBuffer *input_buffer,
                                Statement *statement) {
  if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
    return prepare_insert(input_buffer, statement);
  }

  if (strcmp(input_buffer->buffer, "select") == 0) {
    statement->state = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }
  return PREPARE_UNRECOGINZED_STATEMENT;
}

void serialize_row(Row *source, void *destination) {
  memcpy(destination + OFFSET_ID, &(source->id), ID_SIZE);
  memcpy(destination + OFFSET_USERNAME, &(source->username), USERNAME_SIZE);
  memcpy(destination + OFFSET_EMAIL, &(source->email), EMAIL_SIZE);
}

void deserialize_row(void *source, Row *destination) {
  memcpy(&(destination->id), source + OFFSET_ID, ID_SIZE);
  memcpy(&(destination->username), source + OFFSET_USERNAME, USERNAME_SIZE);
  memcpy(&(destination->email), source + OFFSET_EMAIL, EMAIL_SIZE);
}

void *row_slot(Table *table, uint32_t row_num) {
  uint32_t page_num = row_num / ROW_PRE_PAGE;
  void *page = table->pages[page_num];
  if (page == NULL) {
    page = table->pages[page_num] = malloc(PAGE_SIZE);
  }
  uint32_t row_offset = row_num % ROW_PRE_PAGE;
  uint32_t byte_offset = row_offset * PAGE_SIZE;
  return page + byte_offset;
}

ExecuteResult excute_insert(Statement *statement, Table *table) {
  if (table->num_rows >= TABLE_MAX_ROWS) {
    return EXCUTE_TABLE_FULL;
  }

  Row *row_to_insert = &(statement->row_to_insert);
  void *destination = row_slot(table, table->num_rows);
  serialize_row(row_to_insert, destination);
  table->num_rows += 1;

  return EXECUTE_SUCCESS;
}

ExecuteResult excute_select(Statement *statement, Table *table) {
  Row row;
  for (uint32_t i = 0; i < table->num_rows; i++) {
    deserialize_row(row_slot(table, i), &row);
    print_row(&row);
  }
  return EXECUTE_SUCCESS;
}

ExecuteResult excute_statement(Statement *statement, Table *table) {
  switch (statement->state) {
    case STATEMENT_INSERT:
      return excute_insert(statement, table);
    case STATEMENT_SELECT:
      return excute_select(statement, table);
  }
}

Table *new_table() {
  Table *table = (Table *)malloc(sizeof(Table));
  table->num_rows = 0;
  for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
    table->pages[i] = NULL;
  }
  return table;
}

int main(int argc, char *argv[]) {
  Table *table = new_table();
  InputBuffer *input_buffer = new_input_buffer();
  while (true) {
    print_promt();
    read_input(input_buffer);

    if (strcmp(input_buffer->buffer, ".exit") == 0) {
      if (input_buffer->buffer[0] == '.') {
        switch (do_meta_command(input_buffer, table)) {
          case META_COMMAND_SUCCESS:
            continue;
          case META_COMMAND_UNRECOGINZED_COMMAND:
            printf("Unrecoginzed command %s \n", input_buffer->buffer);
            continue;
        }
      }
    }

    Statement statement;
    switch (prepare_statement(input_buffer, &statement)) {
      case PREPARE_SUCCESS:
        break;
      case PREPARE_STRING_TOO_LONG:
        printf("String is too long.\n");
        break;

      case PREPARE_NEGATIVE_ID:
        printf("ID must be positive.\n");
        continue;

      case PREPARE_SYNTAX_ERROR:
        printf("Syntax error. Could not parse statement.\n");
        continue;

      case PREPARE_UNRECOGINZED_STATEMENT:
        printf("Unrecognized keyword at start of '%s'.\n",
               input_buffer->buffer);
        continue;
    }

    switch (excute_statement(&statement, table)) {
      case EXECUTE_SUCCESS:
        printf("Executed.\n");
        break;
      case EXCUTE_TABLE_FULL:
        printf("Error: Table full.\n");
        break;
    }
  }
}
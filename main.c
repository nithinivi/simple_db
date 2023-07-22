#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "table.h"
#include "interface.h"
#include "sqlCommandProcesor.h"




void print_promt(){
  printf("db > ");
}

void read_input(InputBuffer* input_buffer){
  // getline use realloc the resize of buffer
  ssize_t bytes_read  = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);
  if (bytes_read <= 0){
    printf("Error reading input \n");
    exit(EXIT_FAILURE);
  }
  input_buffer->input_length = bytes_read-1;
  input_buffer->buffer[bytes_read-1]=0;
}

void close_input_buffer(InputBuffer* input_buffer){
  free(input_buffer->buffer);
  free(input_buffer);
}


MetaCommandResult do_meta_command(InputBuffer* input_buffer){
  if (strcmp(input_buffer->buffer, ".exit")==0) {
    exit(EXIT_SUCCESS);
  }else {
    return META_COMMAND_UNRECOGINZED_COMMAND;
  }
}



PrepareResult prepare_statement(InputBuffer* inputBuffer, Statement* statement){

  if (strncmp(inputBuffer->buffer, "insert", 6)==0) {
    statement->state = STATEMENT_INSERT;
    return PREPARE_SUCCESS;
  }

  if (strcmp(inputBuffer->buffer, "select")==0) {
    statement->state = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }
  return PREPARE_UNRECOGINZED_STATEMENT;
}


void excute_statement(Statement* statement){
  switch (statement->state) {
  case STATEMENT_INSERT:
    printf("Inserting statement\n");
    break;
  case STATEMENT_SELECT:
    printf("selecting statement\n");
    break;
  }

}


void serialize_row(Row* source, void* destination){
  memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE);
  memcpy(destination + USERNAME_OFFSET, &(source->username), USERNAME_SIZE);
  memcpy(destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
}


void deserialize_row(void* source, Row* destination){
  memcpy(&(destination->id), source + ID_OFFSET, ID_SIZE);
  memcpy(&(destination->username), source + USERNAME_OFFSET, USERNAME_SIZE);
  memcpy(&(destination->email), source + EMAIL_OFFSET, EMAIL_SIZE);
 }

int main(int argc, char *argv[])
{
  InputBuffer* input_buffer = new_input_buffer();
  while (true) {
    print_promt();
    read_input(input_buffer);

    if (strcmp(input_buffer->buffer, ".exit")==0){
      if(input_buffer->buffer[0] == '.'){
	switch (do_meta_command(input_buffer)) {
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
    case PREPARE_UNRECOGINZED_STATEMENT:
      printf("Unrecognized keyword at start of '%s'.\n",
	     input_buffer->buffer);
      continue;
    }

    excute_statement(&statement);
    printf("Excuted ! \n");
  }

  return 0;
}

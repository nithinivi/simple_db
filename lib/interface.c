#include "../include/interface.h"


InputBuffer* new_input_buffer(){
  InputBuffer* input_buffer = (InputBuffer*) malloc(sizeof(InputBuffer));
  input_buffer->buffer = NULL;
  input_buffer->buffer_length = 0;
  input_buffer->buffer_length= 0;
  return input_buffer;
}
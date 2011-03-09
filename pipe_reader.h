#include "m_pd.h"
#include <stdio.h>

#define MAX_OUTPUT_ATOMS 8

static t_class* pipe_reader_class;

typedef struct _pipe_reader
{
	t_object  x_obj;
	t_outlet* output;
	FILE* pipe;
	char* input_buffer;
	size_t buffer_size;
} t_pipe_reader;

void pipe_reader_setup(void);

void* pipe_reader_new(void);
void pipe_reader_free(t_pipe_reader* x);

void pipe_reader_bang(t_pipe_reader* x);

void pipe_reader_open_file(t_pipe_reader* x, t_symbol* filename);
void pipe_reader_close_file(t_pipe_reader* x);

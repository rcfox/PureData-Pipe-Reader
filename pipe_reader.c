#include "pipe_reader.h"
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

static void chomp (char* s)
{
	size_t end = strlen(s) - 1;
	if (end >= 0 && s[end] == '\n')
	{
		s[end] = '\0';
	}
}

void pipe_reader_bang(t_pipe_reader* x)
{
	if(x->pipe)
	{
		size_t counter = 0;
		t_atom atoms[MAX_OUTPUT_ATOMS];

		// Read a line from the file.
		ssize_t read = getline(&x->input_buffer,&x->buffer_size,x->pipe);

		if(read > 0)
		{
			chomp(x->input_buffer);

			char* str = strtok(x->input_buffer," ");
			while(str && counter < MAX_OUTPUT_ATOMS)
			{
				char* end;
				double d = strtod(str,&end);
				if(str != end) // Check to see if the string contains a number.
				{
					SETFLOAT(&atoms[counter],d);
				}
				else
				{
					SETSYMBOL(&atoms[counter],gensym(str));
				}
				++counter;
				str = strtok(NULL," ");
			}
			
			outlet_anything(x->output,
			                atom_getsymbol(&atoms[0]), // The message type
			                counter,&atoms[1]); // The rest of the atoms
		}
		else
		{
			// If there's nothing else to read, go to the beginning and clear the error.
			// We'll try again next time.
			rewind(x->pipe);
		}
	}
}

void* pipe_reader_new(void)
{
	t_pipe_reader* x = (t_pipe_reader*)pd_new(pipe_reader_class);
	x->input_buffer = NULL;
	x->buffer_size = 0;
	x->pipe = NULL;

	x->output = outlet_new(&x->x_obj, 0);
	
	return (void*)x;
}

void pipe_reader_free(t_pipe_reader* x)
{
	pipe_reader_close_file(x);
}

void pipe_reader_setup(void)
{
	pipe_reader_class = class_new(gensym("pipe_reader"),
	                              (t_newmethod)pipe_reader_new,
	                              (t_method)pipe_reader_free,
	                              sizeof(t_pipe_reader),
	                              CLASS_DEFAULT, 0);
	class_addbang(pipe_reader_class, pipe_reader_bang);

	class_addmethod(pipe_reader_class, (t_method)pipe_reader_open_file, gensym("open"), A_DEFSYMBOL, 0);
	class_addmethod(pipe_reader_class, (t_method)pipe_reader_close_file, gensym("close"), 0);
}

void pipe_reader_open_file(t_pipe_reader* x, t_symbol* filename)
{
	post("Opening %s",filename->s_name);

	if(x->pipe)
	{
		pipe_reader_close_file(x);
	}

	// Allow us to open the pipe before anyone else has it open for writing.
	int fifo_fd = open(filename->s_name, O_RDONLY | O_NONBLOCK);
	x->pipe = fdopen(fifo_fd, "r");
	
	if(!x->pipe)
	{
		post("Couldn't open %s.",filename->s_name);
	}
}


void pipe_reader_close_file(t_pipe_reader* x)
{
	if(x->pipe)
	{
		fclose(x->pipe);
		x->pipe = NULL;
	}
	if(x->input_buffer)
	{
		free(x->input_buffer);
		x->input_buffer = NULL;
		x->buffer_size = 0;
	}
}

#include "pipe_reader.h"
#include <stdlib.h>

void pipe_reader_bang(t_pipe_reader* x)
{
	if(x->pipe)
	{
		size_t read = getline(&x->input_buffer,&x->buffer_size,x->pipe);
		
		if(read > 0)
		{
			post(x->input_buffer);
		}
	}
}

void pipe_reader_message(t_pipe_reader* x, t_symbol* s, int argc, t_atom* argv)
{
	if(s == gensym("open"))
	{
		if(argc > 0)
		{
			char filename[256];
			atom_string(argv,filename,sizeof(filename));
			pipe_reader_open_file(x,filename);
		}
		else
		{
			post("No filename given.");
		}
	}
	else if(s == gensym("close"))
	{
		pipe_reader_close_file(x);
	}
	else
	{
		// Ignore unexpected messages.
	}
}

void* pipe_reader_new(void)
{
	t_pipe_reader* x = (t_pipe_reader*)pd_new(pipe_reader_class);
	x->input_buffer = NULL;
	x->buffer_size = 0;
	
	return (void*)x;
}

void pipe_reader_free(t_pipe_reader* x)
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

void pipe_reader_setup(void)
{
	pipe_reader_class = class_new(gensym("pipe_reader"),
	                              (t_newmethod)pipe_reader_new,
	                              (t_method)pipe_reader_free,
	                              sizeof(t_pipe_reader),
	                              CLASS_DEFAULT, 0);
	class_addbang(pipe_reader_class, pipe_reader_bang);
	class_addanything(pipe_reader_class, pipe_reader_message);
}

void pipe_reader_open_file(t_pipe_reader* x, const char* filename)
{
	char buf[256];
	snprintf(buf,sizeof(buf),"Opening %s.",filename);
	post(buf);
			
	x->pipe = fopen(filename,"r");
	
	if(!x->pipe)
	{
		snprintf(buf,sizeof(buf),"Couldn't open %s.",filename);
		post(buf);
		return;
	}
}


void pipe_reader_close_file(t_pipe_reader* x)
{
	if(x->pipe)
	{
		fclose(x->pipe);
		x->pipe = NULL;
		
		free(x->input_buffer);
		x->input_buffer = NULL;
		x->buffer_size = 0;
	}
	else
	{
		post("No file opened.");
	}
}

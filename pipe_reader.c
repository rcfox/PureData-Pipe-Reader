#include "pipe_reader.h"
#include <stdlib.h>

void pipe_reader_bang(t_pipe_reader* x)
{
	if(x->pipe)
	{
		ssize_t read = getline(&x->input_buffer,&x->buffer_size,x->pipe);
		
		if(read > 0)
		{
			post(x->input_buffer);
		}
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
	char buf[256];
	snprintf(buf,sizeof(buf),"Opening %s.",filename->s_name);
	post(buf);

	if(x->pipe)
	{
		pipe_reader_close_file(x);
	}
			
	x->pipe = fopen(filename->s_name,"r");
	
	if(!x->pipe)
	{
		snprintf(buf,sizeof(buf),"Couldn't open %s.",filename->s_name);
		post(buf);
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

#include "pipe_reader.h"

void pipe_reader_bang(t_pipe_reader* x)
{
	post("Hello world !!");
}

void* pipe_reader_new(void)
{
	t_pipe_reader* x = (t_pipe_reader*)pd_new(pipe_reader_class);

	return (void*)x;
}

void pipe_reader_setup(void)
{
	pipe_reader_class = class_new(gensym("pipe_reader"),
	                              (t_newmethod)pipe_reader_new,
	                              0, sizeof(t_pipe_reader),
	                              CLASS_DEFAULT, 0);
	class_addbang(pipe_reader_class, pipe_reader_bang);
}

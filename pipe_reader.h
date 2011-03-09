#include "m_pd.h"

static t_class* pipe_reader_class;

typedef struct _pipe_reader
{
	t_object  x_obj;
} t_pipe_reader;

void* pipe_reader_new(void);
void pipe_reader_bang(t_pipe_reader* x);
void pipe_reader_setup(void);

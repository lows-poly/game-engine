#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#include "log.h"

static enum log_level current_level = LOG_INFO;

void log_set_level( enum log_level level )
{
	current_level = level;
}

void log_write( enum log_level level, const char *file, int line, 
                const char *func, const char *fmt, ... )
{
	static const char *tag[] = { "ERR", "WARN", "INFO", "DEBUG" };
	va_list ap;

	if( level > current_level )
		return;

	fprintf( stderr, "[%s] %s:%d %s(): ", tag[level], file, line, func );

	va_start( ap, fmt );
	vprintf( fmt, ap );
	va_end( ap );

	fprintf( stderr, "\n" );
}

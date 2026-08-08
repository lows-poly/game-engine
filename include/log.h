#ifndef ENGINE_LOG_H
#define ENGINE_LOG_H

enum log_level {
	LOG_ERR = 0,
	LOG_WARN,
	LOG_INFO,
	LOG_DEBUG
};

void log_set_level( enum log_level level );
void log_write( enum log_level level, const char *file, int line, const char *func,
                const char *fmt, ... )
                __attribute__(( format( printf, 5, 6 ) ));

#define pr_err( fmt, ... ) \
	log_write( LOG_ERR, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__ )
#define pr_warn( fmt, ... ) \
	log_write( LOG_WARN, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__ )
#define pr_info( fmt, ... ) \
	log_write( LOG_INFO, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__ )

#ifdef DEBUG
#define pr_debug(fmt, ...) \
	log_write(LOG_DEBUG, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#else
#define pr_debug(fmt, ...) do { } while ( 0 )
#endif

#endif

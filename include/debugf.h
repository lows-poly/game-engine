#ifndef DEBUGF_H
#define DEBUGF_H

#include <stddef.h>

enum field_type {
	FIELD_BOOL,
	FIELD_INT,
	FIELD_UINT,
	FIELD_FLOAT,
	FIELD_DOUBLE,
	FIELD_STRING,
	FIELD_STRUCT,
	FIELD_STRUCT_PTR
};

struct field_desc {
	const char              *name;
	enum field_type         type;
	size_t                  offset;
	const struct field_desc *nested;
	int                     nested_count;
};

#define FIELD( struct_type, member, ftype ) \
	{ #member, ftype, offsetof( struct_type, member ), NULL, 0 }

#define FIELD_NESTED( struct_type, member, desc, count ) \
	{ #member, FIELD_STRUCT, offsetof( struct_type, member ), desc, count }

#define FIELD_NESTED_PTR( struct_type, member, desc, count ) \
	{ #member, FIELD_STRUCT_PTR, offsetof( struct_type, member ), desc, count }

void dump( const void *base, const struct field_desc *fields, int count, int level );

#endif

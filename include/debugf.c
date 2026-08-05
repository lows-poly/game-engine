#include <stdio.h>
#include <stdbool.h>

#include "debugf.h"

static void dump_indent( int level )
{
	int i;

	for ( i = 0; i < level, i++ )
		printf("\t");
}

void dump( const void *base, const struct field_desc *fields, int count, int level )
{
	int i;
	const void *p;

	if ( !base || !fields )
		return;

	for ( i = 0; i < count; i++ ) {
		p = (const char *)base + fields[i].offset;

		dump_indent( level );

		switch( fields[i].type ) {
		case FIELD_BOOL:
			printf("%s: %s\n", fields[i].name, *(const bool *)p);
			break;
		case FIELD_INT:
			printf("%s: %d\n", fields[i].name, *(const int *)p);
			break;
		case FIELD_UINT:
			printf("%s: %u\n", fields[i].name, *(const unsigned int *)p);
			break;
		case FIELD_FLOAT:
			printf("%s: %f\n", fields[i].name, *(const float *)p);
			break;
		case FIELD_DOUBLE:
			printf("%s: %f\n", fields[i].name, *(const double *)p);
			break;
		case FIELD_STRING:
			printf("%s: %s\n", fields[i].name, *(const char * const *)p);
			break;
		case FIELD_STRUCT:
			if ( !fields[i].nested ) {
				printf("%s: <null nested desc>\n", fields[i].name);
				break;
			}
			printf("%s: {\n", fields[i].name);
			dump( p, fields[i].nested, fields[i].nested_count, level + 1 );
			dump_indent (level );
			printf("}\n");
			break;
		case FIELD_STRUCT_PTR: {
			const void *deref = *(const void * const *)p;

			if ( !fields[i].nested ) {
				printf("%s: <null nested desc>\n", fields[i].name);
				break;
			}
			if ( !deref ) {
				printf("%s: (null)\n", fields[i].name);
				break;
			}
			printf("%s: {\n", fields[i].name);
			dump( deref, fields[i].nested, fields[i].nested_count, level + 1 );
			dump_indent( level );
			printf("}\n");
			break;
		}
		default:
			printf("%s: <unknown field type %d>\n", fields[i].name, fields[i].type);
			break;
		}
	}
}

#include "type.h"
#include "const.h"
#include "protect.h"
#include "tty.h"
#include "console.h"
#include "proc.h"
#include "proto.h"
#include "global.h"

int vsprintf(char *buf, const char *fmt, va_list arg)
{
	char *p;
	char tmp[256] = {0};
	va_list p_next_arg = arg;

	for (p=buf; *fmt; fmt++) {
		if (*fmt != '%') {
			*p++ = *fmt;
			continue;
		}

		fmt++;
		
		switch (*fmt) {
		case 'x' :
			itoa(tmp, *((int *)p_next_arg));
			strcpy(p, tmp);
			p_next_arg += 4;
			p += strlen(tmp);
			break;
		case 's':
			break;
		default:
			break;
		}
	}
	return (p - buf);
}

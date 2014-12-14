#include "type.h"
#include "const.h"
#include "protect.h"
#include "tty.h"
#include "console.h"
#include "proc.h"
#include "proto.h"
#include "global.h"

int printf(const char *fmt, ...)
{
	int i;
	char buf[1024] = {0};

	va_list arg = (va_list) ((char *)(&fmt) + 4);
	i = vsprintf(buf, fmt, arg);
	buf[i] = 0;
	/*write_system(buf, i);*/
	printx(buf);

	return i;
}

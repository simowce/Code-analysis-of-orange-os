#include "type.h"
#include "const.h"
#include "protect.h"
#include "tty.h"
#include "console.h"
#include "proc.h"
#include "proto.h"
#include "global.h"

/*int _vsprintf(char *buf, const char *fmt, va_list arg)*/
/*{*/
	/*char *p;*/
	/*char tmp[256] = {0};*/
	/*va_list p_next_arg = arg;*/

	/*for (p=buf; *fmt; fmt++) {*/
		/*if (*fmt != '%') {*/
			/**p++ = *fmt;*/
			/*continue;*/
		/*}*/

		/*fmt++;*/
		
		/*switch (*fmt) {*/
		/*case 'x' :*/
			/*itoa(tmp, *((int *)p_next_arg));*/
			/*strcpy(p, tmp);*/
			/*p_next_arg += 4;*/
			/*p += strlen(tmp);*/
			/*break;*/
		/*case 's':*/
			/*break;*/
		/*default:*/
			/*break;*/
		/*}*/
	/*}*/
	/*return (p - buf);*/
/*}*/

PUBLIC int vsprintf(char *buf, const char *fmt, va_list arg)
{
	char *p;
	int align_num;
	char cs;
	int tmp;
	va_list next_arg = arg;
	char *q;
	char inner_buf[STR_DEFAULT_LEN] = {0};

	for (p = buf; *fmt; fmt++) {
		if (*fmt != '%') {
			*p++ = *fmt;
			continue;
		} else {
			align_num = 0;
		}

		fmt++;

		if (*fmt == '%') {
			*p++ = *fmt;
			continue;
		} else if (*fmt == '0') {
			cs = '0';
			fmt++;
		} else {
			cs = ' ';
		}

		while ((unsigned char)(*fmt) >= '0' && (unsigned char)(*fmt) <= '9') {
			align_num *= 10;
			align_num += ((unsigned char)(*fmt) - '0');
			fmt++;
		}

		q = inner_buf;
		memset(q, 0, sizeof(inner_buf));

		switch (*fmt) {
		case 'c':
			*p++ = *((char *)(next_arg));
			next_arg += 4;
			break;
		case 's':
			strcpy(q, *((char **)(next_arg)));
			q += strlen(*((char **)(next_arg)));
			next_arg += 4;
			break;
		case 'x':
			tmp = *((int *)(next_arg));
			i2a(tmp, 16, &q);
			next_arg += 4;
			break;
		case 'd':
			tmp = *((int *)(next_arg));
			
			if (tmp < 0) {
				tmp *= (-1);
				*q++ = '-';
			}

			i2a(tmp, 10, &q);
			next_arg += 4;
			break;
		default:
			break;
		}

		*q = 0;

		if (*fmt == 'c') 
			continue;

		int tmp_len = (align_num > strlen(inner_buf) ? align_num - strlen(inner_buf) : 0);

		for (tmp = 0; tmp < tmp_len; tmp++)
			*p++ = cs;

		q = inner_buf;

		while (*q) {
			*p++ = *q++;
		}
	}
	*p = 0;

	return (p - buf);
}

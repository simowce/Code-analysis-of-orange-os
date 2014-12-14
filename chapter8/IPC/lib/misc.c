#include "type.h"
#include "const.h"
#include "tty.h"
#include "console.h"
#include "protect.h"
#include "proc.h"
#include "proto.h"
#include "global.h"

PUBLIC char *i2a(int num, int base, char **pos)
{
	int remain = num % base;
	int result = num / base;

	if (result) {
		i2a(result, base, pos);
	}

	*((*pos)++) = remain >= 10 ? (remain - 10 + 'A') : (remain + '0');

	return *pos;
}

PUBLIC void spin(char *func_name)
{
	printf("\n\nspining in %s%s", func_name);

	while (1) {}
}

PUBLIC void assert_failure(char *exp, char *file, char *base_file, int line)
{
	printf("%cassert(%s) failed: file: %s, base_file: %s, line: %d",
	       MAG_CH_ASSERT,
	       exp, file, base_file, line);
	/*printf("%c%s\n", MAG_CH_ASSERT, exp);*/
	/*printf("\n\n\n\n\n%d\n", __LINE__);*/

	spin("assert_failure()");	
	/*__asm__ __volatile__("ud2");*/
}

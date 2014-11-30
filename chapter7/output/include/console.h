#define DEFAULT_CHAR_COLOR	0x07
#define SCR_UP			1
#define SCR_DN			-1
#define SCREEN_WIDTH		80
#define SCREEN_SIZE		(80 * 25)
#define NR_TAB			4

typedef struct s_console {
	unsigned int current_start_addr;
	unsigned int original_addr;
	unsigned int v_mem_limit;
	unsigned int cursor;
} CONSOLE;

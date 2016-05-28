#include <console.h>

int kern_entry()
{
	console_clear();
	
	console_write_color("Hello, OS Kernel!\n", rc_black, rc_green);
	console_write("line1\nline2\nline3\nEOS");
	
	return 0;
}
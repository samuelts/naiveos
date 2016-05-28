#include <console.h>
#include <common.h>

// VGA 的显示缓冲的起点是 0xB8000
static uint16_t *video_memory = (uint16_t *)0xB8000;

// 屏幕“光标”的坐标
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

static void move_cursor()
{
	// 屏幕是 80 字节宽
	uint16_t cursorLocation = cursor_y * 80 + cursor_x;
	
	outb(0x3D4, 14);					// 告诉 VGA 我们要设置光标的高字节
	outb(0x3D5, cursorLocation >> 8);	// 发送高 8 位
	outb(0x3D4, 15);					// 告诉 VGA 我们要设置光标的低字节
	outb(0x3D5, cursorLocation);		// 发送低 8 位
}

// 清屏
void console_clear()
{
	uint8_t attribute_byte = (0 << 4) | (15 & 0x0F);
	uint16_t blank = 0x20 | (attribute_byte << 8);
	
	for (int i = 0; i < 80 * 25; ++i)
	{
		video_memory[i] = blank;
	}
	
	cursor_x = 0;
	cursor_y = 0;
	move_cursor();
}

static void scroll()
{
	// attribute_byte 被构造出一个黑底白字的描述格式
	uint8_t attribute_byte = (0 << 4) | (15 & 0x0F);
	uint16_t blank = 0x20 | (attribute_byte << 8);	// space 是 0x20
	
	// cursor_y 到 25 的时候，就该换行了
	if (cursor_y >= 25)
	{
		// 将所有行的显示数据都复制到上一行，第一行永远消失
		for (int i = 0 * 80; i < 24 * 80; ++i)
			video_memory[i] = video_memory[i + 80];
		
		// 最后一行的数据现在填充空格，不显示任何字符
		for (int i = 24 * 80; i < 25 * 80; ++i)
			video_memory[i] = blank;
		
		cursor_y = 24;
	}
}

void console_putc_color(char c, real_color_t back, real_color_t fore)
{
	uint8_t back_color = (uint8_t)back;
	uint8_t fore_color = (uint8_t)fore;
	
	uint8_t attribute_byte = (back_color << 4) | (fore_color & 0x0F);
	uint16_t attribute = (attribute_byte << 8);
	
	// 0x08 是退格键的 ASCII 码
	/*/ 0x09 是tab 键的 ASCII 码
	if (c == 0x08 && cursor_x)
		cursor_x --;
	else if (c == 0x90)
		cursor_x = (cursor_x + 8) & ~(8-1);
	else if (c == '\r')
		cursor_x = 0;
	else if (c == '\n')
	{
		cursor_x = 0;
		cursor_y ++;
	} else if (c >= ' ')
	{
		video_memory[cursor_y * 80 + cursor_x] = c | attribute;
		cursor_x ++;
	}*/
	
	switch (c)
	{
	case (char)0x08:
		if (cursor_x != 0)
			cursor_x --;
		break;
	case (char)0x90:
		cursor_x = (cursor_x + 8) & ~(8-1);
		break;
	case '\r':
		cursor_x = 0;
		break;
	case '\n':
		cursor_x = 0;
		cursor_y ++;
		break;
	default:
		if (c >= ' ')
		{
			video_memory[cursor_y * 80 + cursor_x] = c | attribute;
			cursor_x ++;
		}
		break;
	}
	
	// 每 80 个字符一行，满 80 就必须换行了
	if (cursor_x >= 80)
	{
		cursor_x = 0;
		cursor_y ++;
	}

	// 如果需要的话滚动屏幕显示
	scroll();
	
	// 移动硬件的输入光标
	move_cursor();
}

void console_write(char *cstr)
{
	console_write_color(cstr, rc_black, rc_white);
}

void console_write_color(char *cstr, real_color_t back, real_color_t fore)
{
	while (*cstr)
		console_putc_color(*cstr++, back, fore);
}

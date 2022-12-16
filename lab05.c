#include <stdio.h>
#include <dos.h>
int main() {
	char far *buf_start = (char far *)0x0040001A;
	char far *buf_end = (char far *)0x0040001C;
	char far *physical_buf_start = (char far *)0x0040001E;
	char far *physical_buf_end = (char far *)0x0040003C;

	char far *i;

	while (1) {
		if (*buf_start == *buf_end) {
			continue;
		} else {
			for (i = (char far *)(0x00400000 + *buf_start); (char)i != *buf_end; i += 2) {
				if (i == physical_buf_end + 2) {
					i = physical_buf_start - 2;
					continue;
				}

				printf("ascii(%x)='%c' scan_code(%x)='%d'", (char)i, *i, (char)(i + 1), *(i + 1));
				if (*i == '0') {
					printf("Exiting...");
					return;
				}
				getch();
			}
			printf("\n");
		}
	}

	return 0;
}
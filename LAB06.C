#include <dos.h>
#include <stdio.h>

#define bool int
#define true 1
#define false 0

#define SECTOR_SIZE 512

#define DUMP(varname) printf("%s", #varname)

union REGS in, out;
struct SREGS sr;

int get_sections_count(const unsigned char *main_root_entry);
void read_drive(int, int, int, int, unsigned char *);

int main() {
	unsigned char buf[SECTOR_SIZE];
	int i, j, k, l, p;
	int end;
	unsigned short time, date;
	char pressed_button;

	system("cls");

	read_drive(0, 0, 1, 1, buf);
	printf("1) HDD was read.\n");

	printf("\n2) Error code: %d\n", out.h.ah);

	printf("\n3) Sections count: %d\n", get_sections_count(buf));

	printf("\n4)\n");
	for (i = 0x1BE; i < 0x1BE + 16; i++) {
		printf("%x=%02.2x; ", i - 0x1BE, buf[i]);
	}
	printf("\nbegin_head = %d\n", buf[0x1BE + 1]);
	printf("begin_sector = 1\n");
	printf("begin_cylinder = 0\n");

	printf("\n5) Type of file system = %2.02x = Win95 FAT32 (LBA); 512 Mb-2 Tb\n",
	buf[0x1BE + 4]);

	read_drive(1, 0, 1, 1, buf);
	printf("\n6)\n");
	for (i = 0; i < 0x60; i++) {
		printf("%0.2x=%0.2x;  ", i, buf[i]);
	}
	printf("\nError code: %d\n", out.h.ah);

	printf("\n7)\n");
	printf("FAT Adress = 63 + 1 + 52 - 1 = %d\n", 63 + 1 + 52 - 1);
	printf("Address kataloga = 63 + 1 + 52 + 2 * 0 - 1 = %d\n", 63+1+52+2*0-1);

	getch();

	return 0;
}

void read_drive(int head, int cylinder, int sector, int sectors_count, unsigned char *buf) {
	in.h.ah = 2;
	in.h.dl = 0x80;
	in.h.dh = head;
	in.h.ch = cylinder;
	in.h.cl = sector;
	in.h.al = sectors_count;
	sr.es = FP_SEG(buf);
	in.x.bx = FP_OFF(buf);

	int86x(0x13, &in, &out, &sr);
}

int get_sections_count(const unsigned char *main_root_entry) {
	int i, j;
	int sections_count = 0;
	bool current_section_exist;

	for (i = 0x1BE; i <= 0x1EE; i += 0x10) {
		current_section_exist = false;
		for (j = i; j < i + 0x10; j++) {
			if (main_root_entry[j] != 0) {
				current_section_exist = true;
				break;
			}
		}
		if (current_section_exist) {
			sections_count++;
		}
	}

	return sections_count;
}
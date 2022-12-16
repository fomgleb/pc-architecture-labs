#include <dos.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define bool char
#define true 1
#define false 0

#define SECTOR_SIZE 512

union REGS in, out;
struct SREGS sr;

struct s_datetime {
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
	unsigned short year;
	unsigned char month;
	unsigned char day;
};

struct s_file_data {
	unsigned char name[8];
	unsigned char extension[3];
	unsigned char attributes;
	unsigned char reserve[8];
	unsigned short cluster_high_bytes;
	unsigned short time;
	unsigned short date;
	unsigned short cluster_low_bytes;
	unsigned long size;
};

struct s_address {
	unsigned char cylinder;
	unsigned char head;
	unsigned char sector;
};

char read_drive(char, char, char, int, unsigned char *);
void print_file(struct s_file_data);
struct s_address *find_all_catalog_addresses(int *);
struct s_file_data *get_all_files_in_catalog(struct s_address, int *);
struct s_datetime words_to_datetime(unsigned short, unsigned short);

int main() {
	const unsigned short SEARCHING_YEAR = 2022;
	const unsigned char SEARCHING_MONTH = 10;
	const int FILES_ON_ONE_PAGE = 20;

	int i, j;
	char pressed_button = '1';
	struct s_address *catalog_addresses = NULL;
	int catalog_addresses_len = 0;
	struct s_file_data *all_files = NULL;
	int all_files_len;
	struct s_datetime file_datetime;
	int files_on_this_page = 0;

	system("cls");

	catalog_addresses = find_all_catalog_addresses(&catalog_addresses_len);

	for (i = 0; i < catalog_addresses_len; i++) {
		all_files = get_all_files_in_catalog(catalog_addresses[i], &all_files_len);
		for (j = 0; j < all_files_len; j++) {
			file_datetime = words_to_datetime(all_files[j].time,	all_files[j].date);
			if (file_datetime.year == SEARCHING_YEAR &&
					file_datetime.month == SEARCHING_MONTH) {
				print_file(all_files[j]);
				files_on_this_page++;
			}
			if (files_on_this_page >= FILES_ON_ONE_PAGE) {
				printf("Press any button to continue...\n");
				pressed_button = getch();
				files_on_this_page = 0;
			}
		}
		if (all_files != NULL)
			free(all_files);
	}

	getch();

	free(catalog_addresses);
	free(all_files);

	return 0;
}

struct s_file_data *get_all_files_in_catalog(struct s_address addr, int *len) {
	struct s_file_data files[16];
	struct s_file_data *all_files = NULL;
	bool found_end_of_catalog = false;
	bool is_first_iteration = true;
	int i, j;
	int c, h, s;

	*len = 0;

	for (c = 0; c < 95 && !found_end_of_catalog; c++) {
		for (h = 0; h < 255 && !found_end_of_catalog; h++) {
			for (s = 1; s < 64 && !found_end_of_catalog; s++) {
				if (is_first_iteration) {
					c = addr.cylinder;
					h = addr.head;
					s = addr.sector;
				}
				read_drive(h, c, s, 1, files);
				for (i = 0; i < 16; i++) {
					if ((files[i].attributes & 0x18) > 0 || files[i].name[0] == 0xE5) {
						continue;
					}
					for (j = 0; j < 8; j++) {
						if (files[i].name[j] == '\0' ||
								(files[i].name[j] > 'a' && files[i].name[j] < 'z')) {
							found_end_of_catalog = true;
							break;
						}
					}
					if (!found_end_of_catalog) {
					 (*len)++;
					 all_files = realloc(all_files, sizeof(struct s_file_data) * (*len));
					 all_files[*len - 1] = files[i];
					}
				}
				is_first_iteration = false;
			}
		}
	}

	return all_files;
}

struct s_address *find_all_catalog_addresses(int *catalog_addresses_len) {
	struct s_address *catalog_addresses = NULL;
	struct s_file_data files[16];
	unsigned char i, j, k;
	const int MIN_CYLINDER = 4;  /* 4 */
	const int MAX_CYLINDER = 82; /* 82 */

	printf("Progress = 000%");

	*catalog_addresses_len = 0;
	for (i = MIN_CYLINDER; i <= MAX_CYLINDER; i++) { /* 100 255 64 */
		for (j = 0; j < 255; j++) {
			for (k = 1; k < 64; k++) {
				read_drive(j, i, k, 1, files);
				if (strncmp(files[0].name, ".       ", 8) == 0 &&
						strncmp(files[1].name, "..      ", 8) == 0) {
					(*catalog_addresses_len)++;
					catalog_addresses = realloc(catalog_addresses,
						(*catalog_addresses_len) * sizeof(struct s_address));
					catalog_addresses[*catalog_addresses_len - 1].cylinder = i;
					catalog_addresses[*catalog_addresses_len - 1].head = j;
					catalog_addresses[*catalog_addresses_len - 1].sector = k;
				}
			}
		}
		printf("\b\b\b\b");
		printf("%0.3d\%", (int)((i - MIN_CYLINDER) * 100 / (MAX_CYLINDER -
			MIN_CYLINDER)));
	}

	printf("\b\b\b\b100\%\n");

	return catalog_addresses;
}

struct s_datetime words_to_datetime(unsigned short time_word,
unsigned short date_word) {
	struct s_datetime new_date = {0, 0, 0};

	new_date.hour = (time_word & 0xF800) >> 11;
	new_date.minute = (time_word & 0x07E0) >> 5;
	new_date.second = (time_word & 0x001F) * 2;
	new_date.year = ((date_word & 0xFE00) >> 9) + 1980;
	new_date.month = (date_word & 0x01E0) >> 5;
	new_date.day = date_word & 0x001F;

	return new_date;
}

void print_file(struct s_file_data file) {
	struct s_datetime date = words_to_datetime(file.time, file.date);
	int i;

	printf("n='");
	for (i = 0; i < 8; i++) {
		printf("%c", file.name[i]);
	}
	printf("' e='");
	for (i = 0; i < 3; i++) {
		printf("%c", file.extension[i]);
	}
	printf("' a='%0.2x'", file.attributes);
	printf(" c='%0.4x%0.4x'", file.cluster_high_bytes, file.cluster_low_bytes);
	printf(" t='%d.%0.2d.%0.2d", date.year, date.month, date.day);
	printf(" %0.2d:%0.2d:%0.2d'", date.hour, date.minute, date.second);
	printf(" s='%u'", file.size);

	printf("\n");
}

char read_drive(char head, char cylinder, char sector, int sectors_count,
unsigned char *buf) {
	in.h.ah = 2;
	in.h.dl = 0x80;
	in.h.dh = head;
	in.h.ch = cylinder;
	in.h.cl = sector;
	in.h.al = sectors_count;
	sr.es = FP_SEG(buf);
	in.x.bx = FP_OFF(buf);

	int86x(0x13, &in, &out, &sr);

	return out.h.ah;
}


#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
	char far *i;
	int j;
	char *second_name = "Fomenko";
	char *first_name = "Glib";

	system("cls");

	printf("Second name ascii characters:\n");
	for (j = 0; second_name[j] != '\0'; j++) {
		printf("%c=%d ", second_name[j], second_name[j]);
	}
	printf("\nFirst name ascii characters:\n");
	for (j = 0; first_name[j] != '\0'; j++) {
		printf("%c=%d ", first_name[j], first_name[j]);
	}
	printf('\n');

	/* No 3.1 */
	outportb(0x3D9, 9);

	/* No 3.2 */
	for (i = (char far *)0x00400050; i <= 0x00400050 + 8; i += 2) {
		*i = 11;
		*(i + 1) = 4;
	}

	/* No 3.3 */
	*((char far *)0x00400060) = 12;
	*((char far *)0x00400061) = 1;

	/* No 3.6 */
	printf("%s %s", second_name, first_name);

	return 0;
}
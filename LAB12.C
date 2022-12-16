#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>

int main() {
	char *input_text = malloc(300);
	char far *start_output_address = (char far *)0x00400004;
	char far *video_buffer = (char far *)0xB8000000;
	char far *current_buffer_shift;
	int row_number = 1;
	int i;
	FILE *fp;
	char pressed_button;

	system("cls");

	/* No 1 Some test values, but nothing works */
	printf("Input some text (max size is 300): ");
	fgets(input_text, 300, stdin);
	*start_output_address = 0x03;
	*(start_output_address + 1) = 0x20;
	for (i = 0; input_text[i] != '\n' && input_text[i] != '\0'; i++) {
		current_buffer_shift = video_buffer + row_number * 160 + i * 2;
		*current_buffer_shift = input_text[i];
		delay(100);
	}

	getch();

	/* No 2 */
	fp = fopen("somefile.txt", "rb");
	for (i = 0; i < 4000; i += 2) {
		*(video_buffer + i) = getc(fp);
	}

	/* No 3 */
	pressed_button = ' ';
	while (pressed_button != 'q') {
		if (pressed_button == 'w') {
			for (i = 3999; i >= 0; i--) {
				video_buffer[i + 160] = video_buffer[i];
			}
		} else if (pressed_button == 's') {
			for (i = 0; i < 4000; i++) {
				video_buffer[i - 160] = video_buffer[i];
			}
		}
		pressed_button = getch();
	}

	/* No 4 */
	outportb(0x3D4, 0x18);
	outportb(0x3D5, 7);



	getch();

	free(input_text);
	fclose(fp);

	return 0;
}
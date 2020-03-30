#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char message[0x100];
char *note[9];
void menu()
{
	printf("1. create note\n");
	printf("2. delete note\n");
	printf("3. edit note\n");
	printf("4. give up\n");
	printf("Your choice: ");
}

int main()
{
	setvbuf(stdin, 0, _IONBF, 0);
	setvbuf(stdout, 0, _IONBF, 0);

	printf("I don't want to waste your time !\n");
	char *gift = (char *)malloc(0x10);
	printf("heap @ %p\n", gift);
	printf("free @ %p\n\n", &free);

	while(1){
		menu();

		int op;
		scanf("%d", &op);
		switch(op){
			case 1:
			{
				int size;
				int index;
				printf("note size: ");
				scanf("%d", &size);
				if(size < 0 || size > 0x100){
					printf("Invalid size\n");
					break;
				}
				printf("note index: ");
				scanf("%d", &index);
				if(index < 0 || index > 8){
					printf("Invalid index\n");
					break;
				}
				if(note[index]){
					printf("note already exist\n");
					break;
				}

				note[index] = (char *)malloc(size);
				printf("note content: ");
				read(0, note[index], size);
				note[index][size-1] = 0;
				break;
			}

			case 2:
			{
				int index;
				printf("note index: ");
				scanf("%d", &index);
				if(index < 0 || index > 8 || note[index] == NULL){
					printf("Invalid index\n");
					break;
				}

				free(note[index]);
				note[index] = NULL;
				break;
			}

			case 3:
			{
				int index;
				printf("note index: ");
				scanf("%d", &index);
				if(index < 0 || index > 8 || note[index] == NULL){
					printf("Invalid index\n");
					break;
				}

				const int size = (*(int *)(note[index]-8) & ~7) - 0x10;
				printf("content:");
				read(0, note[index], size);
				break;
			}
			case 4:
			{
				int index;
				printf("note index: ");
				scanf("%d", &index);
				if(index < 0 || index > 8 || note[index] == NULL){
					printf("Invalid index\n");
					break;
				}

				const int size = (*(int *)(note[index]-8) & ~7) - 0x10;
				memcpy(message, note[index], size);
				break;
			}
			case 5:
			{
				printf("I know it's hard\n");
				printf("Maybe you can do some research about _int_malloc\n");
				return 0;
			}
		}
	}

	return 0;
}
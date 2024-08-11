#include <stdio.h>

void get_characters(char user_array[], int size)
{
	char input;
	for(int i = 0; i < size; i++)
	{
		scanf("%c", &user_array[i]);
	}
	
}
void reverse_characters(char user_array[], char reverse_array[], int size)
{
	int i,j;
	for(i = size - 1, j = 0; i >= 0; i--, j++)
	{
			reverse_array[j] = user_array[i];
	}
}
void print_characters(char user_array[], int size)
{
	for(int i = 0; i < size; i++)
	{
		printf("%c", user_array[i]);
	}
	printf("\n");
}
int main()
{
	char array[20], reverse[20];
	printf("Insert 20 consecutive characters:");
	get_characters(array, 20);
	printf("\nThe user entered the following characters:");
	print_characters(array, 20);
    reverse_characters(array, reverse, 20);
	printf("The characters in reverse order are: ");
    print_characters(reverse, 20);
    return 0;
}
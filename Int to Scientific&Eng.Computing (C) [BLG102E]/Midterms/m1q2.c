#include <stdio.h>
#include <stdbool.h>

int main()
{
	int num1, num2, num3, task, game = 0;
	bool equal = false;
		while (game >= 0)
		{
			game = 0;
			printf("Give three integers separated with space:");
			scanf("%d %d %d", &num1,&num2, &num3);
			while(game == 0)
			{
				printf("Which task would you like to perform?\n");
				printf("1 to check if the list is strictly increasing or decreasing\n");
				printf("2 to check if the list is increasing or decreasing\n");
				printf("3 to compute the maximum number in the list\n");
				printf("4 to reverse the list\n");
				printf("5 to enter a new list of integers\n");
				printf("-1 to terminate the program\n");
				scanf("%d", &task);
				if(task == 1)
				{
					if (num1 > num2 && num2 > num3)
					{
					printf("strictly decreasing\n");
					}
					else if (num1 < num2 && num2 < num3)
					{
					printf("strictly increasing\n");
					}
					else
					{
					printf("none of the two\n");
					}
					
				}
				else if(task == 2)
				{
					if(num1 == num2 && num2 == num3){
						equal = true;
					}
					if (num1 >= num2 && num2 >= num3 && !equal)
					{
					printf("decreasing\n");
					}
					else if (num1 <= num2 && num2 <= num3 && !equal)
					{
					printf("increasing\n");
					}
					else
					{
					printf("none of the two\n");
					}
					
				}
				else if(task == 3)
				{
					int max = (num1 > num2 ? num1 : num2) > num3 ? (num1 > num2 ? num1 : num2) : num3;
					printf("the maximum number is %d\n", max);
					
				}
				else if(task == 4)
				{
					printf("%d %d %d\n",num3,num2,num1);
					
				}
				else if(task == 5)
				{
					game = 1;
				}
				else if(task == -1)
				{
					return 0;
				}
				else 
				{
					printf("not a defined option, try again\n");
				}
			}
		}
	return 0;
} 
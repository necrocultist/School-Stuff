#include <stdio.h>

double alternating_function(double arr[], int choice, int size)
{
	double sum = 0.0;
	double tot = 1.0;
	int game = 0;
	while(game == 0)
	{
		if(choice == 1)
		{
			for(int i = 0; i < size; i++)
			{
				if(arr[i] == -1)
				{
					arr[i] = 0;
				}
				if((i % 2) == 0)
				{
					sum += arr[i];
				}
				else
				{
					sum -= arr[i];
				}
			}
			return sum;
		}
		else if(choice == 2)
		{
			for(int i = 0; i < size; i++)
			{
				if(arr[i] == -1)
				{
					arr[i] = 1;
				}
				if((i % 2) == 0)
				{
					tot *= arr[i];
				}
				else
				{
					tot /= arr[i];
				}
			}
			return tot;
		}
		else
		{
			game = 1;
			return 0;
		}
	}
	return 0;
}

int main()
{
	int size = 0;
	double arr[100];
    printf("Enter up to 10 non-negative integers (-1 to terminate):");

	for(int i = 0; i < size + 1; i++)
	{
		scanf("%lf", &arr[i]);
		size++;
		if(arr[i] == -1){
			break;
		}
		if(size == 10){
			break;
		}
	}
	int choice;
    printf("\nEnter your choice (1 for alternating sum or 2 for alternating division):");
    scanf("%d", &choice);
    double result = alternating_function(arr, choice, size);
    printf("The result is %.2lf\n", result);
    return 0;
}
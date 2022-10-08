#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
   if (argc < 2 || argc > 6) 
   {
    printf("Enter 1 to 5 arguments!\n");
    return 1;
   }
   
	int maleCount = 0, femaleCount = 0;
	double msum = 0, fsum = 0;
	
	for(int i = 1; i < argc; i++){
		if(argv[i][0] == 'M') {
			maleCount++;
			msum = msum + strlen(argv[i]) - 2;
		}
		else if(argv[i][0] == 'F') {
			femaleCount++;
			fsum = fsum + strlen(argv[i]) - 2;
		}
		else {
			printf("Names should start with either M_ or F_!");
			return 1;
		}
		
		if(strlen(argv[i]) < 3) {
			printf("Names should contain at least 1 character after gender prefix!");
			return 1;
		}
	}
	
	if(femaleCount != 0){
		fsum /= femaleCount;
	}
	else{
		fsum = 0;
	}
	if(maleCount != 0){
		msum /= maleCount;
	}
	else{
		msum = 0;
	}
	
   printf("Female count: %d, avg. female name length: %2.1f\n", femaleCount, fsum);
   printf("Male count: %d, avg. male name length: %2.1f\n", maleCount, msum);

   return 0;
}
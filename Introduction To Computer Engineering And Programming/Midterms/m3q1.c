#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 50

void lstrip(const char *src, char *dst) {

    int first = 0; 
    while (src[first] == ' ') {
        first++;
    }
    int i = first;
	int j = 0;
    while (src[i] != '\0') {
        dst[j] = src[i];
        i++;
		j++;
    }
    dst[j + 1] = '\0';
}

void rstrip(const char *src, char *dst) {
	int srclen = strlen(src);
	while(src[srclen - 1] == ' ') {	
		srclen--;
	}
	int i = 0;
	while(i < srclen) {
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
}

void strip(const char *src, char *dst) {
	char temp[MAXLEN];
	lstrip(src, temp);
	rstrip(temp, dst);
	
}

int main() {
    printf("enter the string: ");
    char line[MAXLEN];
    scanf("%[^\n]%*c", line);
	
	printf("enter the trim type (l, r, b): ");
    char type;
    scanf("%c", &type);
    if(type == 'l'){
        char lstripped[MAXLEN];
        lstrip(line, lstripped);
        printf("|%s|\n", lstripped);
    }
    else if(type == 'r'){
        char rstripped[MAXLEN];
        rstrip(line, rstripped);
        printf("|%s|\n", rstripped);
    }
    else if(type == 'b'){
        char stripped[MAXLEN];
        strip(line, stripped);
        printf("|%s|\n", stripped);
    }
    else{
        printf("unexpected command.");
    }
    
    return 0;
}

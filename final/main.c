#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	int i;
	long int numbytes = 0;
	struct stat fileStat;

	if (argc < 2) {
		printf("Error: expect at least one argument.\n");
	}	
	

	for (i = 1; i < argc; i++) {
		stat(argv[i], &fileStat);
		numbytes += fileStat.st_size;	
	}

	printf("Size: %ld\n", numbytes);

	return 0;
}

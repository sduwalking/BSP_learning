#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

int main(int argc, char *argv[]){
	if(argc != 3){
		printf("need more arg\n");
		return 1;
	}
	
	char buf[1024];
	char check[1];
	size_t length = 0;
	ssize_t n;
	size_t total = 0;
	int fd;
	
	fd = open(argv[1],O_RDONLY);
	if(fd == -1){
		perror("open");
		return 1;
	}
	n = read(fd,buf,1024);
	if(n == 0){
		printf("src is empty\n");
		return 1;
	}
	length = n;
	if(read(fd,check,1) != 0){
		printf("input.txt > 1024 byte");
	}
	close(fd);
	
	fd = open(argv[2],O_WRONLY | O_CREAT | O_TRUNC,0644);
	while(total != length){
		n = write(fd,buf + total,n - total);
		if(n == -1){
			if(errno == EINTR) continue;
			perror("write");
			break;
		}
		if(n == 0) break;
		
		total += (size_t)n;
	}
	close(fd);
	
	return 0;
}

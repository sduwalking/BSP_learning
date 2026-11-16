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
	ssize_t rd_n;
	int num = 0;
	int check = 0;
	int fd_rd = open(argv[1],O_RDONLY);
	if(fd_rd == -1){
		perror("open rd");
		check = 1;
		return check;
	}
	int fd_wr = open(argv[2],O_WRONLY | O_CREAT | O_TRUNC,0644);
	if(fd_wr == -1){
		perror("open wr");
		check = 1;
		close(fd_rd);
		return check;
	}
	
	while((rd_n = read(fd_rd,buf,1024)) != 0){
		if(rd_n == -1){
			if(errno == EINTR) continue;
			perror("read");
			check = 1;
			break;
		}
		
		ssize_t wr_n = write(fd_wr,buf,rd_n);
		if(wr_n == -1){
			if(errno == EINTR) continue;
			perror("write");
			check = 1;
			break;
		}
		
		if(num <= 1024){
			num = num + (int)rd_n;
			if(num > 1024){
				printf("src > 1024 byte\n");
			}
		}
	}
	
	close(fd_wr);
	close(fd_rd);
	
	return check;
}

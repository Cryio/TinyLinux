#include <unistd.h>
#include <sys/wait.h>

int asm_waitid(idtype_t idtype, __id_t id, siginfo_t *infop, int options, void*);
int main()
{	
	char command[255];
	for (;;){
		write(1, "# ", 2);
		int count = read(0, command, 255);
		
		// /bin/ls\n -> /bin/ls\0
		command[count - 1] = 0;
		pid_t fork_result = fork();
		if (fork_result == 0) {
			execve (command, 0, 0);
			break;
		} else {
			siginfo_t info;
			asm_waitid(P_ALL, 0, &info, WEXITED, 0);
		}
	}
	_exit(0);
}

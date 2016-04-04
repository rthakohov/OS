#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

int signals_count = 0;

void handler(int signal, siginfo_t *info, void *tmp) {
	if (signals_count > 0) {
		return;
	}
	
	signals_count++;
	
	if (signal == SIGUSR1) {
		printf("SIGUSR1");
	} else {
		printf("SIGUSR2");
	};
	
	printf(" from %d\n", info -> si_pid);
}

int main() {
	struct sigaction sa;
	sa.sa_sigaction = handler;
	sigaction(SIGUSR1, &sa, NULL);
	
	if (sigemptyset(&sa.sa_mask) != 0 || sigaddset(&sa.sa_mask, SIGUSR1) != 0 || sigaddset(&sa.sa_mask, SIGUSR2) != 0) {
		perror("error");
	}
	
	if (sigaction(SIGUSR1, &sa, NULL) != 0 || sigaction(SIGUSR2, &sa, NULL) != 0) {
		perror("error");
	}
	
	sleep(10);
	
	if (signals_count == 0) {
		printf("No signals were caught\n");
	}
	return 0;
}
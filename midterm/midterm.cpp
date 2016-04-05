//
//  main.cpp
//  midterm
//
//  Created by Руслан Тхакохов on 05/04/16.
//  Copyright © 2016 Руслан Тхакохов. All rights reserved.
//

#include <errno.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <signal.h>

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

const int BUF_SIZE = 1024;

int fds[100];

std::string read_line(int fd = 0) {
    std::string s = "";
    char buf[BUF_SIZE];
    bool flag = false;
    while (true) {
        ssize_t r = read(fd, buf, BUF_SIZE);
        if (r < 0) {
            perror("reading error!");
        }
        if (r == 0) {
        	exit(0);
        }	
        if (buf[r - 1] == '\n') {
        	buf[r - 1] = '\0';
        	flag = true;
        }
        s += std::string(buf);
        strcpy(buf, "");
        if (flag) {
            break;
        }
    }
    
    return s;
}

void write_line(std::string s, int fd = 1, bool flag = false) {
	if (flag) {
        s += "\n";
    }
    size_t written = 0;
    size_t nbyte = s.size();
    char buf[BUF_SIZE];
    strcpy(buf, "");
    strcpy(buf, s.c_str());
    while (written < nbyte) {
        ssize_t n = write(fd, buf + written, nbyte - written);
        if (n < 0) {
            if (errno == EINTR) {
                continue;
            }
            return perror("writing error");
        }
        written += n;
    }
}


std::vector<std::string> split(std::string s, std::string c) {
    std::vector<std::string> tokens;
    
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(c)) != std::string::npos) {
        token = s.substr(0, pos);
        tokens.push_back(token);
        s.erase(0, pos + c.size());
    }
    if (s.size()) {
        tokens.push_back(s);
    }
    return tokens;
}

void process_command(std::string command) {
	std::vector<std::string> cmd = split(command, " ");
	char **args = new char*[cmd.size() + 1];
	//std::cout << cmd.size() << std::endl;		
	for (size_t j = 0; j < cmd.size(); j++) {
		args[j] = new char[cmd[j].size() + 1];			
		std::copy(cmd[j].begin(), cmd[j].end(), args[j]);
		args[j][cmd[j].size()] = '\0'; 
		//std::cout << args[j] << std::endl;
	}
	args[cmd.size()] = NULL;
	execvp(cmd[0].c_str(), args);
		
	delete [] args;
}


void work(std::string const& cmd1, int i, int n) {
	int pid = fork();
	if (pid == 0) {
		if (i != 0) {
			dup2(fds[(i - 1) * 2], 0);
		}
		if (i + 1 != n) {
			dup2(fds[2 * i + 1], 1);
		}
      	for (int i = 0; i < 2 * (n - 1); i++) {
      		close(fds[i]);
      	}
		process_command(cmd1);
	}
}

void one_step() {
	
	std::vector<std::string> cmds = split(read_line(), "|");
	for (int i = 0; i < 2 * (cmds.size() - 1); i++) {
		pipe(fds + 2 * i);
	}
   	
   	int in = 0;
   	int out = 1;
   	for (size_t i = 0; i < cmds.size(); i++) {
   		work(cmds[i], i, cmds.size());
   	}
}

int main(int argc, const char * argv[]) {
   	while (true) {
   		one_step();
   	}
    return 0;
}

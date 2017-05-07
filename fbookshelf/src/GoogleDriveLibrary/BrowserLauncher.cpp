#include "BrowserLauncher.h"
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>

#include <iostream>

#include "AuthorisationManager.h"

void launchDefaultBrowser(const std::string& url)
{

	char * args[3];
	args[0] = (char*) "xdg-open";
	args[1] = (char*) url.c_str();
	args[2] = (char*) 0;

	pid_t pid = fork();
	if(pid == 0)
	{
		execvp("xdg-open", args);
		exit(0);
	}
	else if (pid > 0)
	{
		wait(&pid);
	} 
	else
	{
		std::cerr << "error" << std::endl;
	}
}

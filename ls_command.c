
int ls_command(char **args){
	  pid_t pid;
	  int status;
	  pid = fork();
	  if (pid == 0){	  	 
	  	if(args[1] != NULL){
	  		if (execl("/bin/ls", "ls" , args[1], (char *)0) == -1) {
		      perror("Error : ");
		    }
	  	}
	  	else{
		    if (execl("/bin/ls", "ls" , (char *)0) == -1) {
		      perror("Error : ");
		    }
		}
	    exit(EXIT_FAILURE);
	  } else if (pid < 0) {
	    perror("Error : ");
	  } else {
	    do {
	      waitpid(pid, &status, WUNTRACED);
	    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
	  }
	  return 1;
}
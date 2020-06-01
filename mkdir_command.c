int mkdir_command(char **args){
	  pid_t pid;
	  int status;
	  pid = fork();
	  if (pid == 0){	  	 
	  	if(args[2] != NULL ){
	  		if (execl("/bin/mkdir", "mkdir" , args[1] , args[2] , (char *)0) == -1) {
		      perror("Error : ");
		    }
	  	}
	  	else{
		    if (execl("/bin/mkdir", "mkdir" , args[1] , (char *)0) == -1) {
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
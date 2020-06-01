#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ls_command.c"
#include "date_command.c"
#include "cat_command.c"
#include "rm_command.c"
#include "mkdir_command.c"


int cd(char **args, char *line);
int e_exit(char **args, char *line);
int echo(char **args, char *line);
int history(char **args, char *line);
int pwd(char **args , char *line);


char *location ;

char *builtin_str[] = {
  "cd",
  "exit",
  "echo",
  "history",
  "pwd"
};

int clearHistory()
{
  FILE *fr;
  fr = fopen(location, "w+");
  fclose(fr);
  return 1;
}


int readHistory()
{
  FILE *fr;
  fr = fopen(location, "a+");
  fseek(fr,0,SEEK_SET);
  char c;
  c = fgetc(fr);
  while( c != EOF)
  {
  	printf("%c",c);
    c = fgetc(fr);      
  }
  fclose(fr);
  return 1;
}


void writeHistory(char **args)
{
  FILE *fr;
  fr = fopen(location, "a+");
  for(int i=0 ; args[i]!=NULL ; i++){
  	fprintf(fr, "%s ", args[i]);
  }
  fprintf(fr, "\n");
  fclose(fr);
}

int (*builtin_func[]) (char ** , char*) = {
  &cd,
  &e_exit,
  &echo,
  &history,
  &pwd
};

int num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

int cd(char **args, char *line)
{
  if (args[1] == NULL) {
    fprintf(stderr, "Error : expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("bash: cd: ");
    }
  }
  return 1;
}

int e_exit(char **args, char *line)
{
  return 0;
}

int echo(char **args, char *line)
{
	if(!(strcmp(args[1],"-n")==0 || strcmp(args[1],"-E")==0 || strcmp(args[1],"-e")==0)){
		for(int i=1 ; args[i]!=NULL ; i++){
			printf("%s ",args[i]);
		}
		printf("\n");
		return 1 ;
	}
	else if(strcmp(args[1],"-n")==0){
		for(int i=2 ; args[i]!=NULL ; i++){
			printf("%s ",args[i]);
		}
		return 1 ;
	}
	else{
		for(int i=2 ; args[i]!=NULL ; i++){
			printf("%s ",args[i]);
		}
		printf("\n");
		return 1 ;
	}  
}
int history(char **args, char *line)
{
	if(args[1]!=NULL && strcmp(args[1],"-c")==0){
		clearHistory();
		return 1;
	}
	else if(args[1]==NULL)
		return readHistory();   
}
int pwd(char **args, char *line){
  char *curd = getcwd(NULL,0);
  printf("\n%s\n\n",curd);
  free(curd); 
  return 1 ;
}


int launch(char **args)
{
  if(strcmp(args[0],"ls")==0){
    return ls_command(args);
  }
  else if(strcmp(args[0],"date") == 0){
  	return date_command(args);
  }
  else if(strcmp(args[0],"mkdir") == 0){
  	return mkdir_command(args);
  }
  else if(strcmp(args[0],"rm") == 0){
  	return rm_command(args);
  }
  else if(strcmp(args[0],"cat") == 0){
  	int t = cat_command(args);
  }
  else{
  	printf("UNKNOWN COMMAND");
  	return 0;
  }
  return 1;
}

int execute(char **args , char *line)
{
  int i;
  if (args[0] == NULL) {
    return 1;
  }
  for (i = 0; i < num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args,line);
    }
  }
  return launch(args);
}


void welcomeScreen(){
        printf("\n\t\t============================================\n");
        printf("\t\t               Simple C Shell\n");
        printf("\t\t--------------------------------------------\n");
        printf("\t\t              Diptanshu Mittal : \n");
        printf("\t\t============================================\n");
        printf("\n\n");
}






#define RL_BUFSIZE 1024
char *read_line(void)
{
  int bufsize = RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "Error : allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    // Read a character
    c = getchar();

    if (c == EOF) {
      exit(EXIT_SUCCESS);
    } else if (c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize) {
      bufsize += RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "Error : allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}
#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\r\n\a"
char **split_line(char *line)
{
  int bufsize = TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token, **tokens_backup;

  if (!tokens) {
    fprintf(stderr, "Error : allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += TOK_BUFSIZE;
      tokens_backup = tokens;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
    free(tokens_backup);
        fprintf(stderr, "Error : allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}






void loop(void)
{
  char *line;
  char **args;
  int status;
  do 
  {
    char *curd = getcwd(NULL,0);
    printf("diptanshu@diptanshu-G3-3579:~%s$ ",curd);
    free(curd);    
    line = read_line();
    args = split_line(line);
    writeHistory(args);
    status = execute(args,line);
    free(line);
    free(args);
  } while (status);
}


int main(int argc, char **argv)
{
	location = getcwd(NULL,0);
	strcat(location,"/history");
    welcomeScreen();
    loop();
    return EXIT_SUCCESS;
}


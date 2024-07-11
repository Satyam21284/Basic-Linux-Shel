#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<unistd.h>
#include<string.h>
#include<time.h>
#include<pthread.h>
char **split(char *string, char *seperator,int *count);
void *sysCommand(void* u);
void printdir();
char **split_strings;
int leng;
char* input;
int main(){
	char** sinput;
	int position;
	char c;
	input=malloc(sizeof(char)*1024);
	sinput=malloc(sizeof(char)*100);
	while(true){
		position=0;
		printf("\n");
		printdir(0);
		while(true){
			c=getchar();
			if(c==EOF || c=='\n'){
				input[position]='\0';
				position++;
				break;
			}
			else{
				input[position]=c;
				position++;
			}
		}

		char command1[] ="exit";
		int a=strcmp(input,command1);
		if(a==0){
			printf("exit  ");
			//char *temp1,*temp2,*temp3;
			//temp1="start";
			//temp2="Funny";
			//temp3="World";
			//execl("start",temp1,temp2,temp3,NULL);
			//printf("error");
			break;
		}
		else{
			int count_strings=0;
			split_strings=split(input, " ",&count_strings);
			char command2[]="echo";
			char command3[]="pwd";
			char command4[]="cd";
//ECHO .......................................................................
			a=strcmp(split_strings[0],command2);
			if(a==0){
				if(count_strings!=1){
					char s1[]="-e";
					if(strcmp(split_strings[1],s1)==0){
						for(int i=2;i<count_strings;i++){
							printf("%s ",split_strings[i]);
						}
					}
					else{
						for(int i=1;i<count_strings;i++){
							printf("%s ",split_strings[i]);
						}
					}
					
				}
				else{
					printf("\n ");
				}
			}
//PWD ......................................................................
			
			else if(strcmp(split_strings[0],command3)==0){
				printdir(1);
			}
//CD .........................................................................
			
			else if(strcmp(split_strings[0],command4)==0){
				if(count_strings==1){
					char *home;
					home=getenv("HOME");
					chdir(home);

				}
				else{
					if(chdir(split_strings[1])!=0){
						printf("error thanks");
					}
				}	
			}
			else{
				pthread_t tid;
				pthread_create(&tid,NULL,sysCommand,NULL);
				leng=count_strings;
				pthread_join(tid,NULL);

				
			}

		}
	}	
	
}
char **split(char *string, char *seperator,int *count){
	int len=strlen(string);
	*count=0;
	int i=0;
	while(i<len){
		while(i<len){
			if(strchr(seperator,string[i])==NULL)
				break;
			i++;
		}
		int old_i=i;
		while(i<len){
			if(strchr(seperator,string[i])!=NULL)
				break;
			i++;
		}
		if(i>old_i){
			*count=*count+1;
		}
	}
	char **strings=malloc(sizeof(char*) * *count);
	i=0;
	char buffer[1024];
	int string_index=0;
	while(i<len){
		while(i<len){
			if(strchr(seperator,string[i])==NULL)
				break;
			i++;
		}
		int j=0;
		while(i<len){
			if(strchr(seperator,string[i])!=NULL)
				break;
			buffer[j] =string[i];
			i++;
			j++;

		}
		if(j>0){
			buffer[j] ='\0';
			int to_allocate=sizeof(char)*(strlen(buffer)+1);
			strings[string_index]=malloc(to_allocate);
			strcpy(strings[string_index],buffer);
			string_index++;
		}
	}

	return strings;
}
void printdir(int i){
	char cwd[1024];
	getcwd(cwd,sizeof(cwd));
	if(i==0){
		printf("satyam@vm:~%s  ",cwd);
	}
	else{
		printf(" %s",cwd);
	}
	
}

void *sysCommand(void* u){

		char command5[]="ls";
		char command6[]="mkdir";
		char command7[]="rm";
		char command9[]="date";
		char command10[]="cat";
		if(strcmp(split_strings[0],command5)==0){
			char *path="/bin/ls";
			if(leng==1){
				execl(path,path,NULL,NULL,NULL);
			}
			if(leng==2){
				char *arg1="-a";
				if(strcmp(arg1,split_strings[1])==0){
					execl(path,path,arg1,NULL,NULL);
				}
				else{
					printf("%s %s Command not found ",split_strings[0],split_strings[1]);
				}
				
			}
			if(leng==3){
				char *arg1="-a";
				char *arg2="-s";
				if(strcmp(arg1,split_strings[1])==0){
					if(strcmp(arg2,split_strings[2])==0){
						execl(path,path,arg1,arg2,NULL);
						perror("execl");
					}
					else{
					printf("%s %s %s Command not found ",split_strings[0],split_strings[1],split_strings[2]);
				}
				}
				else{
					printf("%s %s Command not found ",split_strings[0],split_strings[1]);
				}
			}
		}
		else if(strcmp(split_strings[0],command6)==0){
			char *path1="/bin/mkdir";
			if(leng==1){
				printf("ERROR NULL FOLDER");
			}
			else{
				if(execvp(split_strings[0],split_strings)<0){
					printf("Not a system commands");
				}
				perror("execvp");
			}
		}
		else if(strcmp(split_strings[0],command7)==0){
			if(leng==1){
				printf("ERROR NULL FOLDER");
			}
			else{

				pid_t pid2;

				char** stringoo=malloc(sizeof(char)*1000);
				stringoo[0]=malloc(sizeof(char)*20);
				strcpy(stringoo[0],split_strings[0]);
				int j=1;
				while(j<leng){
					stringoo[1]=malloc(sizeof(char)*1000);
					strcpy(stringoo[1],split_strings[j]);


					pid2=fork();
					if(pid2<0){
						printf("Cant forking ");
					}
					else if(pid2==0){
						if(execvp(split_strings[0],stringoo)<0){
							printf("Not a system commands");
						}
					}
					else{
						j++;
					}
				}
				//strings[string_index]=malloc(to_allocate);
				//strcpy(strings[string_index],buffer);
				
			}
		}
		else if(strcmp(split_strings[0],command9)==0){
			time_t date;
			time(&date);
			printf("%s",ctime(&date));

		}
		else if(strcmp(split_strings[0],command10)==0){
			if(execvp(split_strings[0],split_strings)<0){
				printf("Not a system commands");
			}
			perror("execvp");
		}
		else{
			printf("%s ...... Command not found",split_strings[0]);
		}
	
	
	
}

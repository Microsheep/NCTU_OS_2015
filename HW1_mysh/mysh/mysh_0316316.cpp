#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <string>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
using namespace std;

// Define buffer len
#define username_len 100
#define current_dir_buffer_len 1000
#define argc_limit 1000

// Define colors
#define RESET         "\033[0m"
#define BOLD          "\033[1m"
#define BLACK         "\033[30m"
#define RED           "\033[31m"
#define GREEN         "\033[32m"
#define YELLOW        "\033[33m"
#define BLUE          "\033[34m"
#define MAGENTA       "\033[35m"
#define CYAN          "\033[36m"
#define WHITE         "\033[37m"

// Define Global variables
vector<int> bgid_list;
int mysh_pid, mysh_pgid;

// Define all functions
void printshell();
void hello();
void goodbye();
pair<int, vector<vector<string>>> parse(string command);
void execute(vector<vector<string>> command, int background);
void execute_pipe(vector<vector<string>> command, int background);
void CHLD_handler(int param);
void INT_handler (int param);
void TSTP_handler (int param);

// Function implementation
// Print out the shell
void printshell(){
    char username[username_len], cur_dir[current_dir_buffer_len];
    getlogin_r(username,username_len);
    getcwd(cur_dir,current_dir_buffer_len);
    cout << CYAN << username << WHITE << " in " << YELLOW << cur_dir << RESET << endl; 
    cout << GREEN << "mysh >  " << RESET;
    fflush(stdout);
}

// Print out Hello
void hello(){
    cout << RED << "========= BEGIN ==========" << RESET << endl;
    cout << CYAN << "Welcome to mysh by 0316316!" << RESET << endl; 
    return;
}

// Print out Goodbye
void goodbye(){
    cout << GREEN << "Goodbye!" << RESET << endl;
    cout << RED << "=========  END  ==========" << RESET << endl;
    exit(0);
}

pair<int, vector<vector<string>>> parse(string command){
    string input=command;
    pair<int, vector<vector<string>>> ret;
    //Check if exit
    if(input.find("exit")==0 || input.find("EXIT")==0){
        goodbye();
    }
    else{
        //Find & to see if background process
        if(input.find("&") != string::npos){
            ret.first = 1;
            input.resize(input.find("&"));
        }
        else{
            ret.first = 0;
        }
        //Parse out commands and agrvs
        vector<string> cmds;
        char* inputc = strdup(input.c_str());
        char* t_inputc = strtok(inputc,"|");
        while (t_inputc != NULL) {
            cmds.push_back(string(t_inputc));
            t_inputc = strtok(NULL, "|");
        }
        for (int a=0;a<int(cmds.size());a++){
            vector<string> args;
            char* inputcmd = strdup(cmds[a].c_str());
            char* t_inputcmd = strtok(inputcmd," ");
            while (t_inputcmd != NULL) {
                args.push_back(string(t_inputcmd));
                t_inputcmd = strtok(NULL, " ");
            }
            ret.second.push_back(args);
        }
    }
    return ret; 
}

void execute(vector<vector<string>>command, int background){
    if(command[0][0].find("cd")==0){
        if(int(command[0].size())==1){
            chdir(getenv("HOME"));
            return;
        }
        if(chdir(command[0][1].c_str())==-1){
            cout << WHITE << "-mysh: cd " << command[0][1]  << ": No such file or directory" << RESET << endl;
        }
    }
    else if(command[0][0].find("exit")==0 || command[0][0].find("EXIT")==0){
        goodbye();
    }
    else if(command[0][0].find("kill")==0){
        bgid_list.erase(remove(bgid_list.begin(), bgid_list.end(), stoi(command[0][1],nullptr,10)), bgid_list.end());
        if(kill(stoi(command[0][1],nullptr,10), SIGKILL)==-1){
            perror("E_kill: ");
        }
    }
    else if(command[0][0].find("bg")==0){
        if(kill(stoi(command[0][1],nullptr,10), SIGCONT)==-1){
            perror("E_CONT: ");
        }
    }
    else if(command[0][0].find("fg")==0){
        if(kill(stoi(command[0][1],nullptr,10), SIGCONT)==-1){
            perror("E_CONT: ");
        }
        if(tcsetpgrp(0, stoi(command[0][1],nullptr,10))==-1){
            perror("E: ");
        }
        int status;
        waitpid(stoi(command[0][1],nullptr,10), &status, WUNTRACED);   /* wait for child process terminated */
        if(tcsetpgrp(0, mysh_pid)==-1){
            perror("E: ");
        }
    }
    else{
        execute_pipe(command, background);
    }
    return;
}

void execute_pipe(vector<vector<string>> command, int background){
    int fd[int(command.size())][2];
    for(int k=0;k<int(command.size());k++){
        pid_t childpid,c_pgid;
        pipe(fd[k]);
        childpid = fork();
        if (k==0){
            c_pgid = childpid;
        }
        if (childpid < 0) {   /* cannot allocate new process */
            cout << MAGENTA << "Fork failed" << RESET << endl;
            return;
        }
        else if (childpid == 0) {   /* child process */
            cout << MAGENTA << "Command executed by pid=" << getpid()<< RESET << endl;
            if(k!=0){
                dup2(fd[k-1][0], 0); //stdin
            }
            if(k!=int(command.size())-1){
                dup2(fd[k][1], 1); //stdout
            }
            for(int f=0;f<int(command.size());f++){
                close(fd[f][0]);
                close(fd[f][1]);
            }
            const char* commandc[argc_limit]={0};
            for(int a=0;a<int(command[k].size());a++){
                commandc[a]=command[k][a].c_str();
            }
            if(execvp(command[k][0].c_str(),(char* const*)commandc) == -1){
                cout << WHITE << "-mysh: Command not found " << command[k][0] << RESET << endl;
            }
            exit(0);
        }
        else {   /* parent process */
            pid_t pid = getpid();
            // set pgid
            setpgid(childpid, c_pgid);
            if(k==0){
                if(background==1){
                    bgid_list.push_back(c_pgid); 
                }
                else{
                    if(tcsetpgrp(0, c_pgid)==-1){
                        perror("E: ");
                    }
                }
            }
            if(k==int(command.size())-1){
                for(int d=0;d<int(command.size());d++){
                    close(fd[d][0]);
                    close(fd[d][1]);
                }
                if(background!=1){
                    for(int w=0;w<int(command.size());w++){
                        int status;
                        waitpid(-c_pgid, &status, WUNTRACED);   /* wait for child process terminated */
                    }
                    if(tcsetpgrp(0, pid)==-1){
                        perror("E: ");
                    }
                }
            }
        } 
    }
    return;
}

void CHLD_handler (int param){
    int status;
    while(waitpid(-1, &status, WNOHANG)>0){};
    return;
}

void INT_handler (int param){
    printshell();
    return;
}

void TSTP_handler (int param){
    printshell();
    return;
}

int main(int argc, char** argv){
    //Know who am I
    mysh_pgid = getpgid(mysh_pid=getpid());
    //Handle signals
    signal (SIGTTIN, SIG_IGN);
    signal (SIGTTOU, SIG_IGN);
    signal (SIGCHLD, CHLD_handler);
    signal (SIGINT, INT_handler);
    signal (SIGTSTP, TSTP_handler);
    //Print out hello message
    hello();
    while(1){
        //Print out shell messagr
        printshell();
        //Get the command
        string command="";
        getline(cin, command);
        //Parse the command
        pair<int, vector< vector<string> > > output;
        output = parse(command);
        //If there is no real command (a.k.a only press enter) skip it
        if(output.second.size()==0){
            continue;
        }
        //Execute command
        execute(output.second, output.first);
    }
    return 0;
}

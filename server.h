#ifndef MY_UNIQUE_INCLUDE_NAME_H
#define MY_UNIQUE_INCLUDE_NAME_H

// All content here.
    #include <iostream>
    #include <unistd.h>
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <netinet/in.h>
    #include <signal.h>
    #include <stdlib.h>
    #include <string.h>
    #include <sys/select.h>
    #include <vector>
    #include "helper.h"
#endif

#define PASSWORD "MLSC2023"
#define BUFFER_SIZE 2048

using namespace std;

class Client
{
    public : 
        string name; int fd, uid, admin;

    public : 

        //constructors
        Client(int Fd, int Uid, char* Name);
        Client(int Fd, int Uid);

        //basic functionality functions 
        void send_message(char* msg, int* clients_fd, int size);
        void setName(char* s)
                {
                    name = s;
                }
        //command based functions 
        void handle_commands(char* cmd);
        void leave_client();
        void send_message_private(char* msg, int fd, int name_length);
        void change_name(char* name);
        void make_admin(char* pwd);
};

//function definations    

void Client::send_message_private(char* msg, int fd, int name_length)
{
    if(fd!=-1)
    {
        cout<<"Message from "<<name<<" : "<<msg<<endl;
        write(fd, &msg[name_length+1], strlen(msg));
    }
    else
    {
        sprintf(msg,"Name not Found\n");
        write(this->fd, msg, strlen(msg));
    }
}

Client::Client(int Fd, int Uid)
{
    fd = Fd;
    uid = Uid;
    admin  = 0;
}

Client::Client(int Fd, int Uid, char* Name)
{
    fd = Fd;
    uid = Uid;
    name = Name;
    admin = 0;
}

void Client::send_message(char* msg, int* clients_fd, int size)
{
    sprintf(msg, "%s\n", msg);
    for(int i = 0; i<size; i++)
    {
        if(clients_fd[i] == fd || clients_fd[i] == 0)
        {
            continue;
        }
        else
        {
            write(clients_fd[i], msg, strlen(msg));
        }
    }
}

void Client::change_name(char* name)
{
    char buffer[BUFFER_SIZE];
    if((int)name[0] != 0)
    {
        this->name = name;
    }
    else{
        bzero(buffer, BUFFER_SIZE);
        char n[this->name.length()+1];
        string_to_char(n, this->name);
        sprintf(buffer, "Name : %s\n", n);
        write(fd, buffer, strlen(buffer));
    }
}

void Client::make_admin(char* msg)
{
    char buffer[BUFFER_SIZE];
    char pwd[strlen(PASSWORD)+1];
    string_to_char(pwd, PASSWORD);
    if(strcmp(pwd,msg) == 0)
    {
        bzero(buffer, BUFFER_SIZE);
        sprintf(buffer, "You are now an Admin\n");
        write(fd, buffer, strlen(buffer));
        admin = 1;
    }
    else 
    {
        bzero(buffer, BUFFER_SIZE);
        sprintf(buffer, "Wrong Password\n");
        write(fd, buffer, strlen(buffer));
    }
}


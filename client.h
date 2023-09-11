#ifndef STANDARD_HEADER_FILES
#define STANDARD_HEADER FILES

// header files
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
#endif

#ifndef CUSTOM_HEADER
#define CUSTOM_HEADER
    #include "helper.h"
    #include "message.h"
#endif


#define PASSWORD "69420"
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
        void send_message(Message* msg, int* clients_fd, int size);
        void setName(char* s)
                {
                    name = s;
                }
        //command based functions 
        void send_message_private(Message* msg, Client* cli);
        void change_name(char* name);
        void make_admin(char* pwd);
        int admin_check();

};

//function definations    

int Client::admin_check()
{
    if(admin == 1)
    {
        return 1;
    }
    char msg[BUFFER_SIZE];
    sprintf(msg, "You are not an Admin\n");
    write(fd, msg, strlen(msg));
    return 0;    
}

void Client::send_message_private(Message* msg, Client* cli)
{
    if(cli!=NULL)
    {
        cout<<"Private Message from "<<msg->return_sender()<<" to "<<msg->return_rec()<<endl;
        write(cli->fd, msg, sizeof(msg));
    }
    else
    {
        char buffer[BUFFER_SIZE];
        sprintf(buffer,"Name not Found\n");
        write(this->fd, buffer, strlen(buffer));
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

void Client::send_message(Message* msg, int* clients_fd, int size)
{
    for(int i = 0; i<size; i++)
    {
        if(clients_fd[i] == fd || clients_fd[i] == 0)
        {
            continue;
        }
        else
        {
            write(clients_fd[i], msg, sizeof(msg));
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


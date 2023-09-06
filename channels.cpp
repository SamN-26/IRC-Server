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
    #include "server.h"
#endif

using namespace std;

#define MAX_CLIENTS 20
#define BUFFER_SIZE 2048
#define NAME_LENGTH 32
#define CLIENTS_AT_A_TIME 10

//global variables 
class Channels;
vector<Channels*> channels;
char buffer[BUFFER_SIZE];

class Channels
{
    //data members
    int server_fd, max_fd, current_client, activity, portno, opt, uid;
    socklen_t addrlen;
    char* name;
    struct sockaddr_in address;
    vector<Client*> Client_pointers;
    int clients_fd[MAX_CLIENTS];
    fd_set fdSet;


    public : 
        Channels(char* name);
        void create_channel(int port);
        int accept_client();
        void channel_listen();
        void delete_channel();
        Channels* search_channel_by_name(char* name);
        void form_client(int new_client);
        void check_unique_name(int new_client);
        void reset_fd_set();
        int search_by_fd(int fd);
        void check_clients();
        void leave_client(Client* cli, int iter);
        int search_client_by_name(string name);
        void handle_commands(char* msg, Client* cli);
};

//functions

void Channels::handle_commands(char* msg, Client* cli)
{
    char cmd[20];
    extract(msg, cmd);
    if(strcmp(cmd,"pm") == 0)
    {
        char name[NAME_LENGTH];
        extract(msg, name);
        cli->send_message_private(&msg[3], search_client_by_name(name), strlen(name));
    }
    else if(strcmp(cmd, "name") == 0)
    {   
        cli->change_name(&msg[5]);
    }
    else if(strcmp(cmd, "admin") == 0)
    {
        cli->make_admin(&msg[6]);
    }
}

void Channels::leave_client(Client* cli, int ind)
{
    cout<<"client "<<cli->name<<" closing connection\n";
    FD_CLR(cli->fd, &fdSet);
    close(cli->fd);
    Client_pointers[ind] = NULL;
    delete(cli);
}

int Channels::search_client_by_name(string name)
{
    for(int i = 0; i<Client_pointers.size(); i++)
    {
        if(Client_pointers[i]->fd == 0)
        {
            continue;
        }
        else
        {
            if(name.compare(Client_pointers[i]->name) == 0)
            {
                return Client_pointers[i]->fd;
            }
        }
    }
    return -1;
}

void Channels::check_clients()
{
    for(int i = 0; i<Client_pointers.size(); i++)
    {
        cout<<Client_pointers[i]->name<<endl;
    }
}

int Channels::search_by_fd(int fd)
{
    int i = 0;

    for( ; i < Client_pointers.size(); i++)
    {
        if(fd == Client_pointers[i]->fd)
        {
            return i;
        }
    }
    return i;
}   

void check(int check, char *msg)
{
    if(check < 0)
    {
        perror(msg);
    }
}

Channels::Channels(char* Name)
{
    name = Name;
}

void Channels::create_channel(int port)
{
    opt = 1;
    portno = port;
    //setting up server properties
    address.sin_port = htons(portno);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    addrlen = sizeof(address);

    //initialising client file descriptors to 0
    for(int i = 0; i<MAX_CLIENTS; i++)
    {
        clients_fd[i] = 0;
    }

    //creating server socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd == 0)
    {
        perror("Socket Creation Failed\n");
    }

    //enabling server socket to resuse address
    sprintf(buffer, "setsocket Failed\n");
    check(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)), buffer);
    bzero(buffer, BUFFER_SIZE);
    //binding server
    sprintf(buffer, "Bind failed\n");
    check( bind(server_fd, (struct sockaddr*)&address, addrlen) , buffer);
    bzero(buffer, BUFFER_SIZE);
    //listening for new connections
    sprintf(buffer, "listen failed\n");
    check( listen(server_fd, CLIENTS_AT_A_TIME), buffer);
    bzero(buffer, BUFFER_SIZE);
    cout<<"Server listening on Port "<<portno<<"\n";

    //clearing the socket file descriptors
    FD_ZERO(&fdSet);
    FD_SET(server_fd, &fdSet);

}

int Channels::accept_client()
{
    sprintf(buffer, "Welcome to the Server\n");
    int new_client = accept(server_fd, (struct sockaddr*)&address, &addrlen);
    if(new_client == 0)
    {
        cout<<"Accept Failed\n";
        exit(1);
    }
    cout<<"Client "<<new_client<<" added\n" ;
    write(new_client, buffer, strlen(buffer));
    for(int i = 0; i<MAX_CLIENTS; i++)
    {
        if(clients_fd[i] == 0)
        {
            clients_fd[i] = new_client;
            break;
        }
    }
    return new_client;
}

void Channels::check_unique_name(int new_client)
{
    int i = 0;
    name:
    sprintf(buffer, "Enter a Name : ");
    write(new_client, buffer, strlen(buffer));
    bzero(buffer, BUFFER_SIZE);
    if(read(new_client, buffer, BUFFER_SIZE) == 0)
    {
        cout<<"error\n";
    }

    cout<<buffer<<endl;
    for( ; i<Client_pointers.size(); i++)
    {
        if(&Client_pointers[i] == NULL)
        {
            continue;
        }   
        else
        {
            string s = Client_pointers[i]->name;
            if(s.compare(buffer) == 0)
            {
                sprintf(buffer, "Name Already Taken\n");
                write(new_client, buffer, strlen(buffer));
                goto name;
            }
        }
    }
}

void Channels::reset_fd_set()
{
    FD_ZERO(&fdSet);
    FD_SET(server_fd, &fdSet);
    
    for(int i = 0; i<MAX_CLIENTS; i++)
    {
        if(clients_fd[i] == 0)
        {
            continue;
        }
        FD_SET(clients_fd[i], &fdSet);
    }
}

void Channels::form_client(int new_client)
{
    int i = search_by_fd(0);     
    if(i == Client_pointers.size())
    {
        cout<<"pushed new client\n";
        Client_pointers.push_back(new Client(new_client, ++uid, buffer));
    }
    else
    {
        cout<<"used client renamed\n";
        Client_pointers[i] =  new Client(new_client, ++uid, buffer);
        cout<<Client_pointers[i]->name<<endl;
        cout<<Client_pointers[i]->uid;
    }
    bzero(buffer, BUFFER_SIZE);
}

void trim_buffer(char *msg, int length)
{
    for(int i = 0; i<length; i++)
    {
        if(msg[i] == '\n' || msg[i] == '\r')
        {
            msg[i] = '\0';
            break;
        }
    }
}

void Channels::channel_listen()
{
    while(1)
    {
        bzero(buffer, BUFFER_SIZE);
        max_fd = server_fd;

        //finding max file descriptor
        for(int i = 0; i<MAX_CLIENTS; i++)
        {
            // if(sd > 0)
            // {
            //     FD_SET(sd, &fdSet);
            // }
            //finding maximum file descriptor
            if(clients_fd[i] > max_fd)
            {
                max_fd = clients_fd[i];
            }
        }

        //printf("Waiting for activity\n");
        reset_fd_set();
        activity = select(max_fd + 1, &fdSet, NULL, NULL, NULL);

        sprintf(buffer, "Select Failed\n");
        check(activity, buffer);
        bzero(buffer, BUFFER_SIZE);

        if(activity == 0)
        {
            continue;
        }

        else if(FD_ISSET(server_fd, &fdSet))
        {
            //accept client
            int new_client = accept_client();

            //Adding new Client
            check_unique_name(new_client);

            //Forming new Client Object
            form_client(new_client);
        }

        else
        {
            for(int i = 0; i<MAX_CLIENTS; i++)
            {
                if(clients_fd[i] == 0)
                {
                    continue;
                }
                else if(FD_ISSET(clients_fd[i], &fdSet))
                {
                    //searching for active client
                    int ind = search_by_fd(clients_fd[i]);
                    Client* cli;
                    cli = Client_pointers[ind];
                    if(ind == Client_pointers.size())
                    {
                        cout<<"error fd not found\n";
                        continue;
                    }
                    
                    if(read(clients_fd[i], buffer, BUFFER_SIZE) == 0)
                    {
                        leave_client(cli, ind);
                        clients_fd[i] = 0;
                        break;
                    }
                    trim_buffer(buffer, strlen(buffer));
                    cout<<buffer<<endl;
                    if(strcmp(buffer, "/leave") == 0)
                    {
                        leave_client(cli, ind);
                        clients_fd[i] = 0;
                        break;
                    }
                    else if(buffer[0] == '/')
                    {
                        handle_commands(&buffer[1], cli);
                    }
                    else
                    {
                        cli->send_message(buffer, clients_fd, MAX_CLIENTS);
                        bzero(buffer, BUFFER_SIZE);
                        break;
                    }
                }
            }
        }
    }
}

//Main
int main(int argc, char* argv[])
{
    channels.push_back(new Channels("General"));
    channels[0]->create_channel(atoi(argv[1]));
    channels[0]->channel_listen();
}
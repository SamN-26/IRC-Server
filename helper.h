#include <iostream>
#include <stdlib.h>
#include <string.h>

using namespace std;

void trim(char* ch)
{
    for(int i = 0; i<strlen(ch); i++)
    {
        if(ch[i] == ' ' || ch[i] == '\n' )
        {
            ch[i] = '\0';
            break;
        }
    }
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

void check(int check, char *msg)
{
    if(check < 0)
    {
        perror(msg);
    }
}

void string_to_char(char* ch, string s)
{   
    int i = 0;
    for(; i<s.length(); i++)
    {
        ch[i] = s[i];
    }
    ch[i] = '\0';
}

void extract(char* msg, char* cmd)
{
    int i = 0;
    for(; i<strlen(msg); i++)
    {
        if(msg[i] == ' ')
        {
            break;
        }
        cmd[i] = msg[i];
    }
    cmd[i] = '\0';
}
#include <iostream>
#include <stdlib.h>
#include <string.h>

using namespace std;

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
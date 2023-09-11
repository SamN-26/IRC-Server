#ifndef CUSTOM_HEADER
#define CUSTOM_HEADER
    #include "helper.h"
    #include "message.h"
#endif

class Message
{
    string rec, sender;
    char* msg;

    public : 
        Message(char* msg, string rec, string sender);
        Message(char* msg, string sender);
        Message(char* msg);

        //data member access functions
        char* return_msg()
        {
            return msg;
        }
        string return_sender()
        {
            return sender;
        }
        string return_rec()
        {
            return rec;
        }
};

Message::Message(char* Msg)
{
    msg = Msg;
}

Message::Message(char* Msg, string Sender)
{
    msg = Msg;
    sender = Sender;
}

Message::Message(char* Msg, string Rec, string Sender)
{
    msg = Msg;
    rec = Rec;
    sender = Sender;
}
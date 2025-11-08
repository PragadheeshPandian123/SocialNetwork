#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <ctime>
using namespace std;

struct Message {
    int senderId;
    int receiverId;
    string senderName;
    string content;
    string timestamp;
    
    Message(int sid = 0, int rid = 0, string sname = "", string cont = "", string ts = "")
        : senderId(sid), receiverId(rid), senderName(sname), content(cont), timestamp(ts) {}
};

#endif
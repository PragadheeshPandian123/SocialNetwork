#ifndef ACTIVITYNODE_H
#define ACTIVITYNODE_H

#include <string>
using namespace std;

class ActivityNode {
public:
    string activity;
    string timestamp;
    ActivityNode* next;
    
    ActivityNode(string act, string ts) 
        : activity(act), timestamp(ts), next(nullptr) {}
};

#endif
//
// Created by os on 7/30/23.
//

#ifndef PROJECT_BASE_V1_1_SEM_H
#define PROJECT_BASE_V1_1_SEM_H

#include "../h/TCB.h"

class Sem {

private:
    int val, numBlocked;
    bool closed;

    Blocked *head, *tail;

public:


    static int semOpen(Sem** handle, unsigned init);
    static int semClose(Sem* handle);
    static int semWait(Sem* id);
    static int semSignal(Sem* id);
    bool isClosed(){return closed;}
    void Close(bool _closed){this->closed=_closed;}


    static int semTryWait(Sem* id);

protected:

    void putBlocked(TCB* tcb);
    TCB* getBlocked();

};



#endif //PROJECT_BASE_V1_1_SEM_H

/*********************************************************************
 * $Author: admin $
 *
 * $LastChangedBy: admin $
 *
 * $Revision: 2577 $
 *
 * $LastChangedDate: 2011-03-09 09:50:55 +0800 (Wed, 09 Mar 2011) $
 *
 * $Id: spliter.h 2577 2011-03-09 01:50:55Z admin $
 *
 * $Brief: ini file parser $
 ********************************************************************/

#ifndef SPLITM_SPLITER_H_
#define SPLITM_SPLITER_H_

#include<sys/stat.h>
#include<dirent.h>
#include<cstdio>
#include<cstring>
#include<vector>
#include<algorithm>
#include<queue>
#include<sys/time.h>
#include<unistd.h>
#include<pthread.h>

using namespace std;

//const char* TMPDIR = "./tmpdir";

class Item
{
public:

    Item(int _index,int _value):index(_index),value(_value) {}
    virtual ~Item() {}
    int index;
    int value;

    bool operator < (const Item & other) const
    {
        return value>other.value;
    }
};
class spliter
{
public:
    static const int FILENUM = 20;
    static const int MAXFILELENGTH = 256;
    static const char* TMPDIR;
public:
    spliter() {}
    virtual ~spliter() {}
    int splitFile(const char*);
    int doSort();
    int doMultipleSort();
    int doMerge(const char*);
    char outPath[FILENUM][MAXFILELENGTH];
private:
    static void* singleSort(void*);
    int createDir(const char*);
    FILE *ifp,*ofp[FILENUM];
    vector<int> vec;
    pthread_t task[FILENUM];
};

struct Arg
{
    spliter *ts;
    int index;
};
//const char* spliter::TMPDIR = "./tmpdir";

#endif //SPLITM_SPLITER_H_

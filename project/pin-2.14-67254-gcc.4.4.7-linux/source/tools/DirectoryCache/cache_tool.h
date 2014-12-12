#include <math.h>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <bitset>


#define MAX_SETS    256

enum MSI_STATE {
    M,
    E,
    S,
    I
};

struct coreState {
    int core;
    MSI_STATE s;
};

class dirEntry {
    public:
        //OS_THREAD_ID    tid;
        std::vector<coreState>     coreStates;
        int                         *mref;

        dirEntry(int cid, int *a){
            mref = a;
            coreState s;
            s.core = cid;
            s.s = E;
            coreStates.push_back(s);
        }

        bool operator==(const dirEntry &other){
            return mref == other.mref;
        } 

        void prettyPrint()  {
            //std::cout << "tid: " << tid << "\tmref: " << mref << "\ts: " << s << std::endl;
        }   
};


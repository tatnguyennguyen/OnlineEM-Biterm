/**
 * @author  Nguyen Tat Nguyen
 * @date    6/2016
 */

#ifndef _BITERM_GEN_H_
#define _BITERM_GEN_H_

#include "Biterm.h"
#include "Corpus.h"
#include <vector>
using namespace std;

class BitermGen {
public:
    BitermGen(const Corpus& _corpus);
    vector<Biterm> getBiterms(int n);
    void reset()
    {
        d = 0;
        f = -1;
        s = -1;
    }
    unsigned long nBiterms() { return _nBiterms; }
    int setWindowSize(int w);
    int getWindowSize() { return windowSize; }
private:
    unsigned long countBiterm();
    const Corpus& corpus;
    int nDocs;
    unsigned long _nBiterms;
    int windowSize;
    int d, f, s;
};

#endif

/**
 * @author  Nguyen Tat Nguyen
 * @date    6/2016
 */

#include "BitermGen.h"
#include <iostream>
#include <stdexcept>

using namespace std;

BitermGen::BitermGen(const Corpus& _corpus)
    : corpus(_corpus)
{
    nDocs = corpus.nDocs();
    if (nDocs == 0) {
        cerr << "Corpus is empty\n";
        throw runtime_error("Corpus is empty");
    }
    d = 0;
    f = -1;
    s = -1;
    windowSize = 14;
    _nBiterms = countBiterm();
}

vector<Biterm> BitermGen::getBiterms(int n)
{
    int count = 0;
    vector<Biterm> ret;
    while (true) {
        if (d >= nDocs) {
            d = 0;
        }
        const vector<int>& doc = corpus.getDoc(d);
        int length = doc.size();
        if (f == -1) {
            f = 0;
        }
        for (; f < length - 1; f++) {
            if (s == -1) {
                s = f + 1;
            }
            int end = std::min(length, f + windowSize);
            for (; s < end; s++) {
                ret.push_back(Biterm(doc[f], doc[s]));
                count++;
                if (count == n) {
                    s++;
                    return ret;
                }
            }
            s = -1;
        }
        f = -1;
        d++; //to the next doc
    }
}

int BitermGen::setWindowSize(int w)
{
    if (w <= 0) {
        throw invalid_argument("windowSize must be positive");
    }
    windowSize = w;
    _nBiterms = countBiterm();
}

unsigned long BitermGen::countBiterm()
{
    unsigned long ret = 0;
    for (int i = 0; i < nDocs; i++) {
        int docSize = corpus.getDoc(i).size();
        if (docSize <= windowSize) {
            ret += docSize * (docSize - 1) / 2;
        } else {
            ret += windowSize * (windowSize - 1) / 2 + (docSize - windowSize) * (windowSize - 1);
        }
    }
    return ret;
}

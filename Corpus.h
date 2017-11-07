/**
 * @author  Nguyen Tat Nguyen
 * @date    6/2016
 */

#ifndef _CORPUS_H_
#define _CORPUS_H_

#include <map>
#include <string>
#include <vector>

using namespace std;

class Corpus {
public:
    Corpus(string docs, string vocab);
    int nDocs() const { return docs.size(); }
    int vocabSize() const { return _vocabSize; }
    const vector<int>& getDoc(int i) const { return docs[i]; }
    vector<string> getTerms(const vector<int>& ids) const;

private:
    int _vocabSize;
    vector<vector<int> > docs;
    map<int, string> id2String;
};

#endif

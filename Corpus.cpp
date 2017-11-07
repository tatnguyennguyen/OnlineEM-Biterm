/**
 * @author  Nguyen Tat Nguyen
 * @date    6/2016
 */

#include "Corpus.h"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>

Corpus::Corpus(string docsFileName, string vocabFileName)
{
    ifstream docFile(docsFileName);
    ifstream vocabFile(vocabFileName);

    if (!docFile || !vocabFile) {
        cerr << "Can not read copus: docsFile=" << docsFileName << " vocabFile=" << vocabFileName << endl;
        throw runtime_error("Can not read copus");
    }

    int id;
    string term;

    _vocabSize = 0;

    while (vocabFile >> id >> term) {
        _vocabSize++;
        id2String[id] = term;
    }
//    cout << "Vocabulary size: " << _vocabSize << endl;

    string line;
    while (getline(docFile, line)) {
        stringstream ss(line);
        vector<int> doc;
        while (ss >> id) {
            doc.push_back(id);
        }
        docs.push_back(doc);
    }
//    cout << "There are " << docs.size() << " docs\n";

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(docs.begin(), docs.end(), std::default_random_engine(seed));

    docFile.close();
    vocabFile.close();
}

vector<string> Corpus::getTerms(const vector<int>& ids) const
{
    vector<string> res;
    for (int i : ids) {
        res.push_back(id2String.at(i));
    }
    return res;
}

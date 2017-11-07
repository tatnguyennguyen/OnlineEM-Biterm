/**
 * @author  Nguyen Tat Nguyen
 * @date    6/2016
 */

#include "Corpus.h"
#include "OnlineBiterm.h"
#include <fstream>
#include <iostream>
#include <map>
#include <string>

using namespace std;

int main(int argc, char** argv)
{
    if (argc != 3) {
        cout << "Usage: ./query vocabFile modelName\n";
        return 1;
    }

    cout << "Loading model\n";
    OnlineBiterm model(argv[2]);

    map<string, int> string2id;
    map<int, string> id2string;

    fstream vocabFile(argv[1]);

    int id;
    string word;
    while (vocabFile >> id >> word) {
        string2id[word] = id;
        id2string[id] = word;
    }

    mat phi = model.getPhi();
    int W = model.getW();

    while (1) {
        string s;
        int id;
        cout << "Enter word: ";
        cin >> s;
        if (string2id.find(s) == string2id.end()) {
            cout << "Not found!\n";
            continue;
        }
        id = string2id[s];

        vec distance(W);

        for (int i = 0; i < W; i++) {
            distance(i) = norm(phi.row(i) - phi.row(id));
        }

        uvec index = sort_index(distance);

        for (int i = 0; i < 10; i++) {
            cout << id2string[index(i)] << " " << distance(index(i)) << endl;
        }
    }
}

/**
 * @author  Nguyen Tat Nguyen
 * @date    6/2016
 */

#include "BitermGen.h"
#include "Corpus.h"
#include "OnlineBiterm.h"
#include "measure.h"
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char** argv)
{
    if (argc != 10) {
        cerr << "Usage: ./oembt trainDoc vocabFile alpha beta batchSize stepPower nTopic modelName nStep\n";
        return 1;
    }

    const string trainDoc = argv[1];
    const string vocabFile = argv[2];
    const double alpha = strtod(argv[3], NULL);
    const double beta = strtod(argv[4], NULL);
    ;
    const int batchSize = strtol(argv[5], NULL, 10);
    const double stepPower = strtod(argv[6], NULL);
    const int K = strtol(argv[7], NULL, 10);
    const string modelName = argv[8];
    int nStep = strtol(argv[9], NULL, 10);

    if (batchSize <= 0) {
        cerr << "Batch size must be positive\n";
    }

    if (K <= 0) {
        cerr << "Number of topic must be positive\n";
    }

    Corpus corpus(trainDoc, vocabFile);
    BitermGen gen(corpus);
    const int W = corpus.vocabSize();
    OnlineBiterm model(K, W);

    model.setAlpha(alpha);
    model.setBeta(beta);
    model.setStepPower(stepPower);

    if (nStep == 0) {
        nStep = gen.nBiterms() / batchSize; //iterate over corpus one time
    }

    int residual = gen.nBiterms() % batchSize;

    cout << "nDoc = " << corpus.nDocs() << endl;
    cout << "nVocab = " << W << endl;
    cout << "nBiterm = " << gen.nBiterms() << endl;
    cout << "nTopic = " << K << endl;
    cout << "alpha = " << alpha << endl;
    cout << "beta = " << beta << endl;
    cout << "stepPower = " << stepPower << endl;
    cout << "batchSize = " << batchSize << endl;
    cout << "nStep = " << nStep << endl;

    for (int i = 0; i < nStep; i++) {
        cout << "\rStep " << i << flush;
        if (i + 1 == nStep) { //last batch
            model.fitMiniBatch(gen.getBiterms(batchSize + residual));
        } else {
            model.fitMiniBatch(gen.getBiterms(batchSize));
        }
    }

    // Print top words
    for (int i = 0; i < K; i++) {
        uvec index = sort_index(model.getPhi().col(i), "descend");
        uvec tid = index.subvec(0, 9);
        vector<string> top = corpus.getTerms(conv_to<vector<int> >::from(tid));
        cout << "======= Topic " << i << " =======\n";
        for (string s : top) {
            cout << s << endl;
        }
    }

    cout << "Save model to " << modelName << endl;
    model.saveModel(modelName);

    // NPMI
    cout << NPMI(model.getPhi(), corpus, 10);

    return 0;
}

//    NewYorkTimes Titles
//    alpha = 2.0 / K
//    beta = 1e-10; (1e-7 cho kết quả tốt hơn, 1e-10 cho an toàn)
//    batchSize = 10000;
//    stepPower = 0.7;
//
//    Tweeter
//    alpha = 2.0 / K
//    beta = 1e-10; (an toàn)
//    batchSize = 20000;
//    stepPower = 0.7;
//
//    CHỌN MINIBATCH QUÁ NHỎ THUẬT TOÁN SẼ CHẠY RẤT CHẬM

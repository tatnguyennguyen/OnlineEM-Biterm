/**
 * @author  Nguyen Tat Nguyen
 * @date    6/2016
 */

#include "measure.h"
#include <vector>

using namespace std;
using namespace arma;

double NPMI(const mat& phi, const Corpus& corpus, int nTopWord)
{

    const int K = phi.n_cols;
    const int W = phi.n_rows;
    const int nDocs = corpus.nDocs();
    unsigned long nTerms = 0;
    unsigned long nBiterms = 0;
    vec termProb(W, fill::zeros);
    double ret = 0;

    for (int d = 0; d < nDocs; d++) {
        const vector<int>& doc = corpus.getDoc(d);
        int docSize = doc.size();
        nBiterms += docSize * (docSize - 1) / 2;
        nTerms += docSize;
        for (int i = 0; i < docSize; i++) {
            termProb(doc[i])++;
        }
    }
    termProb /= nTerms;

    for (int k = 0; k < K; k++) {
        vector<int> mark(W, -1);
        uvec index = sort_index(phi.col(k), "descend");
        uvec topid = index.subvec(0, nTopWord - 1);

        for (int i = 0; i < nTopWord; i++) {
            mark[topid(i)] = i;
        }

        mat bitermProb(nTopWord, nTopWord, fill::zeros);

        for (int d = 0; d < nDocs; d++) {
            const vector<int>& doc = corpus.getDoc(d);
            int docSize = doc.size();
            for (int f = 0; f < docSize - 1; f++) {
                for (int s = f + 1; s < docSize; s++) {
                    if ((mark[doc[f]] != -1) && (mark[doc[s]] != -1)) {
                        bitermProb(mark[doc[f]], mark[doc[s]])++;
                        bitermProb(mark[doc[s]], mark[doc[f]])++;
                    }
                }
            }
        }

        // smoothing
        bitermProb += ones(nTopWord, nTopWord) / (double(W) * W);
        bitermProb /= (nBiterms + 1);

        vec p_i = termProb.elem(topid);

        mat temp = -log(bitermProb / (p_i * p_i.t())) / log(bitermProb);
        ret += (accu(temp) - trace(temp)) / (nTopWord - 1) / nTopWord;
    }

    return ret / K;
}

double logPredictiveProb(const vec& theta, const mat& phi, const Corpus& newCorpus)
{
    double ret = 0;
    unsigned long nTerms = 0;
    for (int d = 0; d < newCorpus.nDocs(); d++) {
        const vector<int>& doc = newCorpus.getDoc(d);
        const int docSize = doc.size();
        nTerms += docSize;
        for (int f = 0; f < docSize; f++) {
            double tmp = as_scalar(phi.row(doc[f]) * theta);
            ret += log(tmp);
        }
    }
    return ret / nTerms;
}

double logLikelihoodBiterm(const arma::vec& theta, const arma::mat& phi, const Corpus& newCorpus)
{
    double ret = 0;
    unsigned long nBiterms = 0;
    for (int d = 0; d < newCorpus.nDocs(); d++) {

        const vector<int>& doc = newCorpus.getDoc(d);
        const int docSize = doc.size();
        nBiterms += docSize * (docSize - 1) / 2;

        for (int f = 0; f < docSize - 1; f++) {
            for (int s = f + 1; s < docSize; s++) {
                double tmp = accu(theta % phi.row(doc[f]).t() % phi.row(doc[s]).t());
                ret += log(tmp);
            }
        }
    }
    return ret / nBiterms;
}

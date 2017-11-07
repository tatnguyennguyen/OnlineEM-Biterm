/**
 * @author  Nguyen Tat Nguyen
 * @date    6/2016
 */

#include "OnlineBiterm.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <time.h>

void OnlineBiterm::setAlpha(double a)
{
    alpha = a;
    alphaVec = ones(K) * alpha;
}

void OnlineBiterm::setBeta(double b)
{
    beta = b;
    betaMat = ones(W, K) * beta;
}

OnlineBiterm::OnlineBiterm(int _K, int _W)
{
    if (_K <= 0 || _W <= 0) {
        cerr << "Number of topic and vocabulary size must be positive\n";
        throw invalid_argument("Number of topic and vocabulary size must be positive");
    }

    K = _K;
    W = _W;
    alpha = 1e-6;
    beta = 1e-10;
    stepCount = 0;
    stepPower = 0.8;
    stepOffset = 2;

    arma_rng::set_seed_random();
    theta = normalise(randu(K), 1);
    phi = normalise(randu(W, K), 1);
    thetaSS = zeros(K);
    phiSS = zeros(W, K);

    alphaVec = ones(K) * alpha;
    betaMat = ones(W, K) * beta;
}

OnlineBiterm::OnlineBiterm(string name)
{
    loadModel(name);
}

void OnlineBiterm::saveModel(string name)
{
    // make stored matrix in the same form as the one stored by OnlineBTM (https://github.com/xiaohuiyan/OnlineBTM)
    phi.t().eval().save(string(name + ".phi").c_str(), raw_ascii);
    theta.t().eval().save(string(name + ".theta").c_str(), raw_ascii);
    phiSS.t().eval().save(string(name + ".phiSS").c_str(), raw_ascii);
    thetaSS.t().eval().save(string(name + ".thetaSS").c_str(), raw_ascii);
    ofstream paramFile(name + ".param");
    paramFile << K << " " << W << endl;
    paramFile << alpha << " " << beta << endl;
    paramFile << stepCount << " " << stepPower << " " << stepOffset << endl;
    paramFile.close();
}

void OnlineBiterm::loadModel(string name)
{
    phi.load(string(name + ".phi").c_str(), raw_ascii);
    rowvec theta_tmp;
    theta_tmp.load(string(name + ".theta").c_str(), raw_ascii);
    theta = theta_tmp.t();
    phiSS.load(string(name + ".phiSS").c_str(), raw_ascii);
    rowvec thetaSS_tmp;
    thetaSS_tmp.load(string(name + ".thetaSS").c_str(), raw_ascii);
    thetaSS = thetaSS_tmp.t();
    inplace_trans(phi);
    inplace_trans(phiSS);
    ifstream paramFile(name + ".param");
    paramFile >> K >> W;
    paramFile >> alpha >> beta;
    paramFile >> stepCount >> stepPower >> stepOffset;
    paramFile.close();
}

void OnlineBiterm::fitMiniBatch(const vector<Biterm>& biterms)
{
    int nSample = biterms.size();
    stepCount++;

    vec cThetaSS(K, fill::zeros);
    mat cPhiSS(W, K, fill::zeros);

    for (Biterm b : biterms) {
        vec t_nk = infer(b);
        cThetaSS += t_nk;
        cPhiSS.row(b.tid1) += t_nk.t();
        cPhiSS.row(b.tid2) += t_nk.t();
    }

    cThetaSS /= nSample;
    cPhiSS /= nSample;

    if (stepCount == 1) {
        thetaSS = cThetaSS;
        phiSS = cPhiSS;
    } else {
        double stepSize = pow(stepCount + stepOffset, -stepPower);
        thetaSS = (1 - stepSize) * thetaSS + stepSize * cThetaSS;
        phiSS = (1 - stepSize) * phiSS + stepSize * cPhiSS;
    }

    theta = normalise(thetaSS + alphaVec, 1);
    phi = normalise(phiSS + betaMat, 1);
}

vec OnlineBiterm::infer(Biterm biterm)
{
    vec ret = theta % phi.row(biterm.tid1).t() % phi.row(biterm.tid2).t();
    return normalise(ret, 1);
}

vec OnlineBiterm::infer(const vector<int>& doc)
{
    const int docSize = doc.size();
    vec ret = zeros(K);
    for (int i = 0; i < docSize - 1; i++) {
        for (int j = i + 1; j < docSize; j++) {
            ret += normalise(theta % phi.row(doc[i]).t() % phi.row(doc[j]).t(), 1);
        }
    }
    return normalise(ret, 1);
}

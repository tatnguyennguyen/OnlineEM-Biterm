/**
 * @author  Nguyen Tat Nguyen
 * @date    6/2016
 */

#ifndef _OL_BITERM_H_
#define _OL_BITERM_H_

#include "Biterm.h"
#include <armadillo>
#include <string>
#include <vector>

using namespace std;
using namespace arma;

class OnlineBiterm {
public:
    OnlineBiterm(int _K, int _W);
    OnlineBiterm(string name);
    void saveModel(string name);
    void loadModel(string name);

    void fitMiniBatch(const vector<Biterm>& biterms);
    vec infer(Biterm biterm);
    vec infer(const vector<int>& doc);

    void setAlpha(double a);
    void setBeta(double b);
    void resetStepCount() { stepCount = 0; }
    void setStepPower(double p) { stepPower = p; }
    void setStepOffset(double o) { stepOffset = o; }
    const vec& getTheta() { return theta; }
    const mat& getPhi() { return phi; }
    int getK() { return K; }
    int getW() { return W; }

private:
    double alpha, beta;
    int W; // vocabulary size
    int K; // number of topic
    int stepCount;
    double stepPower; // stepSize = pow(stepCount+stepOffset, stepPower)
    double stepOffset;
    mat phi, phiSS, betaMat; // each column of phi for each topic
    vec theta, thetaSS, alphaVec; // column vector
};

#endif

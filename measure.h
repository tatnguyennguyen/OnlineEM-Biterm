/**
 * @author  Nguyen Tat Nguyen
 * @date    6/2016
 */

#ifndef _MEASURE_H_
#define _MEASURE_H_

#include "Corpus.h"
#include <armadillo>

double NPMI(const arma::mat& phi, const Corpus& corpus, int nTopWord);
double logPredictiveProb(const arma::vec& theta, const arma::mat& phi, const Corpus& newCorpus);
double logLikelihoodBiterm(const arma::vec& theta, const arma::mat& phi, const Corpus& newCorpus);

#endif

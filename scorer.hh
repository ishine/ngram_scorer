//
// Created by Young Mo Kang on 9/27/19.
//

#ifndef NGRAM_SCORER_HH
#define NGRAM_SCORER_HH

#include <string>
#include <vector>

/**
 * Generic Scorer
 */
class Scorer {
public:
    virtual ~Scorer(); // in case child class needs to implement it
    virtual float log10_cond_prob(const std::vector<std::string> &ngram) = 0;
};

#endif //NGRAM_SCORER_HH

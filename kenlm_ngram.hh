//
// Created by Young Mo Kang on 9/27/19.
//

#ifndef NGRAM_KENLM_NGRAM_HH
#define NGRAM_KENLM_NGRAM_HH

#include "scorer.hh"
#include "lm/model.hh"
#include <memory>

class KenlmScorer : public Scorer {
public:
    KenlmScorer(const std::string& model_path) {
        lm::ngram::Config config;
        auto model = LoadVirtual(model_path.c_str(), config);

    }
private:
    std::unique_ptr<lm::base::Model> model;
};

#endif //NGRAM_KENLM_NGRAM_HH

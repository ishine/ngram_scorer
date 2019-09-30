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
        model = std::unique_ptr<lm::base::Model>(LoadVirtual(model_path.c_str(), config));
    }

    float log10_cond_prob(const std::vector<std::string> &words) {
        float cond_prob = 0;
        lm::ngram::State state, out_state;
        // avoid to inserting <s> in begin
        model->NullContextWrite(&state);
        for (const auto& word : words) {
            lm::WordIndex word_index = model->BaseVocabulary().Index(word);
            // encounter OOV
//            if (word_index == 0) {
//                return OOV_SCORE;
//            }
            cond_prob = model->BaseScore(&state, word_index, &out_state);
            std::swap(state, out_state);
        }

        return cond_prob;
    }

private:
    std::unique_ptr<lm::base::Model> model;
    lm::ngram::Config config;
};

#endif //NGRAM_KENLM_NGRAM_HH

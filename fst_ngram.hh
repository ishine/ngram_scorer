//
// Created by Young Mo Kang on 9/27/19.
//

#ifndef NGRAM_FST_NGRAM_HH
#define NGRAM_FST_NGRAM_HH

#include "scorer.hh"

#include <fst/flags.h>
#include <fst/extensions/far/far.h>
#include <ngram/ngram-output.h>

#include <memory>
#include <stdexcept>

class FstScorer : public Scorer {
public:
    FstScorer(const std::string &fst_path) {
        std::unique_ptr<fst::StdMutableFst> fst(
                fst::StdMutableFst::Read(fst_path, true));
        if (!fst) throw std::runtime_error("Error loading the model");
    }

    virtual float log10_cond_prob(const std::vector<std::string> &words) {
        return 0;
    }
};
#endif //NGRAM_FST_NGRAM_HH

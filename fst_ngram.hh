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

class NgramModel : public ngram::NGramOutput {
public:
    explicit NgramModel(fst::StdMutableFst *infst)
    : ngram::NGramOutput{infst} {}

    float log10_cond_prob(const std::vector<std::string> &words) {
        return 0;
    }
};

class FstScorer : public Scorer {
public:
    FstScorer(const std::string &fst_path) {
        infst = std::unique_ptr<fst::StdMutableFst>(
                fst::StdMutableFst::Read(fst_path, true));
        if (!infst) throw std::runtime_error("Error loading the model");
        model = std::unique_ptr<NgramModel>(
                new NgramModel{
                    infst.get()});
        if (!model) throw std::runtime_error("Error loading ngram model");
    }

    float log10_cond_prob(const std::vector<std::string> &words) {
        return model->log10_cond_prob(words);
    }

private:
    std::unique_ptr<fst::StdMutableFst> infst;
    std::unique_ptr<NgramModel> model;
};
#endif //NGRAM_FST_NGRAM_HH

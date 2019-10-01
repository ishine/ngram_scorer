#ifndef NGRAM_FST_NGRAM_HH
#define NGRAM_FST_NGRAM_HH

#include "scorer.hh"

#include <fst/flags.h>
#include <fst/extensions/far/far.h>
#include <ngram/ngram-output.h>
#include <ngram/ngram-model.h>

#include <memory>
#include <stdexcept>

class NGramOutputWrapper : public ngram::NGramOutput {
public:
    explicit NGramOutputWrapper(fst::StdMutableFst *infst)
            : ngram::NGramOutput{infst} {}

    float log_cond_prob(const std::vector<std::string> &words) {
        StateId mst;
        int order;
        double cost = 0;
        for (const auto &word : words) {
            auto label = GetFst().InputSymbols()->Find(word);
            if (!FindNGramInModel(&mst, &order, label, &cost)) {
                throw std::runtime_error("ngram not found");
            }
        }
        return -cost;
    }
};

class FstScorer : public Scorer {
public:
    FstScorer(const std::string &fst_path) {
        infst = std::unique_ptr<fst::StdMutableFst>(
                fst::StdMutableFst::Read(fst_path, true));
        if (!infst) throw std::runtime_error("Error loading the model");
        model = std::unique_ptr<NGramOutputWrapper>(
                new NGramOutputWrapper{
                    infst.get()});
        if (!model) throw std::runtime_error("Error loading ngram model");
    }

    float log10_cond_prob(const std::vector<std::string> &words) {
        return model->log_cond_prob(words) / LOGE_10;
    }

private:
    std::unique_ptr<fst::StdMutableFst> infst;
    std::unique_ptr<NGramOutputWrapper> model;
    static constexpr float LOGE_10 = 2.302585092994046;
};
#endif //NGRAM_FST_NGRAM_HH

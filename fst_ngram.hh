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

    float log_cond_prob(const std::vector<int64> &labels) const {
        StateId mst = 0; // unigram start
        int order;
        double cost = 0;
        for (const auto &label : labels) {
            if (!FindNGramInModel(&mst, &order, label, &cost)) {
                throw std::runtime_error("ngram not found");
            }
        }
        return -cost;
    }

    float log_cond_prob(const std::vector<std::string> &words) const {
        std::vector<int64> labels;
        for (const auto &word : words) {
            auto label = GetFst().InputSymbols()->Find(word);
            labels.push_back(label);
        }
        return log_cond_prob(labels);
    }

    float log_prob(const std::vector<int64> &labels) const {
        StateId mst = 0; // unigram start
        int order;
        double cost = 0, sum = 0;
        for (const auto &label : labels) {
            if (!FindNGramInModel(&mst, &order, label, &cost)) {
                throw std::runtime_error("ngram not found");
            }
            sum += cost;
        }
        return -sum;
    }

    float log_prob(const std::vector<std::string> &words) const {
        std::vector<int64> labels;
        for (const auto &word : words) {
            auto label = GetFst().InputSymbols()->Find(word);
            labels.push_back(label);
        }
        return log_prob(labels);
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

    virtual float log10_cond_prob(const std::vector<std::string> &words) const {
        return model->log_cond_prob(words) / LOGE_10;
    }

    virtual float log10_prob(const std::vector<std::string> &words) const {

    }

    // temporarily public
    std::unique_ptr<NGramOutputWrapper> model;

private:
    std::unique_ptr<fst::StdMutableFst> infst;
    static constexpr float LOGE_10 = 2.302585092994046;
};
#endif //NGRAM_FST_NGRAM_HH

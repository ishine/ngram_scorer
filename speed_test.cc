#include <iostream>
#include <vector>
#include <random>
#include <ctime>

#include "kenlm_ngram.hh"
#include "fst_ngram.hh"

class Tester {
public:
    Tester(const NGramOutputWrapper &model, int num_test)
    : max_order{model.HiOrder()}, num_test{num_test} {
        const auto isym = model.GetFst().InputSymbols();
        const auto num_keys = isym->AvailableKey();

        std::random_device rd;  //Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
        gen.seed(0);
        std::uniform_int_distribution<> dis(1, num_keys - 1);

        for (int i = 0; i < num_test; ++i) {
            const int n = dis(gen) % max_order + 1;
//            const int n = 3;
            std::vector<std::string> ngram;
            std::vector<int64> label_ngram;
            for (int j = 0; j < n; ++j) {
                const auto key = dis(gen);
                ngram.emplace_back(isym->Find(key));
                label_ngram.push_back(key);
            }
            ngrams.emplace_back(ngram);
            label_ngrams.emplace_back(label_ngram);
        }
    }

    std::vector<float> test(const Scorer &scorer) const {
        std::vector<float> result;
        for (const auto& ngram : ngrams) {
            result.push_back(scorer.log10_cond_prob(ngram));
        }

        return result;
    }

    std::vector<float> test_label(const Scorer &scorer) const {
        std::vector<float> result;
        for (const auto& ngram : label_ngrams) {
            result.push_back(scorer.log10_cond_prob(ngram));
        }

        return result;
    }

private:
    const int max_order, num_test;
    std::vector<std::vector<std::string>> ngrams;
    std::vector<std::vector<int64>> label_ngrams;
};

int main(int argc, const char** argv) {
    const std::string kenlm_binary{argv[1]};
    const std::string fst_model{argv[2]};
    int num_test;
    num_test = std::stoi(argv[3]);

    KenlmScorer kenlmScorer{kenlm_binary};
    FstScorer fstScorer{fst_model};

    Tester tester{*fstScorer.model, num_test};

    clock_t begin = clock();
    const auto test1 = tester.test(kenlmScorer);
    clock_t end = clock();
    std::cout << "KenLM time: " << double(end - begin) / CLOCKS_PER_SEC << std::endl;

    begin = clock();
    const auto test2 = tester.test_label(fstScorer);
    end = clock();
    std::cout << "FST time: " << double(end - begin) / CLOCKS_PER_SEC << std::endl;

    float sum1{0}, sum2{0};
    for (const auto p : test1) sum1 += p;
    for (const auto p : test2) sum2 += p;

    std::cout << sum1 << " " << sum2 << std::endl;

    return 0;
}
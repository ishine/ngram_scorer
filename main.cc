#include <iostream>
#include "kenlm_ngram.hh"
#include "fst_ngram.hh"

int main() {
    KenlmScorer kenlmScorer{"kenlm/lm/test.arpa"};
    std::vector<std::string> words = {"a", "little"};
    float p = kenlmScorer.log10_cond_prob(words);
    for (const auto& word : words) std::cout << word << " ";
    std::cout << " : " << p << std::endl;

    FstScorer fstScorer{"test.fst"};
    p = fstScorer.log10_cond_prob(words);
    std::cout << " : " << p << std::endl;

    return 0;
}
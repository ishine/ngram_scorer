#include <iostream>
#include "kenlm_ngram.hh"

int main() {
    KenlmScorer scorer{"kenlm/lm/test.arpa"};
    std::vector<std::string> words = {"a", "little"};
    float p = scorer.log10_cond_prob(words);
    for (const auto& word : words) std::cout << word << " ";
    std::cout << " : " << p << std::endl;
    return 0;
}
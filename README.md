## Ngram Scorer ##
### Prerequisite ###
```
git submodule update --init
wget http://www.openfst.org/twiki/pub/FST/FstDownload/openfst-1.7.3.tar.gz
wget http://www.openfst.org/twiki/pub/GRM/NGramDownload/ngram-1.3.7.tar.gz
tar xfz openfst-1.7.3.tar.gz
tar xfz ngram-1.3.7.tar.gz
cd openfst-1.7.3/ && ./configure --enable-far && make -j8 && cd ..
cd ngram-1.3.7/ && ./configure && make -j8 && cd ..

ngramread --ARPA kenlm/lm/test.arpa test.fst
```
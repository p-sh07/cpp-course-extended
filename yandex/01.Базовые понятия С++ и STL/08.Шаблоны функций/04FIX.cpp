//
//  04.Базовые понятия С++ и STL
//  Тема 08.Шаблоны функций
//  Задача 04.Как устроены шаблоны
//
//  Created by Pavel Sh on 28.11.2023.
//  fix it !!!


#include <cmath>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

template <typename TC, typename TC::value_type>
map<typename TC::value_type, int> ComputeTermFreqs(const TC& terms) {
    map<typename TC::value_type, int> term_freqs;
    for (const typename TC::value_type& term : terms) {
        ++term_freqs[term];
    }
    return term_freqs;
}

template <typename DocContainer, typename Term>
vector<double> ComputeTfIdfs(const DocContainer& documents, Term term) {
    vector<double> tf_idfs;
    vector<map<Term, int>> term_freqs;
    
    for(const auto& doc : documents) {
        auto term_map = ComputeTermFreqs(doc);
        int doc_word_count = 0, term_count = 0;
        for(const auto& [word, count] : doc) {
            doc_word_count += count;
            if(word == term) {
                term_count = count;
            }
        }
        term_freqs.push_back(move(term_map));
        tf_idfs.push_back(1.0*term_count/doc_word_count);
    }
    //number of doc containing term
    int document_freq = 0;
    for(const auto& term_map : term_freqs) {
        if(term_map.count(term) > 0) {
            ++document_freq;
        }
    }
    double idf = log(1.0 * documents.size()/document_freq);
    //find term occurence in each document and push back
    for(auto& tf : tf_idfs) {
        tf *= idf;
    }
    return tf_idfs;
}




int main() {
    const vector<vector<string>> documents = {
        {"белый"s, "кот"s, "и"s, "модный"s, "ошейник"s},
        {"пушистый"s, "кот"s, "пушистый"s, "хвост"s},
        {"ухоженный"s, "пёс"s, "выразительные"s, "глаза"s},
    };
    const auto& tf_idfs = ComputeTfIdfs(documents, "кот"s);
    for (const double tf_idf : tf_idfs) {
        cout << tf_idf << " "s;
    }
    cout << endl;
    return 0;
}

/*
 // tf_06.cpp

 #include <algorithm>
 #include <cmath>
 #include <iostream>
 #include <vector>

 using namespace std;

 template <typename Documents, typename Term>
 vector<double> ComputeTfIdfs(const Documents& documents, const Term& term) {
     vector<double> tf_idfs;

     int document_freq = 0;
     for (const auto& document : documents) {
         const int freq = count(document.begin(), document.end(), term);
         if (freq > 0) {
             ++document_freq;
         }
         tf_idfs.push_back(static_cast<double>(freq) / document.size());
     }

     const double idf = log(static_cast<double>(documents.size()) / document_freq);
     for (double& tf_idf : tf_idfs) {
         tf_idf *= idf;
     }

     return tf_idfs;
 }

 int main() {
     const vector<vector<string>> documents = {
         {"белый"s, "кот"s, "и"s, "модный"s, "ошейник"s},
         {"пушистый"s, "кот"s, "пушистый"s, "хвост"s},
         {"ухоженный"s, "пёс"s, "выразительные"s, "глаза"s},
     };
     const auto& tf_idfs = ComputeTfIdfs(documents, "кот"s);
     for (const double tf_idf : tf_idfs) {
         cout << tf_idf << " "s;
     }
     cout << endl;
     return 0;
 }
 */

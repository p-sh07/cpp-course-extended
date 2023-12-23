//
//  04.Базовые понятия С++ и STL
//  Тема 15.Обработка ошибок. Исключения
//  Задача 02.Использование кодов возврата
//
//  Created by Pavel Sh on 09.12.2023.
//

#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result;
    cin >> result;
    ReadLine();
    return result;
}

pair<bool, vector<string>> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    bool prev_was_minus = false;
    bool no_errors = true;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else if(0 <= static_cast<int>(c) &&
                  static_cast<int>(c) <= 31){
            return {false, {} };
        } else if(c == '-') {
            if(prev_was_minus) { //error
                no_errors = false;
            } else {
                prev_was_minus = true;
                word += c;
            }
            
        } else {
            prev_was_minus = false;
            word += c;
        }
    }
    if (!prev_was_minus && !word.empty()) {
        words.push_back(word);
    } //catch the trailing -
    if(prev_was_minus) {
        no_errors = false;
    }
    
    return {no_errors, words};
}

struct Document {
    Document() = default;
    
    Document(int id, double relevance, int rating)
    : id(id)
    , relevance(relevance)
    , rating(rating) {
    }
    
    int id = 0;
    double relevance = 0.0;
    int rating = 0;
};

template <typename StringContainer>
set<string> MakeUniqueNonEmptyStrings(const StringContainer& strings) {
    set<string> non_empty_strings;
    for (const string& str : strings) {
        if (!str.empty()) {
            non_empty_strings.insert(str);
        }
    }
    return non_empty_strings;
}

enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};

class SearchServer {
public:
    // Defines an invalid document id
    // You can refer to this constant as SearchServer::INVALID_DOCUMENT_ID
    inline static constexpr int INVALID_DOCUMENT_ID = -1;
    
    template <typename StringContainer>
    explicit SearchServer(const StringContainer& stop_words)
    : stop_words_(MakeUniqueNonEmptyStrings(stop_words)) {
    }
    
    explicit SearchServer(const string& stop_words_text)
    : SearchServer(SplitIntoWords(stop_words_text).second)  // Invoke delegating constructor from string container
    {
    }
    
    [[nodiscard]] bool AddDocument(int document_id, const string& document, DocumentStatus status,
                                   const vector<int>& ratings) {
        if(document_id < 0) {
            return false;
        }
        const auto words = SplitIntoWordsNoStop(document);
        if(words.empty() || (documents_.count(document_id) > 0)) {
            return false;
        }//Error: cannot parse, or id already present
        const double inv_word_count = 1.0 / words.size();
        for (const string& word : words) {
            word_to_document_freqs_[word][document_id] += inv_word_count;
        }
        documents_.emplace(document_id, DocumentData{ComputeAverageRating(ratings), status});
        return true;
    }
    
    template <typename DocumentPredicate>
    [[nodiscard]] bool FindTopDocuments(const string& raw_query, DocumentPredicate document_predicate, vector<Document>& result) const {
        Query query;
        if(!ParseQuery(raw_query, query)) {
            result = {};
            return false;
        }
        auto matched_documents = FindAllDocuments(query, document_predicate);
        
        sort(matched_documents.begin(), matched_documents.end(),
             [](const Document& lhs, const Document& rhs) {
            if (abs(lhs.relevance - rhs.relevance) < 1e-6) {
                return lhs.rating > rhs.rating;
            } else {
                return lhs.relevance > rhs.relevance;
            }
        });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        result = std::move(matched_documents);
        return true;
    }
    
    [[nodiscard]] bool FindTopDocuments(const string& raw_query, DocumentStatus status, vector<Document>& result) const {
        return FindTopDocuments(
                                raw_query, [status](int document_id, DocumentStatus document_status, int rating) {
                                    return document_status == status;
                                }, result);
    }
    [[nodiscard]] bool FindTopDocuments(const string& raw_query, vector<Document>& result) const {
        return FindTopDocuments( raw_query, DocumentStatus::ACTUAL, result);
    }
    
    int GetDocumentCount() const {
        return static_cast<int>(documents_.size());
    }
    int GetDocumentId(int doc_number) const {
        if(doc_number < 0 || doc_number >= documents_.size()) {
            return SearchServer::INVALID_DOCUMENT_ID;
        }
        auto nth_document = documents_.begin();
        advance(nth_document, doc_number);
        return nth_document->first; //returns id of nth document
    }
    
    bool MatchDocument(const string& raw_query, int document_id, tuple<vector<string>, DocumentStatus>& result) {
        Query query;
        if(!ParseQuery(raw_query, query) || documents_.count(document_id) == 0) {
            result = {};
            return false;
        }
        vector<string> matched_words;
        for (const string& word : query.plus_words) {
            for (const string& word : query.minus_words) {
                if (word_to_document_freqs_.count(word) > 0 && word_to_document_freqs_.at(word).count(document_id)) {
                    return {};
                }
            }
            if (word_to_document_freqs_.count(word) > 0 && word_to_document_freqs_.at(word).count(document_id)) {
                matched_words.push_back(word);
            }
        }
        result = {matched_words, documents_.at(document_id).status};
        return true;
    }
    
private:
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };
    const set<string> stop_words_;
    map<string, map<int, double>> word_to_document_freqs_;
    map<int, DocumentData> documents_;
    
    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }
    
    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        const auto split_text = SplitIntoWords(text);
        if(!split_text.first) {
            return {};
        }
        for (const string& word : split_text.second) {
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }
    
    static int ComputeAverageRating(const vector<int>& ratings) {
        if (ratings.empty()) {
            return 0;
        }
        int rating_sum = 0;
        for (const int rating : ratings) {
            rating_sum += rating;
        }
        return rating_sum / static_cast<int>(ratings.size());
    }
    
    struct QueryWord {
        string data;
        bool is_minus;
        bool is_stop;
    };
    
    QueryWord ParseQueryWord(string text) const {
        bool is_minus = false;
        // Word shouldn't be empty
        if (text[0] == '-') {
            is_minus = true;
            text = text.substr(1);
        }
        return {text, is_minus, IsStopWord(text)};
    }
    
    struct Query {
        set<string> plus_words;
        set<string> minus_words;
    };
    
    bool ParseQuery(const string& text, Query& result) const {
        //clear in case not empty
        result.minus_words.clear();
        result.plus_words.clear();
        //returns pair bool, vector
        const auto qwords = SplitIntoWords(text);
        if(!qwords.first) { //some errors in split
            return false; //return error
        }
        for (const string& word : qwords.second) {
            const QueryWord query_word = ParseQueryWord(word);
            if (!query_word.is_stop) {
                if (query_word.is_minus) {
                    result.minus_words.insert(query_word.data);
                } else {
                    result.plus_words.insert(query_word.data);
                }
            }
        }
        return true;
    }
    
    // Existence required
    double ComputeWordInverseDocumentFreq(const string& word) const {
        return log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
    }
    
    template <typename DocumentPredicate>
    vector<Document> FindAllDocuments(const Query& query,
                                      DocumentPredicate document_predicate) const {
        map<int, double> document_to_relevance;
        for (const string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
            for (const auto [document_id, term_freq] : word_to_document_freqs_.at(word)) {
                const auto& document_data = documents_.at(document_id);
                if (document_predicate(document_id, document_data.status, document_data.rating)) {
                    document_to_relevance[document_id] += term_freq * inverse_document_freq;
                }
            }
        }
        
        for (const string& word : query.minus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            for (const auto [document_id, _] : word_to_document_freqs_.at(word)) {
                document_to_relevance.erase(document_id);
            }
        }
        
        vector<Document> matched_documents;
        for (const auto [document_id, relevance] : document_to_relevance) {
            matched_documents.push_back(
                                        {document_id, relevance, documents_.at(document_id).rating});
        }
        return matched_documents;
    }
};

// ==================== для примера =========================

void PrintDocument(const Document& document) {
    cout << "{ "s
    << "document_id = "s << document.id << ", "s
    << "relevance = "s << document.relevance << ", "s
    << "rating = "s << document.rating << " }"s << endl;
}

int main() {
    
    {
        SearchServer search_server("и в на"s);
        // Явно игнорируем результат метода AddDocument, чтобы избежать предупреждения
        // о неиспользуемом результате его вызова
        (void) search_server.AddDocument(1, "пушистый кот пушистый хвост"s, DocumentStatus::ACTUAL, {7, 2, 7});
        if (!search_server.AddDocument(1, "пушистый пёс и модный ошейник"s, DocumentStatus::ACTUAL, {1, 2})) {
            cout << "Документ не был добавлен, так как его id совпадает с уже имеющимся"s << endl;
        }
        if (!search_server.AddDocument(-1, "пушистый пёс и модный ошейник"s, DocumentStatus::ACTUAL, {1, 2})) {
            cout << "Документ не был добавлен, так как его id отрицательный"s << endl;
        }
        if (!search_server.AddDocument(3, "большой пёс скво\x12рец"s, DocumentStatus::ACTUAL, {1, 3, 2})) {
            cout << "Документ не был добавлен, так как содержит спецсимволы"s << endl;
        }
        vector<Document> documents;
        if (search_server.FindTopDocuments("--пушистый"s, documents)) {
            for (const Document& document : documents) {
                PrintDocument(document);
            }
        } else {
            cout << "Ошибка в поисковом запросе"s << endl;
        }
    }
    
    {
        SearchServer search_server("и в на"s);
        
        bool success = search_server.AddDocument(0, "белый кот и модный ошейник"s, DocumentStatus::ACTUAL, {8, -3});
        success = search_server.AddDocument(1, "пушистый кот пушистый хвост"s, DocumentStatus::ACTUAL, {7, 2, 7});
        success = search_server.AddDocument(2, "ухоженный пёс выразительные глаза"s, DocumentStatus::ACTUAL,
                                            {5, -12, 2, 1});
        success = search_server.AddDocument(3, "ухоженный скворец евгений"s, DocumentStatus::BANNED, {9});
        
        cout << "ACTUAL by default:"s << endl;
        vector<Document> result;
        if(search_server.FindTopDocuments("пушистый ухоженный кот"s, result)) {
            for (const Document& document : result) {
                PrintDocument(document);
            }
        } else {
            cerr << "Failed a good search" << endl;
        }
        success = search_server.FindTopDocuments("на"s, result);
        cerr << boolalpha;
        cerr << "Search with a stop word returned: "  << success << endl;
        cerr << "Result is empty: " << result.empty() << endl;
        if(success) {
            for (const Document& document : result) {
                PrintDocument(document);
            }
        }
        tuple<vector<string>, DocumentStatus> match_result;
        success = search_server.MatchDocument("нах"s, 0, match_result);
        cerr << boolalpha;
        cerr << "Match with a stop word returned: "  << success << endl;
        cerr << "Result is empty: " << get<0>(match_result).empty() << endl;
        if(success) {
            for (const Document& document : result) {
                PrintDocument(document);
            }
        }
    }

    {
        SearchServer search_server("и в на"s);
        // Явно игнорируем результат метода AddDocument, чтобы избежать предупреждения
        // о неиспользуемом результате его вызова
        (void) search_server.AddDocument(1, "пушистый кот пушистый хвост"s, DocumentStatus::ACTUAL, {7, 2, 7});
        vector<Document> documents;
        if (search_server.FindTopDocuments("-пушистый"s, documents)) {
            for (const Document& document : documents) {
                PrintDocument(document);
            }
        } else {
            cout << "Ошибка в поисковом запросе"s << endl;
        }
        if (search_server.FindTopDocuments("в"s, documents)) {
            for (const Document& document : documents) {
                PrintDocument(document);
            }
        } else {
                cerr << boolalpha;
                cerr << "Search with a stop word returned: False" << endl;
                cerr << "Result is empty: " << documents.empty() << endl;
        }
        vector<Document> result;
        if(search_server.FindTopDocuments("кот"s, result)) {
                cerr << boolalpha;
                cerr << "Search with a non-stop word returned: True" << endl;
                cerr << "Result is empty: " << result.empty() << endl;
            for (const Document& document : result) {
                PrintDocument(document);
            }
        } else {
            cerr << boolalpha;
            cerr << "Search with a non-stop word returned: False" << endl;
            cerr << "Result is empty: " << result.empty() << endl;
        }
        if(search_server.FindTopDocuments("на"s, result)) {
            for (const Document& document : result) {
                PrintDocument(document);
            }
        } else {
            cerr << boolalpha;
            cerr << "Search with a stop word returned: False" << endl;
            cerr << "Result is empty: " << result.empty() << endl;
        }
        if(search_server.FindTopDocuments("-нет"s, result)) {
            for (const Document& document : result) {
                PrintDocument(document);
            }
        }
        tuple<vector<string>, DocumentStatus> match_result;
        bool success = search_server.MatchDocument("в"s, 0, match_result);
        cerr << boolalpha;
        cerr << "Match with a stop word returned: "  << success << endl;
        cerr << "Result is empty: " << get<0>(match_result).empty() << endl;
        if(success) {
            for (const Document& document : result) {
                PrintDocument(document);
            }
        }
        success = search_server.MatchDocument("-no"s, 0, match_result);
        cerr << boolalpha;
        cerr << "Match with a stop word returned: "  << success << endl;
        cerr << "Result is empty: " << get<0>(match_result).empty() << endl;
        if(success) {
            for (const Document& document : result) {
                PrintDocument(document);
            }
        }
    }
return 0;
}

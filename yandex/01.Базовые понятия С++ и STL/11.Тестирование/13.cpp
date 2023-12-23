//
//  04.Базовые понятия С++ и STL
//  Тема 11.Тестирование
//  Задача 13.Фреймворк юнит-тестов
//
//  Created by Pavel Sh on 06.12.2023.
//

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

/* Подставьте вашу реализацию класса SearchServer сюда */
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

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }
    
    return words;
}

struct Document {
    int id;
    double relevance;
    int rating;
};

void PrintDocument(const Document& document) {
    cout << "{ "s
    << "document_id = "s << document.id << ", "s
    << "relevance = "s << document.relevance << ", "s
    << "rating = "s << document.rating
    << " }"s << endl;
}

enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};

class SearchServer {
public:
    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }
    
    void AddDocument(int document_id, const string& document, DocumentStatus status,
                     const vector<int>& ratings) {
        const vector<string> words = SplitIntoWordsNoStop(document);
        const double inv_word_count = 1.0 / words.size();
        for (const string& word : words) {
            word_to_document_freqs_[word][document_id] += inv_word_count;
        }
        documents_.emplace(document_id, DocumentData{ComputeAverageRating(ratings), status});
    }
    
    template <typename DocumentCompare>
    vector<Document> FindTopDocuments(const string& raw_query, DocumentCompare document_check) const {
        const Query query = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query, document_check);
        
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
        return matched_documents;
    }
    //overload to call with status + default ACTUAL
    vector<Document> FindTopDocuments(const string& raw_query, DocumentStatus required_status = DocumentStatus::ACTUAL) const {
        return FindTopDocuments(raw_query, [&required_status](int document_id, DocumentStatus status, int rating) {
            return status == required_status;
        });
    }
    
    int GetDocumentCount() const {
        return static_cast<int>(documents_.size());
    }
    
    tuple<vector<string>, DocumentStatus> MatchDocument(const string& raw_query,
                                                        int document_id) const {
        //return empty if such document id does not exist
        if(documents_.count(document_id) == 0) {
            return {};
        }
        const Query query = ParseQuery(raw_query);
        const DocumentStatus this_doc_status = documents_.at(document_id).status;
        //return empty vector if a minus word is found
        for (const string& word : query.minus_words) {
            if (word_to_document_freqs_.at(word).count(document_id)) {
                return {{}, this_doc_status};
            }
        }
        //no minus words, find matching plus words
        vector<string> matched_words;
        for (const string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            if (word_to_document_freqs_.at(word).count(document_id)) {
                matched_words.push_back(word);
            }
        }
        return {matched_words, this_doc_status};
    }
    
private:
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };
    
    set<string> stop_words_;
    map<string, map<int, double>> word_to_document_freqs_;
    map<int, DocumentData> documents_;
    
    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }
    
    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
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
    
    Query ParseQuery(const string& text) const {
        Query query;
        for (const string& word : SplitIntoWords(text)) {
            const QueryWord query_word = ParseQueryWord(word);
            if (!query_word.is_stop) {
                if (query_word.is_minus) {
                    query.minus_words.insert(query_word.data);
                } else {
                    query.plus_words.insert(query_word.data);
                }
            }
        }
        return query;
    }
    
    // Existence required
    double ComputeWordInverseDocumentFreq(const string& word) const {
        //        cerr << "Computing: log " <<  GetDocumentCount() * 1.0 << '/' << word_to_document_freqs_.at(word).size() << endl;
        return log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
    }
    
    template <typename DocumentCompare>
    vector<Document> FindAllDocuments(const Query& query, DocumentCompare document_check) const {
        map<int, double> document_to_relevance;
        for (const string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
            for (const auto [document_id, term_freq] : word_to_document_freqs_.at(word)) {
                const auto& current_doc = documents_.at(document_id);
                if (document_check(document_id, current_doc.status, current_doc.rating)) {
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
/*
 Подставьте сюда вашу реализацию макросов
 ASSERT, ASSERT_EQUAL, ASSERT_EQUAL_HINT, ASSERT_HINT и RUN_TEST
 */
// Функция LogImpl выполняет основную работу
void LogImpl(const string& str, const string& func_name, const string& file_name, int line_number) {
    cerr << file_name << "("s << line_number << "): "s;
    cerr << func_name << ": "s << str << endl;
}

#define LOG(expr) LogImpl(#expr, __FUNCTION__, __FILE__, __LINE__)

void AssertImpl(const bool t, const string& t_str, const string& file,
                const string& func, unsigned line, const string& hint) {
    if (t != true) {
        cerr << boolalpha;
        cerr << file << "("s << line << "): "s << func << ": "s;
        cerr << "ASSERT("s << t_str << ") failed: "s;
        cerr << t << " != True."s;
        if (!hint.empty()) {
            cerr << " Hint: "s << hint;
        }
        cerr << endl;
        //        abort();
    }
}

#define ASSERT(a) AssertImpl((a), #a,  __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_HINT(a, hint) AssertImpl((a), #a, __FILE__, __FUNCTION__, __LINE__, (hint))


template <typename T, typename U>
void AssertEqualImpl(const T& t, const U& u, const string& t_str, const string& u_str, const string& file,
                     const string& func, unsigned line, const string& hint) {
    if (t != u) {
        cerr << boolalpha;
        cerr << file << "("s << line << "): "s << func << ": "s;
        cerr << "ASSERT_EQUAL("s << t_str << ", "s << u_str << ") failed: "s;
        cerr << t << " != "s << u << "."s;
        if (!hint.empty()) {
            cerr << " Hint: "s << hint;
        }
        cerr << endl;
        //        abort();
    }
}

#define ASSERT_EQUAL(a, b) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))

template <typename TestFunc>
void RunTestImpl(const TestFunc& func, const string& test_name) {
    func();
    cerr << test_name << " OK"s << endl;
}

#define RUN_TEST(func) RunTestImpl(func, #func)
// -------- Начало модульных тестов поисковой системы ----------

/*
 Разместите код остальных тестов здесь
 */
//Добавление документов. Добавленный документ должен находиться по поисковому запросу, который содержит слова из документа.
void TestDocumentAdd() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("cat"s);
        ASSERT_EQUAL(found_docs.size(), 1u);
        const Document& doc0 = found_docs[0];
        ASSERT_EQUAL(doc0.id, doc_id);
    }
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("city"s);
        ASSERT_EQUAL(found_docs.size(), 1u);
        const Document& doc0 = found_docs[0];
        ASSERT_EQUAL(doc0.id, doc_id);
    }
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        ASSERT_HINT(server.FindTopDocuments("dog"s).empty(),
                    "There should be no documents found with the word [dog]"s);
    }
}
// Тест проверяет, что поисковая система исключает стоп-слова при добавлении документов
void TestExcludeStopWordsFromAddedDocumentContent() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("in"s);
        ASSERT_EQUAL(found_docs.size(), 1u);
        const Document& doc0 = found_docs[0];
        ASSERT_EQUAL(doc0.id, doc_id);
    }
    
    {
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        ASSERT_HINT(server.FindTopDocuments("in"s).empty(),
                    "Stop words must be excluded from documents"s);
    }
}
//Поддержка минус-слов. Документы, содержащие минус-слова из поискового запроса, не должны включаться в результаты поиска.
void TestMinusWords() {
    const int doc_id1 = 41;
    const string content1 = "cat in the city"s;
    const vector<int> ratings1 = {1, 2, 3};
    
    const int doc_id2 = 42;
    const string content2 = "dog in the park"s;
    const vector<int> ratings2 = {1, 2, 3};
    {
        SearchServer server;
        server.AddDocument(doc_id1, content1, DocumentStatus::ACTUAL, ratings1);
        server.AddDocument(doc_id2, content2, DocumentStatus::ACTUAL, ratings2);
        const auto found_docs = server.FindTopDocuments("in -park"s);
        ASSERT_EQUAL(found_docs.size(), 1u);
        const Document& doc0 = found_docs[0];
        ASSERT_EQUAL(doc0.id, doc_id1);
    }
}
//Соответствие документов поисковому запросу. При этом должны быть возвращены все слова из поискового запроса, присутствующие в документе. Если есть соответствие хотя бы по одному минус-слову, должен возвращаться пустой список слов.
void TestMatchDocumentReturn() {
    const int doc_id1 = 41;
    const string content1 = "cat in the city"s;
    const vector<int> ratings1 = {1, 2, 3};
    
    const int doc_id2 = 42;
    const string content2 = "dog in the park"s;
    const vector<int> ratings2 = {1, 2, 3};
    
    const string query = "cat"s;
    const vector<string> plus_words_required{"cat"};
    {//return of correct words
        SearchServer server;
        server.AddDocument(doc_id1, content1, DocumentStatus::ACTUAL, ratings1);
        server.AddDocument(doc_id2, content2, DocumentStatus::BANNED, ratings2);
        const auto found_docs = server.FindTopDocuments(query);
        ASSERT_EQUAL(found_docs.size(), 1u);
        const Document& doc0 = found_docs[0];
        ASSERT_EQUAL(doc0.id, doc_id1);
        const vector<string> plus_words = std::get<0>(server.MatchDocument(query, doc_id1));
        ASSERT_EQUAL(plus_words.size(), plus_words_required.size());
        ASSERT_EQUAL_HINT(plus_words[0], plus_words_required[0], "Should return all matching plus words from query"s);
    }
    const string query2 = "dog -park"s;
    {//return empty when minus word present
        SearchServer server;
        server.AddDocument(doc_id1, content1, DocumentStatus::ACTUAL, ratings1);
        server.AddDocument(doc_id2, content2, DocumentStatus::BANNED, ratings2);
        ASSERT(server.FindTopDocuments(query2).empty());
        const vector<string> plus_words = std::get<0>(server.MatchDocument(query2, doc_id2));
        ASSERT_HINT(plus_words.empty(), "Should return empty vector when minus words found in document"s);
    }
    const string query3 = "dog"s;
    const vector<string> plus_words_required3{"dog"};
    const DocumentStatus required_status = DocumentStatus::BANNED;
    {//return of correct words with doc status other than ACTUAL & returns correct status
        SearchServer server;
        server.AddDocument(doc_id1, content1, DocumentStatus::ACTUAL, ratings1);
        server.AddDocument(doc_id2, content2, DocumentStatus::BANNED, ratings2);
        const auto found_docs = server.FindTopDocuments(query3, DocumentStatus::BANNED);
        ASSERT_EQUAL(found_docs.size(), 1u);
        const Document& doc0 = found_docs[0];
        ASSERT_EQUAL(doc0.id, doc_id2);
        const auto match_return = server.MatchDocument(query3, doc_id2);
        const vector<string>& plus_words = std::get<0>(match_return);
        const DocumentStatus& status = std::get<DocumentStatus>(match_return);
        ASSERT_EQUAL(plus_words.size(), plus_words_required3.size());
        ASSERT_EQUAL_HINT(plus_words[0], plus_words_required3[0], "Should return all matching plus words from query for any document status"s);
        ASSERT_HINT(status == required_status, "MatchDocument replaces document status!");
    }
}
//Сортировка найденных документов по релевантности. Возвращаемые при поиске документов результаты должны быть отсортированы в порядке убывания релевантности.
void TestDocumentSortByRelevance() {
    const int doc_id1 = 41;
    const string content1 = "cat in the city"s;
    const vector<int> ratings1 = {1, 2, 3};
    
    const int doc_id2 = 42;
    const string content2 = "dog in the park"s;
    const vector<int> ratings2 = {1, 2, 3};
    
    const string query = "in the city"s;
    {//return in correct order
        SearchServer server;
        server.AddDocument(doc_id1, content1, DocumentStatus::ACTUAL, ratings1);
        server.AddDocument(doc_id2, content2, DocumentStatus::ACTUAL, ratings2);
        const auto found_docs = server.FindTopDocuments(query);
        ASSERT_EQUAL(found_docs.size(), 2u);
        const Document& doc0 = found_docs[0];
        const Document& doc1 = found_docs[1];
        ASSERT_EQUAL(doc0.id, doc_id1);
        ASSERT_EQUAL(doc1.id, doc_id2);
        ASSERT_HINT(doc0.relevance > doc1.relevance, "Return documents should be sorted by decreasing relevance"s);
    }
}
//Вычисление рейтинга документов. Рейтинг добавленного документа равен среднему арифметическому оценок документа.
void TestRatingCompute() {
    const int doc_id1 = 41;
    const string content1 = "cat in the city"s;
    const vector<int> ratings1 = {1, 2, 3}; // 6/3 = 2;
    
    const int doc_id2 = 42;
    const string content2 = "dog in the park"s;
    const vector<int> ratings2 = {1, 3, 5}; // 9/3 = 3;
    
    const string query = "in the city"s;
    {//return of correct retings
        SearchServer server;
        server.AddDocument(doc_id1, content1, DocumentStatus::ACTUAL, ratings1);
        server.AddDocument(doc_id2, content2, DocumentStatus::ACTUAL, ratings2);
        const auto found_docs = server.FindTopDocuments(query);
        ASSERT_EQUAL(found_docs.size(), 2u);
        const Document& doc0 = found_docs[0];
        const Document& doc1 = found_docs[1];
        
        ASSERT_EQUAL(doc0.rating, 2);
        ASSERT_EQUAL(doc1.rating, 3);
    }
}
//Фильтрация результатов поиска с использованием предиката, задаваемого пользователем.
void TestUserDefinedPredicateFilter(){
    {
        {//return by status
            SearchServer search_server;
            search_server.SetStopWords("и в на"s);
            search_server.AddDocument(0, "белый кот и модный ошейник"s,        DocumentStatus::ACTUAL, {1});
            search_server.AddDocument(1, "пушистый кот пушистый хвост"s,       DocumentStatus::ACTUAL, {2});
            search_server.AddDocument(2, "ухоженный пёс выразительные глаза"s, DocumentStatus::ACTUAL, {3});
            search_server.AddDocument(3, "ухоженный скворец евгений"s,         DocumentStatus::BANNED, {4});
            
            const auto found_docs_status_banned =  search_server.FindTopDocuments("пушистый ухоженный кот"s, [](int document_id, DocumentStatus status, int rating) { return status == DocumentStatus::BANNED; });
            
            ASSERT_EQUAL(found_docs_status_banned.size(), 1u);
            const Document& doc0 = found_docs_status_banned[0];
            ASSERT_EQUAL_HINT(doc0.id, 3, "Needs to return user-defined filered docs (Status == Banned)");
            
        }
        {//return above sertain rating
            SearchServer search_server;
            search_server.SetStopWords("и в на"s);
            search_server.AddDocument(0, "белый кот и модный ошейник"s,        DocumentStatus::ACTUAL, {1});
            search_server.AddDocument(1, "пушистый кот пушистый хвост"s,       DocumentStatus::ACTUAL, {2});
            search_server.AddDocument(2, "ухоженный пёс выразительные глаза"s, DocumentStatus::ACTUAL, {3});
            search_server.AddDocument(3, "ухоженный скворец евгений"s,         DocumentStatus::BANNED, {4});
            
            const auto found_docs_rating_above2 =  search_server.FindTopDocuments("пушистый ухоженный кот и пёс"s, [](int document_id, DocumentStatus status, int rating) { return rating > 2; });
            ASSERT_EQUAL(found_docs_rating_above2.size(), 2u);
            const Document& doc0 = found_docs_rating_above2[0];
            const Document& doc1 = found_docs_rating_above2[1];
            
            ASSERT_EQUAL(doc0.id, 2);
            ASSERT_EQUAL(doc1.id, 3);
            
            ASSERT_EQUAL_HINT(doc0.rating, 3, "Needs to return user-defined filered docs (Rating > 2)");
            ASSERT_EQUAL_HINT(doc1.rating, 4, "Needs to return user-defined filered docs (Rating > 2)");
        }
        
    }
}
//Поиск документов, имеющих заданный статус.
void TestStatusFilter() {
    SearchServer search_server;
    search_server.SetStopWords("и в на"s);
    search_server.AddDocument(0, "белый кот и модный ошейник"s,        DocumentStatus::ACTUAL, {1});
    search_server.AddDocument(1, "пушистый кот пушистый хвост"s,       DocumentStatus::ACTUAL, {2});
    search_server.AddDocument(2, "ухоженный пёс выразительные глаза"s, DocumentStatus::IRRELEVANT, {3});
    search_server.AddDocument(3, "ухоженный скворец евгений"s,         DocumentStatus::BANNED, {4});
    {//return actual by default
        const auto found_docs_actual =  search_server.FindTopDocuments("пушистый ухоженный кот"s);
        ASSERT_EQUAL(found_docs_actual.size(), 2u);
        const Document& doc0 = found_docs_actual[0];
        const Document& doc1 = found_docs_actual[1];
        
        ASSERT_EQUAL_HINT(doc0.id, 1, "Failed to return ACTUAL by default");
        ASSERT_EQUAL_HINT(doc1.id, 0, "Failed to return ACTUAL by default");
    }
    {//return IRRELEVANT
        const auto found_docs_irrelevant =  search_server.FindTopDocuments("пушистый ухоженный кот"s, DocumentStatus::IRRELEVANT);
        ASSERT_EQUAL(found_docs_irrelevant.size(), 1u);
        const Document& doc0 = found_docs_irrelevant[0];
        
        ASSERT_EQUAL_HINT(doc0.id, 2, "Failed to return IRRELEVANT");
    }
}
//Корректное вычисление релевантности найденных документов.
void TestRelevanceCompute() {
    const int doc_id = 41;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3}; // 6/3 = 2;
    
    const int doc_id2 = 42;
    const string content2 = "dog in the park"s;
    const vector<int> ratings2 = {1, 3, 5}; // 9/3 = 3;
    
    
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        server.AddDocument(doc_id2, content2, DocumentStatus::ACTUAL, ratings2);
        const auto found_docs = server.FindTopDocuments("cat"s);
        ASSERT_EQUAL(found_docs.size(), 1u);
        const Document& doc0 = found_docs[0];
        ASSERT_EQUAL(doc0.id, doc_id);
        
        const double relevance = (log(2.0/1)* 0.25);
        ASSERT_EQUAL(doc0.relevance, relevance);
    }
    
}
// Функция TestSearchServer является точкой входа для запуска тестов
void TestSearchServer() {
    // Не забудьте вызывать остальные тесты здесь
    RUN_TEST(TestDocumentAdd);
    //***sample test***
    RUN_TEST(TestExcludeStopWordsFromAddedDocumentContent);
    RUN_TEST(TestMinusWords);
    RUN_TEST(TestMatchDocumentReturn);
    RUN_TEST(TestDocumentSortByRelevance);
    RUN_TEST(TestRatingCompute);
    RUN_TEST(TestUserDefinedPredicateFilter);
    RUN_TEST(TestStatusFilter);
    RUN_TEST(TestRelevanceCompute);
}

// --------- Окончание модульных тестов поисковой системы -----------

int main() {
    TestSearchServer();
    // Если вы видите эту строку, значит все тесты прошли успешно
    cerr << "Search server testing finished"s << endl;
}
/*
 SearchServer search_server;
 search_server.SetStopWords("и в на"s);
 search_server.AddDocument(0, "белый кот и модный ошейник"s,        DocumentStatus::ACTUAL, {8, -3});
 search_server.AddDocument(1, "пушистый кот пушистый хвост"s,       DocumentStatus::ACTUAL, {7, 2, 7});
 search_server.AddDocument(2, "ухоженный пёс выразительные глаза"s, DocumentStatus::ACTUAL, {5, -12, 2, 1});
 search_server.AddDocument(3, "ухоженный скворец евгений"s,         DocumentStatus::BANNED, {9});
 cout << "ACTUAL by default:"s << endl;
 for (const Document& document : search_server.FindTopDocuments("пушистый ухоженный кот"s)) {
 PrintDocument(document);
 }
 cout << "ACTUAL:"s << endl;
 for (const Document& document : search_server.FindTopDocuments("пушистый ухоженный кот"s, [](int document_id, DocumentStatus status, int rating) { return status == DocumentStatus::ACTUAL; })) {
 PrintDocument(document);
 }
 cout << "Even ids:"s << endl;
 for (const Document& document : search_server.FindTopDocuments("пушистый ухоженный кот"s, [](int document_id, DocumentStatus status, int rating) { return document_id % 2 == 0; })) {
 PrintDocument(document);
 }
 */


//01.06.2024

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <string>

using namespace std;

struct Nucleotide {     // total packed -> 64 bits
    size_t position;    // 0 -> 3,3 * 10^9 -> 2^35 -> 32 bits
    int gene_num;       // 0 -> 2,5 * 10^4 -> 15 bits
    bool is_marked;     // flag, 1 bit
    char service_info;  // any char ->  8 bits
    char symbol;        // A, T, G, C -> 3 bits
    int chromosome_num; // 1 -> 48 -> 2^6 -> 6 bits
};

const static int N = 4;
const std::array<char, N> nucs = {'A', 'T', 'G', 'C'};

inline uint16_t SymToId(const char& nuc) {
    for(uint16_t i = 0; i < 4; ++i) {
        if(nuc == nucs[i]) {
            return i;
        }
    }
    return 0;
}

inline char IdToSym(uint16_t id) {
    return nucs[id];
}

struct CompactNucleotide {
    // напишите вашу реализацию здесь
    // total packed -> 64 bits
   uint32_t position:32;      // 0 -> 3,3 * 10^9 -> 2^35 -> 32 bits
   uint16_t gene_num:15;      // 0 -> 2,5 * 10^4 -> 15 bits
   uint16_t is_marked:1;      // flag, 1 bit
   uint16_t service_info:8;   // any char ->  8 bits
   uint16_t symbol:2;         // A, T, G, C -> up to 3 -> 2 bits
   uint16_t chromosome_num:6; // 1 -> 48 -> 2^6 -> 6 bits
};

CompactNucleotide Compress(const Nucleotide& n) {
    // напишите вашу реализацию здесь
    return CompactNucleotide {
        static_cast<uint32_t>(n.position),
        static_cast<uint16_t>(n.gene_num),
        static_cast<uint16_t>(n.is_marked),
        static_cast<uint16_t>(n.service_info),
        SymToId(n.symbol),
        static_cast<uint16_t>(n.chromosome_num)
    };
}

Nucleotide Decompress(const CompactNucleotide& cn) {
    // напишите вашу реализацию здесь
    return Nucleotide {
        static_cast<size_t>(cn.position),
        static_cast<int>(cn.gene_num),
        static_cast<bool>(cn.is_marked),
        static_cast<char>(cn.service_info),
        IdToSym(cn.symbol),
        static_cast<int>(cn.chromosome_num)
    };
}

static_assert(sizeof(CompactNucleotide) <= 8, "Your CompactNucleotide is not compact enough");
static_assert(alignof(CompactNucleotide) == 4, "Don't use '#pragma pack'!");
bool operator==(const Nucleotide& lhs, const Nucleotide& rhs) {
    return (lhs.symbol == rhs.symbol) && (lhs.position == rhs.position) && (lhs.chromosome_num == rhs.chromosome_num)
        && (lhs.gene_num == rhs.gene_num) && (lhs.is_marked == rhs.is_marked) && (lhs.service_info == rhs.service_info);
}
void TestSize() {
    assert(sizeof(CompactNucleotide) <= 8);
}
void TestCompressDecompress() {
    Nucleotide source;
    source.symbol = 'T';
    source.position = 1'000'000'000;
    source.chromosome_num = 48;
    source.gene_num = 1'000;
    source.is_marked = true;
    source.service_info = '!';

    CompactNucleotide compressedSource = Compress(source);
    Nucleotide decompressedSource = Decompress(compressedSource);

    assert(source == decompressedSource);
}

int main() {
    TestSize();
    TestCompressDecompress();
}

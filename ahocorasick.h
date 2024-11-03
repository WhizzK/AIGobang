#pragma once
#include <vector>
#include <string>
#include <unordered_map>

struct Pattern {
    std::string pattern;
    int score;
};

class AhoCorasick {
public:
    explicit AhoCorasick(const std::vector<Pattern>& patterns) {
        buildTrie(patterns);
        buildFailureLinks();
    }

    std::vector<int> search(const std::string& text) const;

private:
    struct TrieNode {
        std::unordered_map<char, int> children;
    };
    void buildTrie(const std::vector<Pattern>& patterns);
    void buildFailureLinks();
    std::vector<TrieNode> nodes;
    std::vector<int> failure;
    std::unordered_map<int, std::vector<int>> output;
    std::vector<Pattern> patterns;
};

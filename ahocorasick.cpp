#include "ahocorasick.h"

#include <iostream>
#include <queue>

std::vector<int> AhoCorasick::search(const std::string &text) const {
    std::vector<int> result;
    int node = 0;
    for (char ch : text) {
        while (node != 0 && nodes[node].children.count(ch) == 0) {
            node = failure[node];
        }
        if (nodes[node].children.count(ch) > 0) {
            node = nodes[node].children.at(ch);
        }
        int tempNode = node;
        while (tempNode != 0) {
            if (output.find(tempNode) != output.end()) {
                for (int patternIndex : output.at(tempNode)) {
                    result.push_back(patterns[patternIndex].score);
                }
            }
            tempNode = failure[tempNode];
        }
    }
    return result;
}

void AhoCorasick::buildTrie(const std::vector<Pattern> &patterns) {
    this->patterns = patterns;
    nodes.emplace_back(); // root node
    for (int i = 0; i < patterns.size(); ++i) {
        const std::string& pattern = patterns[i].pattern;
        int node = 0;
        for (char ch : pattern) {
            if (nodes[node].children.count(ch) == 0) {
                nodes[node].children[ch] = nodes.size();
                nodes.emplace_back();
            }
            node = nodes[node].children[ch];
        }
        output[node].push_back(i);
    }
}

void AhoCorasick::buildFailureLinks()  {
    failure.resize(nodes.size(), 0);
    std::queue<int> q;
    for (const auto& pair : nodes[0].children) {
        int node = pair.second;
        failure[node] = 0;
        q.push(node);
    }
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        for (const auto& pair : nodes[current].children) {
            char ch = pair.first;
            int child = pair.second;
            int fallback = failure[current];
            while (fallback != 0 && nodes[fallback].children.count(ch) == 0) {
                fallback = failure[fallback];
            }
            if (nodes[fallback].children.count(ch) > 0) {
                fallback = nodes[fallback].children[ch];
            }
            failure[child] = fallback;
            output[child].insert(output[child].end(), output[fallback].begin(), output[fallback].end());
            q.push(child);
        }
    }
}

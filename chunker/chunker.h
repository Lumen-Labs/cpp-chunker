#pragma once

#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <sstream>
#include <cstring>
#include <cstddef>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <cmath>
#include <memory>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

namespace py = pybind11;

struct Sentence
{
    std::string text;
    int start_pos;
    int end_pos;
    double semantic_weight;
    std::vector<std::string> keywords;
    std::string section_type; // paragraph, list_item, header, etc.
};

struct SemanticChunk
{
    std::string text;
    double coherence_score;
    std::vector<std::string> dominant_topics;
    int sentence_count;
    std::string primary_section_type;
};

class SemanticTextChunker
{
private:
    std::unordered_set<std::string> stopwords = {
        "the", "a", "an", "and", "or", "but", "in", "on", "at", "to", "for",
        "of", "with", "by", "is", "are", "was", "were", "be", "been", "being",
        "have", "has", "had", "do", "does", "did", "will", "would", "could",
        "should", "may", "might", "can", "must", "shall", "this", "that",
        "these", "those", "i", "you", "he", "she", "it", "we", "they",
        "me", "him", "her", "us", "them", "my", "your", "his", "her", "its",
        "our", "their", "mine", "yours", "ours", "theirs"};

    std::vector<std::string> discourse_markers = {
        "however", "nevertheless", "moreover", "furthermore", "in addition",
        "on the other hand", "in contrast", "similarly", "likewise",
        "therefore", "thus", "consequently", "as a result", "meanwhile",
        "subsequently", "previously", "initially", "finally", "in conclusion",
        "to summarize", "for example", "for instance", "specifically",
        "in particular", "namely", "that is", "in other words"};

    std::unordered_map<std::string, std::shared_ptr<std::regex>> section_patterns;

public:
    SemanticTextChunker();

    std::vector<std::string> chunk_text_semantically(
        const std::string &text,
        int max_chunk_size = 2000,
        int min_chunk_size = 500,
        double min_coherence_threshold = 0.3);

    std::vector<py::dict> get_chunk_details(
        const std::string &text,
        int max_chunk_size = 2000,
        int min_chunk_size = 500,
        double min_coherence_threshold = 0.3);

private:
    std::vector<SemanticChunk> chunk_semantically(
        const std::string &text,
        int max_chunk_size = 2000,
        int min_chunk_size = 500,
        double min_coherence_threshold = 0.3);

    std::string trim(const std::string &str);
    std::string to_lower(const std::string &str);
    std::vector<std::string> tokenize(const std::string &text);
    std::vector<Sentence> extract_sentences(const std::string &text);
    std::string detect_section_type(const std::string &text);
    double calculate_semantic_weight(const std::string &sentence);
    std::vector<std::string> extract_keywords(const std::string &text);
    double calculate_coherence_score(const std::vector<Sentence> &sentences);
    std::vector<std::string> identify_dominant_topics(const std::vector<Sentence> &sentences);
    std::vector<SemanticChunk> merge_small_chunks(const std::vector<SemanticChunk> &chunks, int min_chunk_size);
    std::vector<SemanticChunk> split_large_chunks(const std::vector<SemanticChunk> &chunks, int max_chunk_size);
};

std::vector<std::string> chunk_text_semantically_standalone(
    const std::string &text,
    int max_chunk_size = 2000,
    int min_chunk_size = 500,
    double min_coherence_threshold = 0.3);

std::vector<py::dict> get_chunk_details_standalone(
    const std::string &text,
    int max_chunk_size = 2000,
    int min_chunk_size = 500,
    double min_coherence_threshold = 0.3);

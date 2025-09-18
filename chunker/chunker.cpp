#include "chunker.h"

SemanticTextChunker::SemanticTextChunker()
{
    try
    {
        section_patterns["header"] = std::make_shared<std::regex>(R"(^#+\s+|\n#+\s+|^[A-Z][A-Za-z\s]+:|\n[A-Z][A-Za-z\s]+:)");
        section_patterns["list_item"] = std::make_shared<std::regex>(R"(^\s*[-*+•]\s+|\n\s*[-*+•]\s+|^\s*\d+\.\s+|\n\s*\d+\.\s+)");
        section_patterns["quote"] = std::make_shared<std::regex>(R"(^\s*>\s+|\n\s*>\s+|^\s*[""']\s*|\s*[""']\s*$)");
        section_patterns["code"] = std::make_shared<std::regex>(R"(```|`[^`]+`|^\s{4,}|\t)");
        section_patterns["dialogue"] = std::make_shared<std::regex>(R"(^\s*[""'"][^""'']+[""'']\s*$|^\s*-\s*[A-Z])");
        section_patterns["paragraph"] = std::make_shared<std::regex>(R"(.*)");
    }
    catch (const std::regex_error &e)
    {
        section_patterns.clear();
    }
}

std::string SemanticTextChunker::trim(const std::string &str)
{
    if (str.empty())
        return "";
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos)
        return "";
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

std::string SemanticTextChunker::to_lower(const std::string &str)
{
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::vector<std::string> SemanticTextChunker::tokenize(const std::string &text)
{
    if (text.empty())
        return {};

    std::vector<std::string> tokens;
    try
    {
        std::regex word_regex(R"(\b\w+\b)");
        std::sregex_iterator iter(text.begin(), text.end(), word_regex);
        std::sregex_iterator end;

        for (; iter != end; ++iter)
        {
            std::string word = to_lower(iter->str());
            if (stopwords.find(word) == stopwords.end() && word.length() > 2)
            {
                tokens.push_back(word);
            }
        }
    }
    catch (const std::regex_error &e)
    {
        std::istringstream iss(text);
        std::string word;
        while (iss >> word)
        {
            std::string clean_word = to_lower(word);
            if (stopwords.find(clean_word) == stopwords.end() && clean_word.length() > 2)
            {
                tokens.push_back(clean_word);
            }
        }
    }
    return tokens;
}

std::string SemanticTextChunker::detect_section_type(const std::string &text)
{
    for (const auto &[type, pattern] : section_patterns)
    {
        if (type != "paragraph" && std::regex_search(text, *pattern))
        {
            return type;
        }
    }
    return "paragraph";
}

std::vector<Sentence> SemanticTextChunker::extract_sentences(const std::string &text)
{
    std::vector<Sentence> sentences;
    if (text.empty())
        return sentences;

    try
    {
        std::regex sentence_regex(R"([.!?]+)");
        std::sregex_token_iterator iter(text.begin(), text.end(), sentence_regex, -1);
        std::sregex_token_iterator end;

        int current_pos = 0;
        for (; iter != end; ++iter)
        {
            std::string sentence_text = trim(iter->str());
            if (!sentence_text.empty())
            {
                Sentence sentence;
                sentence.text = sentence_text;
                sentence.start_pos = current_pos;
                sentence.end_pos = current_pos + sentence_text.length();
                sentence.semantic_weight = calculate_semantic_weight(sentence_text);
                sentence.keywords = extract_keywords(sentence_text);
                sentence.section_type = detect_section_type(sentence_text);
                sentences.push_back(sentence);
                current_pos += sentence_text.length() + 1;
            }
        }
    }
    catch (const std::regex_error &e)
    {
        std::istringstream iss(text);
        std::string line;
        int current_pos = 0;
        while (std::getline(iss, line))
        {
            std::string sentence_text = trim(line);
            if (!sentence_text.empty())
            {
                Sentence sentence;
                sentence.text = sentence_text;
                sentence.start_pos = current_pos;
                sentence.end_pos = current_pos + sentence_text.length();
                sentence.semantic_weight = calculate_semantic_weight(sentence_text);
                sentence.keywords = extract_keywords(sentence_text);
                sentence.section_type = detect_section_type(sentence_text);
                sentences.push_back(sentence);
                current_pos += sentence_text.length() + 1;
            }
        }
    }
    return sentences;
}

double SemanticTextChunker::calculate_semantic_weight(const std::string &sentence)
{
    if (sentence.empty())
        return 0.0;

    std::vector<std::string> tokens = tokenize(sentence);
    if (tokens.empty())
        return 0.0;

    double weight = 0.0;
    int keyword_count = 0;

    for (const auto &token : tokens)
    {
        if (stopwords.find(token) == stopwords.end())
        {
            keyword_count++;
            weight += 1.0;
        }
    }

    if (keyword_count == 0)
        return 0.0;

    double avg_weight = weight / keyword_count;

    for (const auto &marker : discourse_markers)
    {
        if (sentence.find(marker) != std::string::npos)
        {
            avg_weight *= 1.2;
            break;
        }
    }

    return std::min(avg_weight, 10.0);
}

std::vector<std::string> SemanticTextChunker::extract_keywords(const std::string &text)
{
    std::vector<std::string> tokens = tokenize(text);
    std::vector<std::string> keywords;

    for (const auto &token : tokens)
    {
        if (stopwords.find(token) == stopwords.end() && token.length() > 3)
        {
            keywords.push_back(token);
        }
    }

    return keywords;
}

double SemanticTextChunker::calculate_coherence_score(const std::vector<Sentence> &sentences)
{
    if (sentences.size() <= 1)
        return 1.0;

    double total_coherence = 0.0;
    int comparisons = 0;

    for (size_t i = 0; i < sentences.size() - 1; ++i)
    {
        const auto &sentence1 = sentences[i];
        const auto &sentence2 = sentences[i + 1];

        double keyword_overlap = 0.0;
        if (!sentence1.keywords.empty() && !sentence2.keywords.empty())
        {
            std::unordered_set<std::string> keywords1(sentence1.keywords.begin(), sentence1.keywords.end());
            std::unordered_set<std::string> keywords2(sentence2.keywords.begin(), sentence2.keywords.end());

            int common_keywords = 0;
            for (const auto &keyword : keywords1)
            {
                if (keywords2.find(keyword) != keywords2.end())
                {
                    common_keywords++;
                }
            }

            keyword_overlap = static_cast<double>(common_keywords) /
                              std::max(keywords1.size(), keywords2.size());
        }

        double semantic_similarity = std::min(sentence1.semantic_weight, sentence2.semantic_weight) /
                                     std::max(sentence1.semantic_weight, sentence2.semantic_weight);

        double coherence = (keyword_overlap * 0.6) + (semantic_similarity * 0.4);
        total_coherence += coherence;
        comparisons++;
    }

    return comparisons > 0 ? total_coherence / comparisons : 1.0;
}

std::vector<std::string> SemanticTextChunker::identify_dominant_topics(const std::vector<Sentence> &sentences)
{
    std::unordered_map<std::string, int> topic_counts;

    for (const auto &sentence : sentences)
    {
        for (const auto &keyword : sentence.keywords)
        {
            topic_counts[keyword]++;
        }
    }

    std::vector<std::pair<std::string, int>> sorted_topics(topic_counts.begin(), topic_counts.end());
    std::sort(sorted_topics.begin(), sorted_topics.end(),
              [](const auto &a, const auto &b)
              { return a.second > b.second; });

    std::vector<std::string> dominant_topics;
    for (size_t i = 0; i < std::min(size_t(5), sorted_topics.size()); ++i)
    {
        dominant_topics.push_back(sorted_topics[i].first);
    }

    return dominant_topics;
}

std::vector<SemanticChunk> SemanticTextChunker::merge_small_chunks(const std::vector<SemanticChunk> &chunks, int min_chunk_size)
{
    std::vector<SemanticChunk> merged_chunks;

    for (const auto &chunk : chunks)
    {
        if (chunk.text.length() < min_chunk_size && !merged_chunks.empty())
        {
            auto &last_chunk = merged_chunks.back();
            last_chunk.text += " " + chunk.text;
            last_chunk.sentence_count += chunk.sentence_count;
            last_chunk.dominant_topics.insert(last_chunk.dominant_topics.end(),
                                              chunk.dominant_topics.begin(), chunk.dominant_topics.end());
        }
        else
        {
            merged_chunks.push_back(chunk);
        }
    }

    return merged_chunks;
}

std::vector<SemanticChunk> SemanticTextChunker::split_large_chunks(const std::vector<SemanticChunk> &chunks, int max_chunk_size)
{
    std::vector<SemanticChunk> split_chunks;

    for (const auto &chunk : chunks)
    {
        if (chunk.text.length() <= max_chunk_size)
        {
            split_chunks.push_back(chunk);
        }
        else
        {
            std::vector<Sentence> sentences = extract_sentences(chunk.text);
            std::vector<SemanticChunk> sub_chunks;

            std::string current_text;
            std::vector<Sentence> current_sentences;

            for (const auto &sentence : sentences)
            {
                if (current_text.length() + sentence.text.length() + 1 <= max_chunk_size)
                {
                    if (!current_text.empty())
                        current_text += " ";
                    current_text += sentence.text;
                    current_sentences.push_back(sentence);
                }
                else
                {
                    if (!current_text.empty())
                    {
                        SemanticChunk sub_chunk;
                        sub_chunk.text = current_text;
                        sub_chunk.coherence_score = calculate_coherence_score(current_sentences);
                        sub_chunk.dominant_topics = identify_dominant_topics(current_sentences);
                        sub_chunk.sentence_count = current_sentences.size();
                        sub_chunk.primary_section_type = current_sentences.empty() ? "paragraph" : current_sentences[0].section_type;
                        sub_chunks.push_back(sub_chunk);
                    }

                    current_text = sentence.text;
                    current_sentences = {sentence};
                }
            }

            if (!current_text.empty())
            {
                SemanticChunk sub_chunk;
                sub_chunk.text = current_text;
                sub_chunk.coherence_score = calculate_coherence_score(current_sentences);
                sub_chunk.dominant_topics = identify_dominant_topics(current_sentences);
                sub_chunk.sentence_count = current_sentences.size();
                sub_chunk.primary_section_type = current_sentences.empty() ? "paragraph" : current_sentences[0].section_type;
                sub_chunks.push_back(sub_chunk);
            }

            split_chunks.insert(split_chunks.end(), sub_chunks.begin(), sub_chunks.end());
        }
    }

    return split_chunks;
}

std::vector<SemanticChunk> SemanticTextChunker::chunk_semantically(
    const std::string &text,
    int max_chunk_size,
    int min_chunk_size,
    double min_coherence_threshold)
{
    try
    {
        if (text.empty())
        {
            return {};
        }

        if (max_chunk_size <= 0 || min_chunk_size <= 0 || min_coherence_threshold < 0.0 || min_coherence_threshold > 1.0)
        {
            return {};
        }

        std::vector<Sentence> sentences = extract_sentences(text);
        if (sentences.empty())
        {
            return {};
        }

        std::vector<SemanticChunk> chunks;
        std::vector<Sentence> current_chunk_sentences;
        std::string current_chunk_text;

        for (const auto &sentence : sentences)
        {
            std::string potential_text = current_chunk_text.empty() ? sentence.text : current_chunk_text + " " + sentence.text;

            if (potential_text.length() <= max_chunk_size)
            {
                current_chunk_text = potential_text;
                current_chunk_sentences.push_back(sentence);
            }
            else
            {
                if (!current_chunk_text.empty())
                {
                    double coherence = calculate_coherence_score(current_chunk_sentences);
                    if (coherence >= min_coherence_threshold)
                    {
                        SemanticChunk chunk;
                        chunk.text = current_chunk_text;
                        chunk.coherence_score = coherence;
                        chunk.dominant_topics = identify_dominant_topics(current_chunk_sentences);
                        chunk.sentence_count = current_chunk_sentences.size();
                        chunk.primary_section_type = current_chunk_sentences.empty() ? "paragraph" : current_chunk_sentences[0].section_type;
                        chunks.push_back(chunk);
                    }
                }

                current_chunk_text = sentence.text;
                current_chunk_sentences = {sentence};
            }
        }

        if (!current_chunk_text.empty())
        {
            double coherence = calculate_coherence_score(current_chunk_sentences);
            if (coherence >= min_coherence_threshold)
            {
                SemanticChunk chunk;
                chunk.text = current_chunk_text;
                chunk.coherence_score = coherence;
                chunk.dominant_topics = identify_dominant_topics(current_chunk_sentences);
                chunk.sentence_count = current_chunk_sentences.size();
                chunk.primary_section_type = current_chunk_sentences.empty() ? "paragraph" : current_chunk_sentences[0].section_type;
                chunks.push_back(chunk);
            }
        }

        chunks = merge_small_chunks(chunks, min_chunk_size);
        chunks = split_large_chunks(chunks, max_chunk_size);

        return chunks;
    }
    catch (const std::exception &e)
    {
        return {};
    }
}

std::vector<std::string> SemanticTextChunker::chunk_text_semantically(
    const std::string &text,
    int max_chunk_size,
    int min_chunk_size,
    double min_coherence_threshold)
{
    auto semantic_chunks = chunk_semantically(text, max_chunk_size, min_chunk_size, min_coherence_threshold);
    std::vector<std::string> text_chunks;

    for (const auto &chunk : semantic_chunks)
    {
        text_chunks.push_back(chunk.text);
    }

    return text_chunks;
}

std::vector<py::dict> SemanticTextChunker::get_chunk_details(
    const std::string &text,
    int max_chunk_size,
    int min_chunk_size,
    double min_coherence_threshold)
{
    try
    {
        auto semantic_chunks = chunk_semantically(text, max_chunk_size, min_chunk_size, min_coherence_threshold);
        std::vector<py::dict> chunk_details;

        for (const auto &chunk : semantic_chunks)
        {
            py::dict chunk_dict;
            chunk_dict["text"] = chunk.text;
            chunk_dict["coherence_score"] = chunk.coherence_score;
            chunk_dict["dominant_topics"] = chunk.dominant_topics;
            chunk_dict["sentence_count"] = chunk.sentence_count;
            chunk_dict["primary_section_type"] = chunk.primary_section_type;
            chunk_details.push_back(chunk_dict);
        }

        return chunk_details;
    }
    catch (const std::exception &e)
    {
        return {};
    }
}

std::vector<std::string> chunk_text_semantically_standalone(
    const std::string &text,
    int max_chunk_size,
    int min_chunk_size,
    double min_coherence_threshold)
{
    try
    {
        SemanticTextChunker chunker;
        return chunker.chunk_text_semantically(text, max_chunk_size, min_chunk_size, min_coherence_threshold);
    }
    catch (const std::exception &e)
    {
        return {};
    }
}

std::vector<py::dict> get_chunk_details_standalone(
    const std::string &text,
    int max_chunk_size,
    int min_chunk_size,
    double min_coherence_threshold)
{
    try
    {
        SemanticTextChunker chunker;
        return chunker.get_chunk_details(text, max_chunk_size, min_chunk_size, min_coherence_threshold);
    }
    catch (const std::exception &e)
    {
        return {};
    }
}

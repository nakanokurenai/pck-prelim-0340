#include <vector>
#include <iostream>
#include <utility>
#include <iterator>
#include <map>
#include <algorithm>

typedef std::vector<int> Max;
typedef std::vector<int> Input;
typedef std::vector<int> Section;
typedef std::map<int, int> ScoreResult;

std::map<std::string, Section> get_great_sections (Input input) {
    std::size_t input_size = input.size();

    /**
     * Find max
     */
    int max_value = 1;
    Max max;

    for (int index = 0; index < input_size; index++) {
        if (input[index] == 0) continue;
        int value = 1;
        if ((index != 0) && (index != input_size-1)) {
            value = input[index];
        }
        if (value < max_value) {
            continue;
        }
        if (value > max_value) {
            max.erase(max.begin(), max.end());
            max_value = value;
        }
        max.push_back(index);
    }

    if (max.size() == 0) {
        std::map<std::string, Section> empty_section;
        return empty_section;
    }

    // Find sections
    std::map<std::string, Section> sections;
    for (int index : max) {
        // [index, index+1]
        std::string key_upper = "";
        Section section_upper(0);
        key_upper += std::to_string(index);
        section_upper.push_back(index);
        if (index != input_size-1) {
            key_upper += std::to_string(index+1);
            section_upper.push_back(index+1);
        }
        sections[key_upper] = section_upper;

        // [index-1, index]
        std::string key_lower = "";
        Section section_lower(0);
        if (index != 0) {
            key_lower += std::to_string(index-1);
            section_lower.push_back(index-1);
        }
        key_lower += std::to_string(index);
        section_lower.push_back(index);
        sections[key_lower] = section_lower;
    }

    /**
     * scores sections and get 
     */
    // score with counting zero
    int score_zero = -2;
    std::map<std::string, Section> sections_zero_scored;
    for (auto iterator = sections.begin(); iterator != sections.end(); iterator++) {
        Section section = iterator->second;
        int score = 0;
        int size = section.size();
        if (input[section[0]] == 0) {
            score -= 1;
        }
        if (size > 1 && input[section[1]] == 0) {
            score -= 1;
        }

        if (score_zero > score) {
            continue;
        }
        if (score > score_zero) {
            score_zero = score;
            sections_zero_scored.erase(sections_zero_scored.begin(), sections_zero_scored.end());
        }
        sections_zero_scored[iterator->first] = section;
    }

    // score with components
    int score_component = 1;
    std::map<std::string, Section> sections_scored_component;
    for (auto iterator = sections_zero_scored.begin(); iterator != sections_zero_scored.end(); iterator++) {
        Section section = iterator->second;
        std::size_t score = section.size();

        if (score_component > score) {
            continue;
        }
        if (score > score_component) {
            score_component = score;
            sections_scored_component.erase(sections_scored_component.begin(), sections_scored_component.end());
        }
        sections_scored_component[iterator->first] = section;
    }

    // score with difficulty to delete
    bool score_difficulty = false;
    std::map<std::string, Section> sections_scored_difficulty;
    for (auto iterator = sections_scored_component.begin(); iterator != sections_scored_component.end(); iterator++) {
        Section section = iterator->second;
        int max = input_size - 1;
        bool score = true;

        // index pairs that has case [0, x] or [1, x]
        if (section[0] < 2) {
            if (section[0] == 0 || (section[0] == 1 && input[0] != 0)) {
                score = false;   
            }
        }

        // index pairs that has case [x, max] or [x, max-1]
        if (max - 2 < section[1]) {
            if (section[1] == max) {
                score = false;
            }
        }

        if (!score && score_difficulty) break;

        if (score && !score_difficulty) {
            score_difficulty = score;
            sections_scored_difficulty.erase(sections_scored_difficulty.begin(), sections_scored_difficulty.end());
        }
        sections_scored_difficulty[iterator->first] = section;
    }

    if (score_difficulty) return sections_scored_difficulty;

    // score with distance from first or last
    bool score_distance = 0;
    std::map<std::string, Section> sections_scored_distance;

    for (auto iterator = sections_scored_difficulty.begin(); iterator != sections_scored_difficulty.end(); iterator++) {
        Section section = iterator->second;
        int max = input_size - 1;
        int score_first = 2;
        int score_last = 2;

        // score pairs that has case [0, x] or [1, x]
        if (section[0] < 2) score_first = section[0];

        // score pairs that has case [x, max] or [x, max-1]
        if (max - 2 < section[1]) score_last = max - section[1];

        int score = std::min(score_first, score_last);

        if (score_distance > score) continue;
        if (score > score_distance) {
            score_distance = score;
            sections_scored_distance.erase(sections_scored_distance.begin(), sections_scored_distance.end());
        }
        sections_scored_distance[iterator->first] = section;
    }

    return sections_scored_distance;
}

int main () {
    int input_size;
    std::cin >> input_size;
    Input input(0);

    for (int i = 0; i < input_size; i++) {
        int tmp;
        std::cin >> tmp;
        input.push_back(tmp);
    }

    if (input.size() != input_size) {
        std::cerr << "expected size " << input.size() << " but expect " << input_size << std::endl;
        exit(1);
    }

    int cnt = 0;

    while (true) {
        auto section_map = get_great_sections(input);
        if (section_map.size() == 0) break;

        for (int a : input) {
            std::cerr << a << ", ";
        }
        std::cerr << "cnt: " << cnt << std::endl;

        auto section = (*section_map.begin()).second;
        if (section.size() == 1) {
            cnt += input[section[0]];
            input[section[0]] = 0;
            continue;
        }
        int min = std::min(input[section[0]], input[section[1]]);
        if (min != 0) {
            cnt += min * 2;
            input[section[0]] -= min;
            input[section[1]] -= min;
            continue;
        }
        int max = std::max(input[section[0]], input[section[1]]);
        cnt += max*2;
        input[section[0]] = input[section[1]] = 0;
    }

    std::cout << cnt << std::endl;
}

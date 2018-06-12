#include <vector>
#include <iostream>
#include <utility>
#include <iterator>
#include <map>

typedef std::vector<int> Max;
typedef std::vector<int> Input;
typedef std::vector<int> Section;
typedef std::map<int, int> ScoreResult;

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

    // Find max
    int max_value = 1;
    Max max;

    for (int index = 0; index < input_size; index++) {
        int value = input[index];
        if (value < max_value) {
            continue;
        }
        if (value > max_value) {
            max.erase(max.begin(), max.end());
            max_value = value;
        }
        max.push_back(index);
        std::cerr << "Found max as " << value << ", on [" << index << "]" << std::endl;
    }

    if (max.size() == 0) {
        std::cerr << "All value is zero." << std::endl;
        return 0;
    }

    // Find sections
    std::map<std::string, Section> sections;
    for (int index : max) {
        // [index, index+1]
        std::string key_upper = "";
        Section section_upper(0);
        key_upper += std::to_string(index);
        section_upper.push_back(index);
        if (index != input_size) {
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

    std::cerr << "[!] Found " << sections.size() << " sections" << std::endl;
    for (auto iterator = sections.begin(); iterator != sections.end(); ++iterator) {
        Section section = iterator->second;
        std::cerr << "* [";
        std::cerr << section[0];
        if (section.size() > 1) {
            std::cerr << ", " << section[1];
        }
        std::cerr << "]" << std::endl;
    }

    // scoreling sections


    return 0;
}

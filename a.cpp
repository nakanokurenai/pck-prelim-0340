#include <vector>
#include <algorithm>
#include <iostream>
#include <utility>
#include <iterator>

int main () {
    int N;
    std::cin >> N;
    std::vector<int> v(N);
    for (int i = 0; i < N; i++) {
        std::cin >> v[i];
    }
    int cnt = 0;
    while (true) {
        auto max_it = std::max_element(v.begin(), v.end());
        int max = *max_it;
        std::size_t index = std::distance(v.begin(), max_it);
        std::cerr << "Max is " << max << ", index is " << index << std::endl;
        if (max == 0) {
            break;
        }
        if (index > 0 && v[index-1] > 0) {
            std::cerr << "Remove with left value " << v[index-1] << std::endl;
            int min = std::min(max, v[index-1]);
            cnt += min * 2;
            v[index] -= min;
            v[index-1] -= min;
            continue;
        }
        if (index < N && v[index+1] > 0) {
            std::cerr << "Remove with right value " << v[index-1] << std::endl;
            int min = std::min(max, v[index+1]);
            cnt += min * 2;
            v[index] -= min;
            v[index+1] -= min;
            continue;
        }
        std::cerr << "Remove single value" << std::endl;
        if (index == 0 || index == N-1) {
            std::cerr << "Hashikko" << std::endl;
            cnt += max;
        } else {
            cnt += max *2;
        }
        v[index] = 0;
    }
    std::cout << cnt << std::endl;
    return 0;
}

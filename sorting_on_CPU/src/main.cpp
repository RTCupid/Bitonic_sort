#include <iostream>
#include <vector>
#include <algorithm>
#include "utils.hpp"


int main() {
    std::vector<int> data = {5, 2, 9, 1, 5, 6};
    
    yLab::dump(data);
    std::sort(data.begin(), data.end());
    yLab::dump(data);

}
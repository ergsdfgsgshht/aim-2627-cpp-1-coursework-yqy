#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <limits>
#include <memory>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

// Assessment 01 Q5; see 题面.pdf.
namespace aim::q5 {


// BEGIN IMPLEMENTATION
std::vector<std::vector<int>> risk(const std::vector<std::string>& grid) {
    const std::size_t rows = grid.size();
    if (rows == 0) {
        return {};
    }
    // 第一步：完整验证整张地图，任何一项不满足都抛异常，不产出部分结果
    const std::size_t cols = grid[0].size();
    if (cols == 0) {
        throw std::invalid_argument("map has a row with zero columns");
    }
    for (const std::string& row : grid) {
        if (row.size() != cols) {
            throw std::invalid_argument("map rows have inconsistent lengths");
        }
        for (char cell : row) {
            if (cell != '.' && cell != '#') {
                throw std::invalid_argument("map contains a character other than . and #");
            }
        }
    }
    // 第二步：计算风险图；越界方向不算障碍，斜对角不算邻居
    std::vector<std::vector<int>> out(rows, std::vector<int>(cols, 0));
    for (std::size_t r = 0; r < rows; ++r) {
        for (std::size_t c = 0; c < cols; ++c) {
            if (grid[r][c] == '#') {
                out[r][c] = -1;
                continue;
            }
            int count = 0;
            if (r > 0 && grid[r - 1][c] == '#') ++count;          // 上
            if (r + 1 < rows && grid[r + 1][c] == '#') ++count;    // 下
            if (c > 0 && grid[r][c - 1] == '#') ++count;           // 左
            if (c + 1 < cols && grid[r][c + 1] == '#') ++count;    // 右
            out[r][c] = count;
        }
    }
    return out;
}
// END IMPLEMENTATION
}

#define CHECK(...) do { if (!(__VA_ARGS__)) throw std::runtime_error("check failed"); } while(false)
#define EXPECT_INVALID(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::invalid_argument&) { caught=true; } CHECK(caught); } while(false)
#define EXPECT_RANGE(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::out_of_range&) { caught=true; } CHECK(caught); } while(false)

using namespace aim::q5;
int main() {
    try {
        { CHECK((risk({".#",".."})==std::vector<std::vector<int>>{{1,-1},{0,1}})); }
        { CHECK(risk({}).empty()); }
        std::cout << "PUBLIC 2/2\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}

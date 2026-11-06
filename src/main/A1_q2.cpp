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

// Assessment 01 Q2; see 题面.pdf.
namespace aim::q2 {
struct LabelEdit { std::string from; std::string to; };

// BEGIN IMPLEMENTATION
// aim-cpp-cw
std::string edit_labels(std::string labels, const std::vector<LabelEdit>& edits) {
    // 按逗号拆成完整标签列表再逐条编辑：整标签比较天然排除子串误命中，
    // 首尾/连续逗号问题交给最后的重组，不必在原字符串上拼接着删。
    std::vector<std::string> tags;
    if (!labels.empty()) {
        std::string::size_type start = 0;
        while (true) {
            std::string::size_type comma = labels.find(',', start);
            if (comma == std::string::npos) {
                tags.push_back(labels.substr(start));
                break;
            }
            tags.push_back(labels.substr(start, comma - start));
            start = comma + 1;
        }
    }

    for (const LabelEdit& edit : edits) {
        if (edit.from.empty()) {
            // from 为空：to 非空才追加到末尾，重组时自动补分隔逗号；两者均空不操作。
            if (!edit.to.empty()) tags.push_back(edit.to);
            continue;
        }
        // 每次编辑都基于编辑后的当前列表重新查找第一个完整命中；下标 0 也是有效命中。
        std::vector<std::string>::iterator hit = std::find(tags.begin(), tags.end(), edit.from);
        if (hit == tags.end()) continue;  // 未命中：不修改
        if (edit.to.empty()) {
            tags.erase(hit);  // 删除该标签，相邻逗号随重组消失
        } else {
            *hit = edit.to;  // 只替换/处理首次命中一次
        }
    }

    std::string result;
    for (std::size_t i = 0; i < tags.size(); ++i) {
        if (i > 0) result += ',';
        result += tags[i];
    }
    return result;
}
// END IMPLEMENTATION
}

#define CHECK(...) do { if (!(__VA_ARGS__)) throw std::runtime_error("check failed"); } while(false)
#define EXPECT_INVALID(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::invalid_argument&) { caught=true; } CHECK(caught); } while(false)
#define EXPECT_RANGE(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::out_of_range&) { caught=true; } CHECK(caught); } while(false)

using namespace aim::q2;
int main() {
    try {
        { CHECK(edit_labels("cam,imu,radar",{{"imu","gyro"},{"cam",""},{"","lidar"}})=="gyro,radar,lidar"); }
        { CHECK(edit_labels("cam,cam2,cam",{{"cam","camera"},{"cam",""}})=="camera,cam2"); }
        std::cout << "PUBLIC 2/2\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}

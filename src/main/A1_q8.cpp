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

// Assessment 01 Q8; see 题面.pdf.
namespace aim::q8 {
struct Telemetry { int id; int hp; };
struct Summary { int valid; int duplicate; int invalid; long long total_hp; std::vector<int> ids; };

// BEGIN IMPLEMENTATION
Summary summarize(const std::vector<Telemetry>& frames) {
    Summary result{0, 0, 0, 0, {}};
    // 编号标记表: 每次调用局部新建并清零, 不共享状态
    std::array<bool, 10000> seen{};
    for (const Telemetry& f : frames) {
        // 第一步范围验证: 任一字段不满足计 invalid, 不占用编号;
        // 必须先确认 id 在 [0,9999] 才能用它访问标记表
        if (f.id < 0 || f.id > 9999 || f.hp < -1000 || f.hp > 1000) {
            ++result.invalid;
            continue;
        }
        // 第二步判重: 仅在完全合法后, id 已有首次合法记录才算 duplicate
        if (seen[static_cast<std::size_t>(f.id)]) {
            ++result.duplicate;
            continue;
        }
        // 首次合法: 记编号, hp 夹到 [0,100] 后累加到 long long
        seen[static_cast<std::size_t>(f.id)] = true;
        ++result.valid;
        const int clamped = f.hp < 0 ? 0 : (f.hp > 100 ? 100 : f.hp);
        result.total_hp += clamped;
    }
    // 按编号顺序扫描标记表, ids 天然升序且无重复
    for (int id = 0; id < 10000; ++id) {
        if (seen[static_cast<std::size_t>(id)]) {
            result.ids.push_back(id);
        }
    }
    return result;
}
// END IMPLEMENTATION
}

#define CHECK(...) do { if (!(__VA_ARGS__)) throw std::runtime_error("check failed"); } while(false)
#define EXPECT_INVALID(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::invalid_argument&) { caught=true; } CHECK(caught); } while(false)
#define EXPECT_RANGE(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::out_of_range&) { caught=true; } CHECK(caught); } while(false)

using namespace aim::q8;
int main() {
    try {
        { aim::q8::Summary r=summarize({{2,200},{1,-5},{2,10},{-1,30}}); CHECK(r.valid==2 && r.duplicate==1 && r.invalid==1 && r.total_hp==100 && (r.ids==std::vector<int>{1,2})); }
        { aim::q8::Summary r=summarize({{1,-1001},{1,30},{1,1001}}); CHECK(r.valid==1 && r.duplicate==0 && r.invalid==2 && r.total_hp==30 && (r.ids==std::vector<int>{1})); }
        std::cout << "PUBLIC 2/2\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}

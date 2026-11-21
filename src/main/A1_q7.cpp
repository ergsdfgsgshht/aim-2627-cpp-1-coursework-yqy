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

// Assessment 01 Q7; see 题面.pdf.
namespace aim::q7 {
struct FireResult { int shots; int rejected; int ammo; int heat; };

// BEGIN IMPLEMENTATION
FireResult simulate(int ammo, int heat, int cool, int shot_heat, const std::vector<int>& requests) {
    FireResult result{0, 0, ammo, heat};
    for (int request : requests) {
        // 每帧第一步固定为冷却; 为负归零
        result.heat -= cool;
        if (result.heat < 0) {
            result.heat = 0;
        }
        if (request == 1) {
            // 用冷却后的热量判定; 等于上限 100 仍允许发射
            const bool can_fire = result.ammo > 0 && result.heat + shot_heat <= 100;
            if (can_fire) {
                --result.ammo;
                result.heat += shot_heat;
                ++result.shots;
            } else {
                // 拒绝不扣弹、不加热量
                ++result.rejected;
            }
        }
        // 请求为 0: 本帧只执行了冷却
    }
    return result;
}
// END IMPLEMENTATION
}

#define CHECK(...) do { if (!(__VA_ARGS__)) throw std::runtime_error("check failed"); } while(false)
#define EXPECT_INVALID(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::invalid_argument&) { caught=true; } CHECK(caught); } while(false)
#define EXPECT_RANGE(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::out_of_range&) { caught=true; } CHECK(caught); } while(false)

using namespace aim::q7;
int main() {
    try {
        { aim::q7::FireResult r=simulate(2,90,10,20,{1,1,1}); CHECK(r.shots==2 && r.rejected==1 && r.ammo==0 && r.heat==100); }
        { aim::q7::FireResult r=simulate(3,100,20,10,{0}); CHECK(r.heat==80 && r.rejected==0 && r.ammo==3); }
        std::cout << "PUBLIC 2/2\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}

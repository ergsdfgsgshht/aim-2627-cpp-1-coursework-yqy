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

// Assessment 01 Q1; see 题面.pdf.
namespace aim::q1 {


// BEGIN IMPLEMENTATION
long long ratio(long long hp, long long max_hp) {
    if (max_hp <= 0) return -1;
    if (hp < 0) hp = 0;
    if (hp > max_hp) hp = max_hp;
    return hp * 100 / max_hp;
}
std::string report(int id, long long hp, long long max_hp, int battery) {
    const long long percent = ratio(hp, max_hp);
    // 电量先夹到 [0,100], 再按规范化后的值分档; 不存在 INVALID 档位
    if (battery < 0) battery = 0;
    if (battery > 100) battery = 100;
    const char* state = battery >= 60 ? "OK" : (battery >= 20 ? "WARNING" : "LOW");
    std::string result = "ID=" + std::to_string(id)
                   + ";HP=" + (percent == -1 ? std::string("INVALID") : std::to_string(percent))
                   + ";BAT=" + std::to_string(battery)
                   + ";STATE=" + state;
    return result;
}
// END IMPLEMENTATION
}

#define CHECK(...) do { if (!(__VA_ARGS__)) throw std::runtime_error("check failed"); } while(false)
#define EXPECT_INVALID(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::invalid_argument&) { caught=true; } CHECK(caught); } while(false)
#define EXPECT_RANGE(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::out_of_range&) { caught=true; } CHECK(caught); } while(false)

using namespace aim::q1;
int main() {
    try {
        { CHECK(ratio(2,3)==66); }
        { CHECK(report(7,2,3,20)=="ID=7;HP=66;BAT=20;STATE=WARNING"); }
        std::cout << "PUBLIC 2/2\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}

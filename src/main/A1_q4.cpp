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

// Assessment 01 Q4; see 题面.pdf.
namespace aim::q4 {
struct CommandResult { long long value; int accepted; int rejected; };

// BEGIN IMPLEMENTATION
CommandResult execute(const std::vector<std::string>& lines) {
    CommandResult result{0, 0, 0};
    constexpr long long kLimit = 1000000LL;
    for (const std::string& line : lines) {
        std::istringstream input(line);
        std::string op;
        bool ok = false;
        long long candidate = 0;
        if (input >> op) {
            if (op == "ADD" || op == "SET") {
                long long x = 0;
                std::string extra;
                // 参数必须恰好一个：能读出一个整数，且之后读不出任何多余字段
                if (input >> x && !(input >> extra) && x >= -kLimit && x <= kLimit) {
                    candidate = (op == "ADD") ? result.value + x : x;
                    ok = candidate >= -kLimit && candidate <= kLimit;
                }
            } else if (op == "RESET") {
                std::string extra;
                if (!(input >> extra)) {  // RESET 不允许任何参数
                    candidate = 0;
                    ok = true;
                }
            }
        }
        if (ok) {
            result.value = candidate;
            ++result.accepted;
        } else {
            ++result.rejected;
        }
    }
    return result;
}
// END IMPLEMENTATION
}

#define CHECK(...) do { if (!(__VA_ARGS__)) throw std::runtime_error("check failed"); } while(false)
#define EXPECT_INVALID(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::invalid_argument&) { caught=true; } CHECK(caught); } while(false)
#define EXPECT_RANGE(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::out_of_range&) { caught=true; } CHECK(caught); } while(false)

using namespace aim::q4;
int main() {
    try {
        { aim::q4::CommandResult r=execute({"ADD 5","SET 8 x","RESET","ADD -2"}); CHECK(r.value==-2 && r.accepted==3 && r.rejected==1); }
        { aim::q4::CommandResult r=execute({}); CHECK(r.value==0 && r.accepted==0 && r.rejected==0); }
        std::cout << "PUBLIC 2/2\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}

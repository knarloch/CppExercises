#include <vector>
#include <string>
#include <stdexcept>
#include <cassert>
#include <iostream>

std::pair<int, size_t> Calculate(int x, const char *expr, size_t begin);

std::pair<int, size_t> CalculateOperand(int x, const char *expr, size_t begin) {
    switch (expr[begin]) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '-':
        {
            char* cursor = nullptr;
            auto val = std::strtoll(expr + begin, &cursor, 10);
            return {val, cursor - expr};
        }
        case 'x':
            return {x, begin + 1};
        case '(': {
            auto [val, cursor] = Calculate(x, expr, begin + 1);
            return {val, cursor + 1};
        }
        default:
            throw std::runtime_error{std::string{"Unexpected operand: "} + expr[begin]};
    }
}

std::pair<int, size_t> Calculate(int x, const char *expr, size_t begin) {
    auto [left, cursor_past_left] = CalculateOperand(x, expr, begin);
    auto [right, cursor_past_right] = CalculateOperand(x, expr, cursor_past_left + 3);

    switch (expr[cursor_past_left]) {
        case 'a':
            return {left + right, cursor_past_right};
        case 'm':
            return {left * right, cursor_past_right};
        case 's':
            return {left - right, cursor_past_right};
        default:
            throw std::runtime_error{std::string{"Unexpected operator: "} + expr[cursor_past_left]};
    }
}

std::vector<int> Calculate(int x, const std::string &expr) {
    std::vector<int> result;
    for (auto cursor = 1; cursor < expr.size() - 1;) {
        auto calculated = Calculate(x, expr.c_str(), cursor);
        result.push_back(calculated.first);
        cursor = calculated.second + sizeof(',');
    }
    return result;
}

int main() {
    try {
        {
            std::string expr = "{2mul(xadd7),(6subx)mulx,2add((xadd3)mul2)}";
            auto result = Calculate(1, expr);
            assert((std::vector<int>{16, 5, 10} == result));
        }
        {
            std::string expr = "{2mul(xadd7),(6subx)mulx,2add((xadd3)mul2)}";
            auto result = Calculate(0, expr);
            assert((std::vector<int>{14, 0, 8} == result));
        }
        {
            std::string expr = "{20mul(xadd7),(-16subx)mulx}";
            auto result = Calculate(2, expr);
            assert((std::vector<int>{180, -36} == result));
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        assert(false);
    }

    return 0;
}

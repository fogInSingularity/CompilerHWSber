#include "parser/parser.hpp"
#include "helpers/common.hpp"

#include <array>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace slr {
namespace prs {

namespace {

enum class Terminal {
    kPlus,
    kMinus,
    kMul,
    kDiv,
    kLParen,
    kRParen,
    kNum,
    kVar,
    kEnd,
    kInvalid,
};

enum class NonTerm {
    kE,
    kT,
    kF,
};

enum class ActionType {
    kShift,
    kReduce,
    kAccept,
    kError,
};

struct Action {
    ActionType type;
    int value;
};

struct Production {
    NonTerm lhs;
    int rhs_len;
};

constexpr size_t kStateCount = 17;
constexpr size_t kTermCount = 9;
constexpr size_t kNonTermCount = 3;

constexpr Action Shift(int state) {
    return Action{ActionType::kShift, state};
}

constexpr Action Reduce(int production) {
    return Action{ActionType::kReduce, production};
}

constexpr Action Accept() {
    return Action{ActionType::kAccept, 0};
}

constexpr Action Error() {
    return Action{ActionType::kError, -1};
}

constexpr std::array<Production, 10> kProductions = {{
    Production{NonTerm::kE, 1}, // 0: S' -> E
    Production{NonTerm::kE, 3}, // 1: E -> E + T
    Production{NonTerm::kE, 3}, // 2: E -> E - T
    Production{NonTerm::kE, 1}, // 3: E -> T
    Production{NonTerm::kT, 3}, // 4: T -> T * F
    Production{NonTerm::kT, 3}, // 5: T -> T / F
    Production{NonTerm::kT, 1}, // 6: T -> F
    Production{NonTerm::kF, 3}, // 7: F -> ( E )
    Production{NonTerm::kF, 1}, // 8: F -> NUM
    Production{NonTerm::kF, 1}, // 9: F -> VAR
}};

constexpr std::array<const char*, 10> kProductionStrs = {{
    "S' -> E",
    "E -> E + T",
    "E -> E - T",
    "E -> T",
    "T -> T * F",
    "T -> T / F",
    "T -> F",
    "F -> ( E )",
    "F -> NUM",
    "F -> VAR",
}};

static std::array<std::array<Action, kTermCount>, kStateCount> BuildActionTable() {
    return {{
        {Error(),   Error(),   Error(),   Error(),   Shift(4), Error(),   Shift(5), Shift(6), Error()  },
        {Shift(7),  Shift(8),  Error(),   Error(),   Error(),  Error(),   Error(),  Error(),  Accept() },
        {Reduce(3), Reduce(3), Shift(9),  Shift(10), Error(),  Reduce(3), Error(),  Error(),  Reduce(3)},
        {Reduce(6), Reduce(6), Reduce(6), Reduce(6), Error(),  Reduce(6), Error(),  Error(),  Reduce(6)},
        {Error(),   Error(),   Error(),   Error(),   Shift(4), Error(),   Shift(5), Shift(6), Error()  },
        {Reduce(8), Reduce(8), Reduce(8), Reduce(8), Error(),  Reduce(8), Error(),  Error(),  Reduce(8)},
        {Reduce(9), Reduce(9), Reduce(9), Reduce(9), Error(),  Reduce(9), Error(),  Error(),  Reduce(9)},
        {Error(),   Error(),   Error(),   Error(),   Shift(4), Error(),   Shift(5), Shift(6), Error()  },
        {Error(),   Error(),   Error(),   Error(),   Shift(4), Error(),   Shift(5), Shift(6), Error()  },
        {Error(),   Error(),   Error(),   Error(),   Shift(4), Error(),   Shift(5), Shift(6), Error()  },
        {Error(),   Error(),   Error(),   Error(),   Shift(4), Error(),   Shift(5), Shift(6), Error()  },
        {Shift(7),  Shift(8),  Error(),   Error(),   Error(),  Shift(16), Error(),  Error(),  Error()  },
        {Reduce(1), Reduce(1), Shift(9),  Shift(10), Error(),  Reduce(1), Error(),  Error(),  Reduce(1)},
        {Reduce(2), Reduce(2), Shift(9),  Shift(10), Error(),  Reduce(2), Error(),  Error(),  Reduce(2)},
        {Reduce(4), Reduce(4), Reduce(4), Reduce(4), Error(),  Reduce(4), Error(),  Error(),  Reduce(4)},
        {Reduce(5), Reduce(5), Reduce(5), Reduce(5), Error(),  Reduce(5), Error(),  Error(),  Reduce(5)},
        {Reduce(7), Reduce(7), Reduce(7), Reduce(7), Error(),  Reduce(7), Error(),  Error(),  Reduce(7)},
    }};
}

static std::array<std::array<int, kNonTermCount>, kStateCount> BuildGotoTable() {
    std::array<std::array<int, kNonTermCount>, kStateCount> table{};
    for (auto& row : table) {
        for (auto& cell : row) {
            cell = -1;
        }
    }

    auto set = [&](int state, NonTerm nt, int next_state) {
        table[state][hlp::FromEnum(nt)] = next_state;
    };

    set(0, NonTerm::kE, 1);
    set(0, NonTerm::kT, 2);
    set(0, NonTerm::kF, 3);

    set(4, NonTerm::kE, 11);
    set(4, NonTerm::kT, 2);
    set(4, NonTerm::kF, 3);

    set(7, NonTerm::kT, 12);
    set(7, NonTerm::kF, 3);

    set(8, NonTerm::kT, 13);
    set(8, NonTerm::kF, 3);

    set(9, NonTerm::kF, 14);
    set(10, NonTerm::kF, 15);

    return table;
}

Terminal ToTerminal(const lex::Token& token) {
    using lex::Operations;
    using TokenType = lex::Token::TokenType;

    switch (token.type) {
        case TokenType::kFinished:
            return Terminal::kEnd;
        case TokenType::kNum:
            return Terminal::kNum;
        case TokenType::kVar:
            return Terminal::kVar;
        case TokenType::kOp: {
            const auto op = std::get<Operations>(token.val);
            switch (op) {
                case Operations::kPlus: return Terminal::kPlus;
                case Operations::kMinus: return Terminal::kMinus;
                case Operations::kMul: return Terminal::kMul;
                case Operations::kDiv: return Terminal::kDiv;
                case Operations::kLeftBracket: return Terminal::kLParen;
                case Operations::kRightBracket: return Terminal::kRParen;
                default:
                    return Terminal::kInvalid;
            }
        }
        default:
            return Terminal::kInvalid;
    }
}

std::string StackToStr(const std::vector<int>& stack) {
    std::string out = "";
    out += "[";

    for (size_t i = 0; i < stack.size(); ++i) {
        if (i > 0) {
            out += " ";
        }
        out += std::to_string(stack[i]);
    }

    out += "]";
    return out;
}

std::string InputToStr(const lex::Token& token) {
    using TokenType = lex::Token::TokenType;
    if (token.type == TokenType::kFinished) {
        return "$";
    }

    lex::Token copy = token;
    std::string out = copy.ToStr();
    out += " ...";
    return out;
}

std::string ActionToStr(const Action& action) {
    switch (action.type) {
        case ActionType::kShift:
            return "shift " + std::to_string(action.value);
        case ActionType::kReduce: {
            if (action.value >= 0 
                && action.value < static_cast<int>(kProductionStrs.size())) 
            {
                return std::string{"reduce "} + kProductionStrs[action.value];
            }
            return "reduce <invalid>";
        }
        case ActionType::kAccept:
            return "accept";
        case ActionType::kError:
        default:
            return "error";
    }
}

void PrintStep(
    size_t step,
    const std::vector<int>& stack,
    const lex::Token& token,
    const Action& action
) {
    std::cout
        << "step " << step
        << " | stack: " << StackToStr(stack)
        << " | input: " << InputToStr(token)
        << " | action: " << ActionToStr(action)
        << "\n";
}

} // namespace

bool Parser::Accept() {
    static const auto action_table = BuildActionTable();
    static const auto goto_table = BuildGotoTable();

    std::vector<int> state_stack;
    state_stack.push_back(0);

    std::optional<lex::Token> lookahead;

    auto Peek = [&]() -> const lex::Token& {
        if (!lookahead.has_value()) {
            lookahead = lexer_.Next();
        }
        return *lookahead;
    };

    auto Consume = [&]() { lookahead.reset(); };

    size_t step = 0;

    while (true) {
        if (state_stack.empty()) { return false; }

        int state = state_stack.back();
        const auto& token = Peek();
        
        Terminal term = ToTerminal(token);
        if (term == Terminal::kInvalid) { return false; }

        Action action = action_table[state][hlp::FromEnum(term)];

        PrintStep(step, state_stack, token, action);
        step++;

        switch (action.type) {
            case ActionType::kShift: {
                state_stack.push_back(action.value);
                Consume();
                break;
            }
            case ActionType::kReduce: {
                if (action.value <= 0 
                    || action.value >= static_cast<int>(kProductions.size())) 
                {
                    return false;
                }

                const Production& prod = kProductions[action.value];
                for (int i = 0; i < prod.rhs_len; ++i) {
                    if (state_stack.empty()) { return false; }
                    state_stack.pop_back();
                }

                if (state_stack.empty()) { return false; }

                int goto_state = goto_table[state_stack.back()][hlp::FromEnum(prod.lhs)];
                if (goto_state < 0) { return false; }

                state_stack.push_back(goto_state);
                break;
            }
            case ActionType::kAccept: return true;
            case ActionType::kError:
            default:
                return false;
        }
    }
}

} // namespace prs
} // namespace slr

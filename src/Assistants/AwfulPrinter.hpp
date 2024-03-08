/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

/*------------------------------------------------------------------*/
/*  ANSI escape codes                                               */
/*------------------------------------------------------------------*/

#define _ANSI(arg1) "\x1B[" #arg1

template <std::integral P1, std::integral... PN>
std::string ANSI(const char codetype, P1&& first, PN&&... params) {
    std::ostringstream oss;
    oss << "\x1B[" << std::forward<P1>(first);
    ((oss << ";" << std::forward<PN>(params)), ...);
    oss << codetype;
    return oss.str();
}

template <std::integral P1, std::integral... PN>
std::wstring ANSI(const wchar_t codetype, P1&& first, PN&&... params) {
    std::wostringstream oss;
    oss << "\x1B[" << std::forward<P1>(first);
    ((oss << ";" << std::forward<PN>(params)), ...);
    oss << codetype;
    return oss.str();
}


inline std::string noANSI(const std::string& input_text) {
    static const std::regex pattern{ "\x1B\\[[0-9;]*[A-Za-z]"s };
    return std::regex_replace(input_text, pattern, "");
}

inline std::wstring noANSI(const std::wstring& input_text) {
    static const std::wregex pattern{ L"\x1B\\[[0-9;]*[A-Za-z]"s };
    return std::regex_replace(input_text, pattern, L"");
}

/*------------------------------------------------------------------*/
/*  textBlock                                                       */
/*------------------------------------------------------------------*/

inline void textBlock(
    std::wstringstream&& stringBuf,
    const std::size_t wrap_len = 70
) {
    const std::wstring border(wrap_len, L'─');

    std::wstring word{};
    std::size_t line_len{ 0 };

    auto upBorder{ [&border]() {
        std::wcout << _ANSI(90m) << L"╭─"sv << border << L"─╮\n"sv << _ANSI(0m);
    } };
    auto dnBorder{ [&border]() {
        std::wcout << _ANSI(90m) << L"╰─"sv << border << L"─╯\n"sv << _ANSI(0m);
    } };
    auto ltBorder{ []() {
        std::wcout << _ANSI(90m) << L"│ "sv << _ANSI(0m);
    } };
    auto rtBorder{ []() {
        std::wcout << _ANSI(90m) << L" │\n"sv << _ANSI(0m);
    } };
    auto padWidth{ [&wrap_len, &line_len]() {
        auto pad{wrap_len - line_len};
        if (pad > wrap_len) pad = 0;
        std::wcout << std::wstring(pad, L' ');
        line_len = 0;
    } };

    upBorder();
    ltBorder();

    while (std::getline(stringBuf, word, L' ')) {
        const auto stripped{ noANSI(word) };
        bool br{ stripped == L"\n"sv };

        if (br || line_len + stripped.length() > wrap_len) {
            padWidth();
            rtBorder();
            ltBorder();
            if (br) continue;
        }

        std::wcout << word;
        line_len += stripped.length();

        if (line_len < wrap_len) {
            std::wcout << L" "sv;
            line_len += 1;
        }
    }
    padWidth();
    rtBorder();
    dnBorder();
}

inline void textBlock(
    const std::wstring_view text,
    const std::size_t wrap_len = 70
) {
    textBlock(std::wstringstream{}
        << text.data(), wrap_len
    );
}

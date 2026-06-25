/**
 * @file rich.hpp
 * @brief 使用 VT 控制序列格式化输出
 * 
 * @author JoyWonderful
 * @date 2026-06-06
 */
#pragma once // 保证头文件只被编译一次
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <windows.h>
#define sleep(ms) Sleep(ms) // 原单位：毫秒
#elif defined(__unix) || defined(__unix__)
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#define sleep(ms) usleep(ms * 1000) // 现用单位：毫秒；原单位：微秒
#endif
#include <stdio.h>
#include <locale.h>
#include <iostream>
#include <initializer_list> // 多参数
#include <string>

/////////////////
// DEFINITIONS //
/////////////////
// https://www.cnblogs.com/chargedcreeper/p/-/ANSI
#define COLOR_FG_BLACK   "\033[30m" /// 前景-黑色
#define COLOR_FG_RED     "\033[31m" /// 前景-红色
#define COLOR_FG_GREEN   "\033[32m" /// 前景-绿色
#define COLOR_FG_YELLOW  "\033[33m" /// 前景-黄色
#define COLOR_FG_BLUE    "\033[34m" /// 前景-蓝色
#define COLOR_FG_MAGENTA "\033[35m" /// 前景-品红
#define COLOR_FG_CYAN    "\033[36m" /// 前景-青色
#define COLOR_FG_WHITE   "\033[37m" /// 前景-白色
#define COLOR_BG_BLACK   "\033[40m" /// 背景-黑色
#define COLOR_BG_RED     "\033[41m" /// 背景-红色
#define COLOR_BG_GREEN   "\033[42m" /// 背景-绿色
#define COLOR_BG_YELLOW  "\033[43m" /// 背景-黄色
#define COLOR_BG_BLUE    "\033[44m" /// 背景-蓝色
#define COLOR_BG_MAGENTA "\033[45m" /// 背景-品红
#define COLOR_BG_CYAN    "\033[46m" /// 背景-青色
#define COLOR_BG_WHITE   "\033[47m" /// 背景-白色

#define STYL_CLEARALL  "\033[0m" /// 清除所有样式（包括颜色）
#define STYL_BOLD      "\033[1m" /// 样式-粗体
#define STYL_DIM       "\033[2m" /// 样式-黯淡
#define STYL_ITALIC    "\033[3m" /// 样式-斜体
#define STYL_UNDERLINE "\033[4m" /// 样式-下划线
#define STYL_BLINK     "\033[5m" /// 样式-闪烁
#define STYL_NEGATIVE  "\033[7m" /// 样式-反转
#define STYL_HIDDEN    "\033[8m" /// 样式-隐藏
#define STYL_DEL       "\033[9m" /// 样式-删除线

//! 遇到换行符时输出缓冲区才会刷新，注意用 fflush 时机
//! VT 序列中光标坐标行列均从 1 开始
namespace rich {
///////////////
// FUNCTIONS //
///////////////
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    /**
     * @brief 在 Windows 下启用虚拟终端序列。
     * @return `true` 代表成功，`false` 代表失败。
     * 
     * @details
     * 输出模式设定 `ENABLE_VIRTUAL_TERMINAL_PROCESSING` `DISABLE_NEWLINE_AUTO_RETURN`；
     * 输入模式设定 `ENABLE_VIRTUAL_TERMINAL_PROCESSING`；
     * 还设置代码页为 `65001`(`CP_UTF8`)。
     * 
     * @see https://learn.microsoft.com/windows/console/console-virtual-terminal-sequences#example-of-enabling-virtual-terminal-processing
     * @see https://learn.microsoft.com/windows/console/classic-vs-vt
     */
    bool win_vt_setup() {
        SetConsoleCP(CP_UTF8); // 65001
        SetConsoleOutputCP(CP_UTF8);

        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut == INVALID_HANDLE_VALUE)
        {
            return false;
        }
        HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
        if (hIn == INVALID_HANDLE_VALUE)
        {
            return false;
        }

        DWORD dwOriginalOutMode = 0;
        DWORD dwOriginalInMode = 0;
        if (!GetConsoleMode(hOut, &dwOriginalOutMode))
        {
            return false;
        }
        if (!GetConsoleMode(hIn, &dwOriginalInMode))
        {
            return false;
        }

        DWORD dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
        DWORD dwRequestedInModes = ENABLE_VIRTUAL_TERMINAL_INPUT;

        DWORD dwOutMode = dwOriginalOutMode | dwRequestedOutModes;
        if (!SetConsoleMode(hOut, dwOutMode))
        {
            // we failed to set both modes, try to step down mode gracefully.
            dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            dwOutMode = dwOriginalOutMode | dwRequestedOutModes;
            if (!SetConsoleMode(hOut, dwOutMode))
            {
                // Failed to set any VT mode, can't do anything here.
                return false;
            }
        }

        DWORD dwInMode = dwOriginalInMode | dwRequestedInModes;
        if (!SetConsoleMode(hIn, dwInMode))
        {
            // Failed to set VT input mode, can't do anything here.
            return false;
        }

        return true;
    }
    #endif

    /**
     * @brief 生成使文字为 RGB 颜色的 VT 序列。
     * @param _color_str 颜色。格式可以为 `rgb({r},{g},{b})`（空格无所谓）；也可以为 `#FFFFFF` 的十六进制形式。
     * @param _is_bg *可选* 是否为背景色。若为 `true`，则控制背景色；否则为前景色（默认）。
     * @return 为 `std::string` 类型的 VT 序列。可以作为 `rich_print` 的参数。
     */
    std::string color_rgb(std::string _color_str, bool _is_bg = false)
    {
        int r = 0, g = 0, b = 0;
        if(_color_str[0] == '#')
        {
            _color_str.erase(0, 1);
            sscanf(_color_str.c_str(), "%2x%2x%2x", &r, &g, &b);
            //printf("if:%s\n", _color_str.c_str()); //@@test
        }
        else if(_color_str[0] == 'r')
        {
            int pos;
            while(pos = _color_str.find(' '), pos != std::string::npos) _color_str.erase(pos, 1);// 去除所有空格以格式化输入
            _color_str.erase(0, 4); _color_str.pop_back(); // 去掉 `rgb` 和括号
            sscanf(_color_str.c_str(), "%d,%d,%d", &r, &g, &b);
            // printf("else:%s\n", _color_str.c_str()); //@@test
        }
        char cret[100];
        //@@test
        // sprintf(cret, "[%d;2;%d;%d;%dm", (_is_bg?48:38), r, g, b);
        // printf("%s\n", cret);
        //@!
        sprintf(cret, "\033[%d;2;%d;%d;%dm", (_is_bg?48:38), r, g, b);
        std::string ret = cret;
        return ret; 
    }
    /**
     * @brief 生成超链接的 OSC 序列
     * @param _uri 网址；URL。
     * @param _text 显示的文字。若为空，则使用 URL。
     * @return 生成的序列。
     * 
     * @see https://terminfo.dev/extensions/osc-8-hyperlinks
     */
    std::string hyper_link(std::string _uri, std::string _text = "")
    {
        if(_text == "") _text = _uri;
        return "\033]8;;"+_uri+"\007"+_text+"\033]8;;\007";
    }

    void cursor_reset() {printf("\033[H"); fflush(stdout);}
    /**
     * @note 行、列从 1 开始。
     * @note 假如到了行首、行尾、列首、列尾，只会卡在那不动了。
     */
    void cursor_mv(int line, int col) {printf("\033[%d;%dH", line, col); fflush(stdout);}
    void cursor_up(int n) {printf("\033[%dA",n); fflush(stdout);}
    void cursor_up_with_goto_start(int n) {printf("\033[%dF",n); fflush(stdout);}
    void cursor_down(int n) {printf("\033[%dB",n);}
    void cursor_down_with_goto_start(int n) {printf("\033[%dE",n); fflush(stdout);}
    void cursor_right(int n) {printf("\033[%dC",n); fflush(stdout);}
    void cursor_left(int n) {printf("\033[%dD",n); fflush(stdout);}
    void cursor_save() {printf("\033[s"); fflush(stdout);}
    void cursor_load() {printf("\033[u"); fflush(stdout);}
    /**
     * @brief 获取光标坐标。
     * @param _row 行将被传递到此变量中。
     * @param _col 列将被传递到此变量中。
     * @return `bool`。`true` 表示成功；`false` 表示失败。
     * 
     * @note 行、列从 1 开始。  
     * @details Linux 部分 由 Deepseek 辅助完成。尽力了...
     */
    bool cursor_coord(int &_row, int &_col)
    {
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
            // 返回的坐标是从 0 开始的，转换为从 1 开始以保持与 ANSI 版本一致
            _row = csbi.dwCursorPosition.Y + 1;
            _col = csbi.dwCursorPosition.X + 1;
            return true;
        }
        return false;
    #elif defined(__unix) || defined(__unix__)
        struct termios old_tio, new_tio;
        
        // 获取当前终端属性
        if (tcgetattr(STDIN_FILENO, &old_tio) != 0) return false;
        new_tio = old_tio;
        
        // 关闭规范模式(ICANON)和回显(ECHO)
        // 规范模式会导致行缓冲，必须关闭才能立即读到无换行的数据
        new_tio.c_lflag &= ~(ICANON | ECHO);
        // 设置 VMIN 和 VTIME，VMIN=1 表示至少读 1 个字符才返回
        new_tio.c_cc[VMIN] = 1;
        new_tio.c_cc[VTIME] = 0;
        
        if (tcsetattr(STDIN_FILENO, TCSANOW, &new_tio) != 0) return false;
        
        // 发送查询光标位置命令
        std::cout << "\033[6n" << std::flush;
        
        // 读取响应，直到遇到 'R'
        std::string response;
        char ch;
        while (read(STDIN_FILENO, &ch, 1) == 1)
        {
            response += ch;
            if (ch == 'R') break;
            // 防止意外无限循环
            if (response.size() > 32) break;
        }
        
        // 恢复原始终端属性
        tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
        
        // 解析响应: 格式为 `ESC[行;列R`。从 1 开始
        if (response.size() < 4 || response[0] != '\033' || response[1] != '[') return false;
        
        size_t semicolon = response.find(';');
        size_t r_pos = response.find('R');
        if (semicolon == std::string::npos || r_pos == std::string::npos) return false;
        
        try {
            _row = std::stoi(response.substr(2, semicolon - 2));
            _col = std::stoi(response.substr(semicolon + 1, r_pos - semicolon - 1));
            return true;
        } catch (...) {
            return false;
        }
    #endif
    }

    void screen_clear()
    {
        printf("\033[0m\033[H\033[2J\033[H\033[0m"); // 将光标移回行首并清零
        fflush(stdout);
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        system("cls");
#elif defined(__unix) || defined(__unix__)
        system("clear");
#endif
    }
    /** 
     * @brief 获取控制台窗口大小。
     * @param _row 行将被传递到此变量中。
     * @param _col 列将被传递到此变量中。
     * @return 无。
    */
    void screen_size_get(int &_row, int &_col)
    {
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hOut, &csbi);
        _row = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        _col = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    #elif defined(__unix) || defined(__unix__)
        winsize wz;
        ioctl(STDOUT_FILENO,TIOCGWINSZ, &wz);
        _row = (int)wz.ws_row;
        _col = (int)wz.ws_col;
    #endif
    }

    /**
     * @brief 带样式的文本输出 & 模拟打字输出。
     * @param _str 要输出的字符串。
     * @param _styles *可选* 带有的样式。
     * @param _delay *可选* 时间间隔，单位为毫秒(ms)；建议一般输出 `120`。默认为 `120`。
     * @return 输出的宽度。
     * 
     * @note 用大括号 `{}` 包裹样式列表。如 `rich_print("Ca+H", {STYL_BOLD, COLOR_FG_RED})`。
     */
    int rich_print(std::string _str, std::initializer_list<std::string> _styles = {}, int _delay = 120)
    {
        int sty_cnt = 0;
        int out_leng = 0;
        for(auto i: _styles) // 可变参数
        {
            printf("%s", i.c_str()); // 应用所有样式
            ++sty_cnt;
        }
        if(_delay == 0) printf("%s", _str.c_str());
        for(int i = 0; i < _str.length(); i++)
        {
            if ((_str[i] & 0xE0) == 0xE0) // 3字节字符，Unicode range:0x0800~0xFFFF (UTF-8: 0b1110XXXX 0b10XXXXXX 0b10XXXXXX)（X 为任意可行数字）
            {   // 即判断是否为UTF-8中文字符的第一个字节 UTF-8 3 字节字符（中文字符，Unicode range:0x4E00~0x9FFF，包含在内）
                // 0b1110XXXX 为 0xE0XX
                if(_delay) printf("%c%c%c", _str[i], _str[i+1], _str[i+2]);
                i += 2;
                out_leng += 2;
            }
            else
            {
                if(_str[i]=='\033' && _str[i+1]==']' && _str[i+2]=='8') // 判定为超链接
                {
                    int pos = _str.find('\007', i);
                    int len = pos - i + 1;
                    std::string out = _str.substr(i, len);
                    std::cout << out << std::flush;
                    i += len - 1;
                    continue;
                }
                if(_delay) printf("%c", _str[i]);
                ++out_leng;
            }
            if(_delay) {fflush(stdout); sleep(_delay);}
        }
        
        if(sty_cnt > 0) printf(STYL_CLEARALL); // 清除所有样式
        fflush(stdout);
        return out_leng;
    }
    /**
     * @brief 带有 placeholder 的问题输入。
     * @param _quest 问题。
     * @param _styles 自定义 placeholder 样式。
     * @return `std::string`，回答。
     */
    std::string rich_input(std::string _quest,
        std::initializer_list<std::string> _styles = {rich::color_rgb("#737373"), STYL_ITALIC})
    {
        cursor_save();
        rich_print(_quest, _styles, 0); // 再 DIM 就看不到了
        cursor_load(); // 形成 Placeholder 效果
        std::string in;
        std::cin >> in;
        return in;
    }
    /**
     * @brief 逐字擦除。
     * @param _num *可选* 擦除的列数。默认为 `0`。若为 `0`，则擦除一整行。
     * @param _delay *可选* 延迟时间。默认为 `10`。
     * @return 无。
     * 
     * @attention 中文字按两列算！
     * @attention 若擦到开头，则会从上一行末尾开始。
     * @attention 擦除完毕，光标正好处于最后一个擦除的位置。例如 `abc|` => erase(1, 0); => `ab|`。
     * @note 你可能需要 `rich_print` 获取显示长度。
     */
    void rich_erase(int _num = 0, int _delay = 10)
    {
        if(_num == 0)
        {
            int row, col;
            cursor_coord(row, col);
            _num = col - 1; // 由于光标处在一行末尾，则它的位置为需要擦除的列 -1
        }
        for(int i = 1; i <= _num; i++)
        {
            int row, col, srow, scol;
            cursor_coord(row, col);
            rich::cursor_left(1);
            printf("\033[0K");
            fflush(stdout);
            sleep(_delay);
            if(col == 1 && i != _num)
            {
                screen_size_get(srow, scol);
                cursor_mv(row-1, scol);
                printf("\033[0K");
                fflush(stdout);
            }
        }
    }
    /**
     * @brief 展示“选择后修改”的效果。
     * @param _orig_str 原本输出的字符串。
     * @param _new_str 修改后的字符串。
     * @param _orig_styles *可选* 应用于原本输出的字符串的样式。
     * @param _new_styles *可选* 应用于修改后的字符串的样式。
     * @param _orig_delay *可选* 用于原本输出字符串的 `rich_print()` 的 `_delay` 参数，默认为 `120`。
     * @param _new_delay *可选* 用于修改后字符串的 `rich_print()` 的 `_delay` 参数，默认为 `0`。
     * @param _wait_delay *可选* “原本输出”与“选择”效果之间、“选择”效果与“修改替换”之间的等待时间，默认为 `128`。
     * @return 无。
     * 
     * @see `rich_print()`
     */
    void rich_modify(std::string _orig_str, std::string _new_str,
        std::initializer_list<std::string> _orig_styles = {},
        std::initializer_list<std::string> _new_styles = {},
        int _orig_delay = 120, int _new_delay = 0, int _wait_delay = 150)
    {
        cursor_save();
        int leng = rich_print(_orig_str, _orig_styles, _orig_delay); sleep(_wait_delay); // 先正常输出要求的字符串
        cursor_load();
        rich_print(_orig_str, {STYL_NEGATIVE}, 0); sleep(_wait_delay); // 把它高亮（模拟选择）出来并等待
        rich_erase(leng, 0); // 避免修改后的输出比原字符串来得短
        rich_print(_new_str, _new_styles, _new_delay); // 替换
    }

    /**
     * @brief 将标题设置为 `_str`。
     * @param _str 要设置的标题。
     * @return 无。
     */
    void title_set(std::string _str)
    {
        std::cout << "\033]0;" << _str << "\007" << std::flush;
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        std::string tit = "title " + _str;
        system(tit.c_str());
    #endif
    }

    /**
     * @brief 测试
     * @return 无
     */
    void test()
    {
        const std::string STYLE_LST_VALUE[] = {
            COLOR_FG_BLACK, COLOR_FG_RED, COLOR_FG_GREEN, COLOR_FG_YELLOW, COLOR_FG_BLUE, COLOR_FG_MAGENTA, COLOR_FG_CYAN, COLOR_FG_WHITE,
            COLOR_BG_BLACK, COLOR_BG_RED, COLOR_BG_GREEN, COLOR_BG_YELLOW, COLOR_BG_BLUE, COLOR_BG_MAGENTA, COLOR_BG_CYAN, COLOR_BG_WHITE,
            STYL_BOLD, STYL_DIM, STYL_ITALIC, STYL_UNDERLINE, STYL_BLINK, STYL_NEGATIVE, STYL_HIDDEN, STYL_DEL
        };
        const std::string STYLE_LST_NAME[] = {
            "FG_BLACK", "FG_RED", "FG_GREEN", "FG_YELLOW", "FG_BLUE", "FG_MAGENTA", "FG_CYAN", "FG_WHITE",
            "BG_BLACK", "BG_RED", "BG_GREEN", "BG_YELLOW", "BG_BLUE", "BG_MAGENTA", "BG_CYAN", "BG_WHITE",
            "BOLD", "DIM", "ITALIC", "UNDERLINE", "BLINK", "NEGATIVE", "HIDDEN", "DEL"
        };
        for(int i = 1; i <= 24; i++)
        {
            char name[100];
            sprintf(name, "%13s", STYLE_LST_NAME[i-1].c_str());
            rich_print(name, {STYLE_LST_VALUE[i-1]}, 0);
            if(i % 3 == 0) printf("\n");
        }
        rich_print("rgb(233, 50, 69)", {color_rgb("rgb(233, 50, 69)", true)}, 0);
        rich_print("rgb(126, 228, 37)", {color_rgb("rgb(126,228,37)", true)}, 0);
        rich_print("#bb00ff", {color_rgb("#bb00ff", true), STYL_BOLD, COLOR_FG_BLACK}, 0);
        printf("\nWaiting for 1000ms...\n");
        sleep(1000);
        printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"); // 236 个 a
        rich_erase(230, 0);
        printf("b");
        printf("\nWaiting for 5000ms...\n");
        sleep(5000);
        printf("test done.\n");
    }
}
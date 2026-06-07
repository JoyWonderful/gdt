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
#define sleep(ms) usleep(ms * 1000) // 现用单位：毫秒；原单位：微秒
#endif
#include <stdio.h>
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
     * @param _is_bg 是否为背景色。若为 `true`，则控制背景色；否则为前景色。
     * @return 为 `std::string` 类型的 VT 序列。可以作为 `rich_print` 的参数。
     */
    std::string color_rgb(std::string _color_str, bool _is_bg)
    {
        unsigned int r = 0, g = 0, b = 0;
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

    void cursor_reset() {printf("\033[H");}
    /**
     * @note 行、列从 0 开始
     */
    void cursor_mv(unsigned int line, unsigned int col) {printf("\033[%d;%dH", line, col);}
    void cursor_up(unsigned int n) {printf("\033[%dA",n);}
    void cursor_up_with_goto_start(unsigned int n) {printf("\033[%dF",n);}
    void cursor_down(unsigned int n) {printf("\033[%dB",n);}
    void cursor_down_with_goto_start(unsigned int n) {printf("\033[%dE",n);}
    void cursor_right(unsigned int n) {printf("\033[%dC",n);}
    void cursor_left(unsigned int n) {printf("\033[%dD",n);}
    void cursor_save() {printf("\033[s");}
    void cursor_load() {printf("\033[u");}

    void screen_clear()
    {
        printf("\e[H\e[2J\e[H"); // 将光标移回行首并清零
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        system("cls");
#elif defined(__unix) || defined(__unix__)
        system("clear");
#endif
    }

    /**
     * @brief 带样式的文本输出。
     * @param _str 要输出的字符串。
     * @param _styles 带有的样式。
     * @return 无。
     * 
     * @note 用大括号 `{}` 包裹样式列表。如 `rich_print("Ca+H", {STYL_BOLD, COLOR_FG_RED})`。
     */
    void rich_print(std::string _str, std::initializer_list<std::string> _styles)
    {
        for(auto i: _styles) // 可变参数
        {
            printf("%s", i.c_str()); // 应用所有样式
        }
        printf("%s", _str.c_str());
        printf(STYL_CLEARALL); // 清除所有样式
    }
    /**
     * @brief 模拟打字输出。
     * @param _str 要输出的字符串。
     * @param _delay 时间间隔，单位为毫秒(ms)；默认 `120`。
     */
    void typing(std::string _str, unsigned int _delay = 120U)
    {
        for(int i = 0; i < _str.length(); i++)
        {
            printf("%c", _str[i]);
            fflush(stdout);
            sleep(_delay);
        }
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
            rich_print(name, {STYLE_LST_VALUE[i-1]});
            if(i % 3 == 0) printf("\n");
        }
        rich_print("rgb(233, 50, 69)", {color_rgb("rgb(233, 50, 69)", true)});
        rich_print("rgb(126, 228, 37)", {color_rgb("rgb(126,228,37)", true)});
        rich_print("#bb00ff", {color_rgb("#bb00ff", true), STYL_BOLD, COLOR_FG_BLACK});
        printf("\nWaiting for 1000ms...\n");
        sleep(1000);
        printf("test done.\n");
    }
}
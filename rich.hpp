/**
 * @file rich.hpp
 * @brief 使用 VT 控制序列格式化输出
 * 
 * @author JoyWonderful
 * @date 2026-06-06
 */
#define __WINDOWS__ defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#if __WINDOWS__
#include <windows.h>
#endif
#include <stdio.h>
#include <initializer_list>
#include <string>

/////////////////
// DEFINITIONS //
/////////////////
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
    #if __WINDOWS__
    /**
     * @brief 在 Windows 下启用虚拟终端序列
     * @return `true` 代表成功，`false` 代表失败
     * 
     * @details
     * 输出模式设定 `ENABLE_VIRTUAL_TERMINAL_PROCESSING` `DISABLE_NEWLINE_AUTO_RETURN`；
     * 输入模式设定 `ENABLE_VIRTUAL_TERMINAL_PROCESSING`；
     * 还设置代码页为 `65001`(`CP_UTF8`)。
     * 
     * @see https://learn.microsoft.com/zh-cn/windows/console/console-virtual-terminal-sequences#example-of-enabling-virtual-terminal-processing
     */
    bool win_vt_setup() {
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

        SetConsoleCP(CP_UTF8); // 65001
        SetConsoleOutputCP(CP_UTF8);

        return true;
    }
    #endif

    /**
     * @brief 带样式的文本输出
     * @param _str 要输出的字符串
     * @param _styles 带有的样式
     * @return 无
     * 
     * @note 用大括号 `{}` 包裹样式
     */
    void richPrint(std::string _str, std::initializer_list<std::string> _styles)
    {
        for(auto i: _styles) // 可变参数
        {
            printf("%s", i.c_str()); // 应用所有样式
        }
        printf("%s", _str.c_str());
        printf(STYL_CLEARALL); // 清除所有样式
    }
    /**
     * @brief 测试样式
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
            richPrint(name, {STYLE_LST_VALUE[i-1]});
            if(i % 3 == 0) printf("\n");
        }
        printf("\n");
    }
}
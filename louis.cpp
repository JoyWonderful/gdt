/**
 * @file louis.cpp
 * @attention
 * This file MUST BE OPENED (ALSO SAVED) IN `UTF-8`, or it won't work/show correctly. \n
 * In windows, run `chcp 65001` first before compiling. \n
 * Add `-fexec-charset=UTF-8` argrument when compiling it. \n
 * Add `-static-libgcc`, `-static-libstdc++` to avoid library missing.
 * 
 * @author JoyWonderful
 * @date 2026-06-06
 */
#include "rich.hpp"
#include <locale.h>

// 方便用的宏
#define p(_str) rich::rich_print(_str, {}, T)

//! 输出时列数控制在 99 以内，行数控制在 19 以内。
int main() {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#pragma message("万恶的 Windows，疯狂的 Microsoft")
    bool setup_stat = rich::win_vt_setup();
    if(!setup_stat) {
        printf("Warning: VT mode set failed. If you're running this in conhost, styles will be disabled.\n");
    }

    int __row, __col;
    rich::screen_size_get(__row, __col);
    if(__col < 99) system("mode con cols=99");
    if(__row < 19) system("mode con lines=19");
    else
    {
        char __cmd[100];
        sprintf(&__cmd[0], "mode con lines=%d", __row);
        system(__cmd);
    }

    // Deepseek 辅助开始
    /*
    // 1. 获取控制台输出句柄
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) {
        return 1;
    }

    // 2. 配置字体信息结构体
    CONSOLE_FONT_INFOEX fontInfo = { 0 };
    fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);

    // 然后设置你关心的字段
    fontInfo.dwFontSize.Y = 24;       // 设置字体高度
    fontInfo.FontWeight = FW_NORMAL;  // 使用常量 400，更清晰
    wcscpy_s(fontInfo.FaceName, L"Cascadia Mono");

    // nFont 和 dwFontSize.X 等其他字段可以保留为0，这通常是安全的

    // 3. 应用新字体
    if (!SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo)) {
        // 如果失败，可以调用 GetLastError() 获取错误码
        return 1;
    }*/
    // Deepseek 辅助结束
#endif
    // rich::test();
    printf("\033[?12h"); // 启用光标闪烁
    setlocale(LC_ALL, "zh_CN.UTF-8"); // 正确处理中文

    /////////////////////////
    // 0TH STAGE: Question //
    /////////////////////////
    rich::screen_clear();
    rich::title_set("QUESTION");
    rich::rich_print("********SAFETY QUESTIONS********\n", {COLOR_BG_WHITE, COLOR_FG_BLACK, STYL_BOLD});
    while(true)
    {
        std::string name = rich::rich_input("Who are you?(in English, without space)");
        if(name.compare("Louis")==0 || name.compare("louis")==0 || name.compare("ShiQianyu")==0 || name.compare("shiqianyu")==0) // 论如何科学地建一座屎山
        {
            break;
        }
        else if(name.compare("Alan")==0 || name.compare("alan")==0 || name.compare("WangZeen")==0 || name.compare("WangZeen")==0) // 论如何科学地再建一座
        {// FAKE GREETING
            rich::screen_clear();
            rich::cursor_down(2);
            rich::rich_print(R"(
 ''';'''                      ,            ;      
    ;    ,'''',  ;,''',  ,''','  ,'''',  ,,;,, ;; 
    ;     ,,,,;  ;    '  ;   ;   ;,,,,;    ;      
    ;    ;    ;  ;        ;''    ;         ;   ,, 
    ;    ',,,';  ;       ,'''',  ',,,,'    ',, '' 
                         ',,,,'                   )", {}, 1);
            std::string ascii1[5] = {
                " ▄▄▄▄▄▄▄▄▄▄▄▄▄    ▄  ▄▄▄▄▄▄▄▄     ▄▄▄▄▄▄▄▄▄▄▄   ",
                "       █           █  ▀▄   ▄▀     █    █    █   ",
                "       █        ▀▄      ▀▄▀       █ ▀▀▀█▀▀▀ █   ",
                "  ▄▄▄▄▄█▄▄▄▄▄    ▀ ▄ ▄▄▀▀▄▀▀▄▄▄   █  ▄▀ ▀▄  █   ",
                "       █          ▄▀"
            };
            rich::cursor_up(7); rich::cursor_save();
            for(int i = 0; i < 5; i++)
            {
                rich::rich_print(ascii1[i], {}, 1+i*20); // 越来越慢
                if(i == 4) continue;
                rich::cursor_load();
                rich::cursor_down(1);
                rich::cursor_save();
            }
            sleep(6000); // 让它先在“恩”那儿闪一会儿
            rich::cursor_down_with_goto_start(4);
            rich::rich_print("......", {}, 2000);
            printf("%s", COLOR_FG_RED);
            int row, col;
            rich::screen_size_get(row, col);
            rich::screen_clear();
            for(int i = 0; i < row * col - 1; i++)
            {
                printf("?");
                fflush(stdout);
            }
            printf("%s", STYL_CLEARALL);
            sleep(3000);
            
            rich::cursor_reset();
            rich::cursor_mv(3, 10);
            rich::rich_print("You're cheating, right?", {}, 200);
            rich::cursor_mv(3, 10);
            rich::rich_print("You're cheating, right?", {COLOR_BG_RED, STYL_UNDERLINE});
            rich::cursor_mv(4, 10);
            rich::rich_print("The cheater will be ", {COLOR_BG_RED}, 600);
            sleep((unsigned int)9982.44353);
            rich::screen_clear();
            rich::rich_print("FATAL: ", {COLOR_FG_WHITE, COLOR_BG_RED, STYL_BOLD});
            printf("Could not read the data, because %s????????????%s.\n", COLOR_FG_BLUE, STYL_CLEARALL);
            printf("Program will be closed in 5s...\n");
            sleep(5000);
            rich::screen_clear();
            return 0; // 让作弊者重开吧
        }
        else rich::rich_print("Invalid.\n", {COLOR_FG_MAGENTA});
    }
    rich::title_set("i've got better question for ya.");
    while(true)
    {
        std::string name = rich::rich_input("What's your LUOGU username?");
        if(name.compare("TLE_1S")==0)
        {
            break;
        }
        else rich::rich_print("Invalid.\n", {COLOR_FG_MAGENTA});
    }
    rich::rich_print("Correct.\n", {COLOR_FG_GREEN});
    sleep(1024);

    /////////////////////////
    // 1ST STAGE: Greeting //
    /////////////////////////
    rich::title_set("greetings.");
    rich::screen_clear();
    rich::cursor_down(2);
    rich::rich_print(R"(
 ''';'''                      ,            ;      
    ;    ,'''',  ;,''',  ,''','  ,'''',  ,,;,, ;; 
    ;     ,,,,;  ;    '  ;   ;   ;,,,,;    ;      
    ;    ;    ;  ;        ;''    ;         ;   ,, 
    ;    ',,,';  ;       ,'''',  ',,,,'    ',, '' 
                         ',,,,'                   )", {}, 3);
    
    std::string ascii1[8] = {
        "   ▀▄    █        ▄    ▀▄   ▄▀   ▄▄▄▄▄   █▄▄▄▄   ",
        "    ▀   ▄▀▀█▀▀▀▀   █  ▄▄▄▄▄▄▄▄▄▄  ▄ ▄▀ ▄█▄▄▄▄█▄▄ ",
        " ▀▀█▀▀▀▀▄  █          ▄▄▄█▄█▄▄▄    ▀▄ ▀ █  ▄▀  █ ",
        "   █▄▄▄ █ ▄█▄▀█  ▀▀█ ▄▄▄▄█▄█▄▄█▄ ▀▀█▀▀█ ▀▀█▀▀▀▀▀ ",
        "   █  █▀█▀ █  █    █  ▄▄▄█▄█▄▄█    █ ▀ ▄▄▀▄▀▄ ▄▀ ",
        "   █  █ █  █▀▄▀▄   █ ▄  ▄█ █▄      █   ▄▄▀ ▄█▀▄  ",
        "  ▄▀  █ █  ▀   █   █▀▄▄▀ █ █ ▀▄▄   █     ▄▀ █ ▀▄ ",
        " ▄▀ ▀▄▀  ▀▀▀▀▀▀▀         █ █     ▀▄▀   ▀▀ ▀▄▀    "
    };
    rich::cursor_up(7); rich::cursor_save();
    for(int i = 0; i < 8; i++)
    {
        rich::rich_print(ascii1[i], {}, 3);
        if(i == 7) continue;
        rich::cursor_load();
        rich::cursor_down(1);
        rich::cursor_save();
    }
    sleep(1536);

    rich::rich_print(R"(
           ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⠀⠀⠀⠀⠀⠀⢀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
           ⠀⠈⢹⠉⢀⠤⠤⡀⢠⠀⠀⡄⢸⠀⠀⡇⢀⠤⠤⡀⢠⡠⠤⡀⢀⠤⢄⡇⢀⠤⠤⡀⢠⡠⠤⡀⢀⣸⣀⠀⢠⠀⠀⡄⠀⠀⡇⠀⠀⠀⠀⠀⢸⠉⠉⡆⢠⡠⠤⡀⢀⠤⠤⡀⢀⠤⠤⡀⢀⠤⠤⡀⢠⡠⠤⡀⢀⣸⣀⠀⢀⠤⠤⡀⠀⠀⠀⠀
           ⢀⠀⢸⠀⢸⠀⠀⡇⠸⡀⢀⡇⢸⡜⢣⡇⢸⠀⠀⡇⢸⠀⠀⡇⢸⠀⠀⡇⢸⠒⠒⠃⢸⠀⠀⠁⠀⢸⠀⠀⢸⠀⠀⡇⠀⠀⡇⠀⠀⠀⠀⠀⢸⠉⠉⠀⢸⠀⠀⠁⢸⠒⠒⠃⠈⠒⠤⡀⢸⠒⠒⠃⢸⠀⠀⡇⠀⢸⠀⠀⠈⠒⠤⡀⠀⠀⠀⠀
           ⠈⠒⠊⠀⠈⠒⠒⠁⠀⣈⣁⠇⠘⠀⠀⠃⠈⠒⠒⠁⠘⠀⠀⠃⠈⠒⠊⠃⠈⠒⠒⠁⠘⠀⠀⠀⠀⠘⠀⠀⠈⠒⠊⠃⠀⠒⠓⠂⠀⠀⠀⠀⠘⠀⠀⠀⠘⠀⠀⠀⠈⠒⠒⠁⠈⠒⠒⠁⠈⠒⠒⠁⠘⠀⠀⠃⠀⠈⠒⠀⠈⠒⠒⠁⠀⠘⠃⠀)", {}, 5);
    sleep(5000);
    // rich::test();
    rich::screen_clear();
    

    ///////////////////////////////////////////////
    // 2ND STAGE: The Long March of Programming? //
    ///////////////////////////////////////////////
    const unsigned int T = 120, // 说话时间
    TC = 10, // 擦除时间
    TS = 256; // 一句话的间隔时间
    rich::title_set("INTRO");
    rich::rich_print("因为不知道怎么开场，所以就这么开场了:)", {STYL_DEL, STYL_DIM}, T); sleep(TS);
    rich::rich_erase(0, TC);
    p("这就是用 `"); rich::rich_print("printf", {rich::color_rgb("#dcdcaa", false)}, T); p("()` 堆成的一个动画而已。\n"); sleep(TS);
    p("这里开始介绍我接触编程的经过（？\n"); sleep(TS);
    p("至于为何要送给"); rich::rich_print("你", {STYL_BOLD, STYL_UNDERLINE}); sleep(1500); rich::cursor_right(2); p("大概因为你也是一名"); rich::rich_print("曾经的？", {STYL_DIM}, T); rich::rich_erase(8, TC); p(" OIer 吧。"); sleep(TS);
    rich::screen_clear();

    printf("\n");
    rich::screen_clear();
    return 0;
}
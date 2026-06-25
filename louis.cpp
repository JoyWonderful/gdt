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
#include "codehi.hpp"
#include <locale.h>

// 方便用的宏
/**
 * @brief 带样式的文本输出 & 模拟打字输出。
 * @param _str 要输出的字符串。
 * @param _styles *可选* 带有的样式。
 * @param _delay *可选* 时间间隔，单位为毫秒(ms)；建议一般输出 `120`。默认为 `120`。
 * @return 输出的宽度。
 * 
 * @note 用大括号 `{}` 包裹样式列表。如 `rich_print("Ca+H", {STYL_BOLD, COLOR_FG_RED})`。
 */
#define p rich::rich_print
#define waitfory rich::rich_input("输入 Y 并按 Enter 继续。", {rich::color_rgb("#737373"), STYL_ITALIC, STYL_BLINK})

const unsigned int T = 120; // 说话每个字的间隔时间
const unsigned int TC = 10; // 擦除时间，Clear
const unsigned int TS = 256; // 每句话之间的间隔时间，Sentence
const unsigned int TST = 900; // 场景间隔时间，STage。你可能更想把它 *2 用

int r1 = 0,c1 = 0;

//! 输出时列数控制在 99 以内，行数控制在 19 以内。
int main()
{
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
    rich::rich_print("********SAFETY QUESTIONS********\n", {COLOR_BG_WHITE, COLOR_FG_BLACK, STYL_BOLD}, 0);
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
            rich::rich_print("You're cheating, right?", {COLOR_BG_RED, STYL_UNDERLINE}, 0);
            rich::cursor_mv(4, 10);
            rich::rich_print("The cheater will be ", {COLOR_BG_RED}, 600);
            sleep((unsigned int)9982.44353);
            rich::screen_clear();
            rich::rich_print("FATAL: ", {COLOR_FG_WHITE, COLOR_BG_RED, STYL_BOLD}, 0);
            printf("Could not read the data, because %s????????????%s.\n", COLOR_FG_BLUE, STYL_CLEARALL);
            printf("Program will be closed in 5s...\n");
            sleep(5000);
            rich::screen_clear();
            return 0; // 让作弊者重开吧
        }
        else rich::rich_print("Invalid.\n", {COLOR_FG_MAGENTA}, 0);
    }
    rich::title_set("i've got better question for ya.");
    while(true)
    {
        std::string name = rich::rich_input("What's your LUOGU username?");
        if(name.compare("TLE_1S")==0)
        {
            break;
        }
        else rich::rich_print("Invalid.\n", {COLOR_FG_MAGENTA}, 0);
    }
    rich::rich_print("Correct.\n", {COLOR_FG_GREEN}, 0);
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
    rich::title_set("INTRO");
    p("这段本来应该有个前情提要的，但是被删掉了。", {STYL_DEL, STYL_DIM}); sleep(TS);
    rich::rich_erase(0, TC);
    p("这就是用 `"); p("printf", {rich::color_rgb(CODE_FUNC)}); p("()` 堆成的一个动画而已。\n"); sleep(TS);
    p("这里开始介绍我接触编程的经过（？\n"); sleep(TS);
    p("至于为何要把这段送给"); p("你", {STYL_BOLD, STYL_UNDERLINE}, 0); sleep(1500); rich::cursor_right(2); p("大概因为你也是一名"); p("曾经的？", {STYL_DIM}); rich::rich_erase(8, TC); p(" OIer 吧。\n"); sleep(TS*2);
    sleep(TST);
    waitfory;
    rich::screen_clear();

    rich::title_set("Once Upon a Time");
    p("最早接触编程就是父母不知为何买了个"); p("优学派", {COLOR_FG_YELLOW, STYL_ITALIC}); p("Umix1", {COLOR_FG_YELLOW, STYL_ITALIC, STYL_DIM}); p("。"); sleep(TS);
    p("里面自带一个“"); p("编程猫 PAD", {rich::color_rgb("#ec443d"), rich::color_rgb("#fec433", true)}); p("”。两个姐姐告诉我，那就是 Scratch 的中文名。"); c1 = p("（其实不是，'编程猫'是中国深圳点猫科技有限公司的，和 Scratch 没有任何关系。他们算是搭了一个本土化的社区和有关初学者编程的一切应用）", {STYL_DIM}, T/3); sleep(TS*2); rich::rich_erase(c1); p("由于那时还没上过信息课，我很好奇。\n"); sleep(TS);
    p("那里面有课程，教我们（因为三个人都挤在平板前看...）进行简单的图形化编程。\n"); sleep(TS);
    p("那个应用大概是"); p("优学派特供版", {COLOR_FG_YELLOW, STYL_ITALIC}); p("，"); p("表扬一下", {STYL_BOLD, STYL_UNDERLINE, COLOR_FG_RED}); sleep(TS*2); p("：有限的素材、超级难用的画板和"); p("超级慢的加载项目速度。\n", {rich::color_rgb("#b67300")}, 500); sleep(TS);
    p("即便如此，我们仍然津津有味，照着课程把里面所有的项目做了个遍。\n"); sleep(TS);
    p("你可以去感受一下"); p(rich::hyper_link("https://kitten.codemao.cn/?course_id=5", "这个课程"), {COLOR_FG_BLUE}); p("。这是当时我(们)做过的最难的作品了，我们仨特别喜欢玩它。虽然说现在看来它"); p("非常简单", {COLOR_FG_CYAN}); p("吧。\n"); sleep(TS);
    p("现在我才知道，我们当时做的就是"); p(rich::hyper_link("https://shequ.codemao.cn/course", "Kitten 公开课"), {COLOR_FG_BLUE}); p("里的所有东西。\n");
    sleep(TST);
    rich::title_set("You WANT to have a try.");
    p("按住 Ctrl 并单击即可访问链接。\n这个网页版的虽然和当时的体验完全不同，但你应该去看看那个课程和课程列表\n", {STYL_DIM, STYL_ITALIC}, 0);
    waitfory;
    rich::cursor_up_with_goto_start(3);
    printf("\033[0J"); fflush(stdout); // 从光标擦除到屏尾
    rich::title_set("Once Upon a Time");
    p("不知道你有没有把那个项目完成？作何感想？\n"); sleep(TS);
    p("然后，因为这个东西没什么新的了。我们就把它"); rich::rich_modify("扔在那儿", "放在那儿", {COLOR_FG_RED}, {}); p("不管不问了。\n");
    sleep(TST*2);
    p("我们是时候进入下一个阶段了。\n", {STYL_ITALIC, COLOR_FG_GREEN, STYL_BOLD}, 0);
    sleep(TST);
    waitfory;
    rich::screen_clear();

    rich::title_set("Here comes the REAL programming.");
    p("大约在上三年级的时候，由于"); p("童心智造", {rich::color_rgb("#f65a14")}); p("的大幅宣传，我去"); p("体验", {STYL_BOLD, STYL_UNDERLINE}); p("了一下它们的体验课程。\n"); sleep(TS);
    c1 = p("（其实就是教我们 Scratch。）", {STYL_DIM}); sleep(TS*2); rich::rich_erase(c1); p("具体教什么忘了，总之就记得：开空调很冷；得益于编程猫我回答了“如果...那么”的问题，让老师很震惊；以及"); p("**班里面很多同学都来体验**", {STYL_BOLD, STYL_UNDERLINE}); printf("。\n"); sleep(TS); // HINT: 伏笔
    p("加之编程猫的体验在先，我觉得很有意思，于是就去真正地报名了。\n"); sleep(TS);
    p("具体学的什么？"); rich::rich_modify("由于记忆久远，再加上其他编程语言的学习，我全忘光了", "好像也没学什么\n", {COLOR_BG_RED}); sleep(TS);
    p("如果你不怕被我 "); p("Bug 百出的、抽象无比的", {COLOR_FG_MAGENTA}); p(" Scratch 项目震撼一整年:)，"); p("你可以去看看"); p(rich::hyper_link("https://xstxzz.scratch3.fun/users/userindex/5efd72384649dc1180d1cb59/", "我的 '象山童心智造 - 图形化编程社区' 的个人主页"), {COLOR_FG_BLUE}); p("。"); p("这算是我为我的 Scratcher 生涯留下的一座墓碑了。\n", {STYL_DEL});
    sleep(TST);
    p("按住 Ctrl 并单击即可访问链接。\n", {STYL_ITALIC, STYL_DIM}, 0);
    waitfory;
    rich::cursor_up_with_goto_start(2);
    printf("\033[0J"); fflush(stdout);
    p("很遗憾的是，这个账号我现在登录不上。\n"); sleep(TS);
    p("现在想想，Scratch 的学习对我就是提起对下一门语言的兴趣。\n"); sleep(TS);
    p("我好像"); rich::rich_modify("没学会什么有用的东西。", "只学到了变量、列表(list?)||数组(array?)、函数的概念。\n");
    sleep(TST*2);
    p("Next chapter?\n", {STYL_ITALIC, COLOR_FG_GREEN, STYL_BOLD}, 0);
    sleep(TST);
    waitfory;
    rich::screen_clear();

    rich::title_set("");

    printf("\n");
    rich::screen_clear();
    return 0;
}
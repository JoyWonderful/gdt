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
#define c(_content, _hex_rgb) rich::rich_print(_content, {rich::color_rgb(_hex_rgb)}, 32) // code

// TODO: REMOVE TC & T
const unsigned int T = 120; // 说话每个字的间隔时间
const unsigned int TC = 10; // 擦除时间，Clear
const unsigned int TS = 256; // 每句话之间的间隔时间，Sentence
const unsigned int TST = 900; // 场景间隔时间，STage。你可能更想把它 *2 用

int c1 = 0;

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
    //# 2.1
    rich::title_set("2.1 INTRO");
    p("这段本来应该有个前情提要的，但是被删掉了。", {STYL_DEL, STYL_DIM}); sleep(TS);
    rich::rich_erase(0, TC);
    p("这就是用 `"); p("printf", {rich::color_rgb(CODE_FUNC)}); p("()` 堆成的一个动画而已。\n"); sleep(TS);
    p("这里开始介绍我接触编程的经过（？\n"); sleep(TS);
    p("至于为何要把这段送给"); p("你", {STYL_BOLD, STYL_UNDERLINE}, 0); sleep(1500); rich::cursor_right(2); p("大概因为你也是一名"); p("曾经的？", {STYL_DIM}); rich::rich_erase(8, TC); p(" OIer 吧。\n"); sleep(TS*2);
    sleep(TST);
    waitfory;
    rich::screen_clear();

    //# 2.2
    rich::title_set("2.2 Once Upon a Time");
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

    //# 2.3
    rich::title_set("2.3 Here comes the REAL programming.");
    p("大约在上三年级的时候，由于"); p("童心智造", {rich::color_rgb("#f65a14")}); p("的大幅宣传，我去"); p("体验", {STYL_BOLD, STYL_UNDERLINE}); p("了一下它们的体验课程。\n"); sleep(TS);
    c1 = p("（其实就是教我们 Scratch。）", {STYL_DIM}); sleep(TS*2); rich::rich_erase(c1); p("具体教什么忘了，总之就记得：开空调很冷；得益于编程猫我回答了“如果...那么”的问题，让老师很震惊；以及"); p("**班里面很多同学都来体验**", {STYL_BOLD, STYL_UNDERLINE}); printf("。\n"); sleep(TS);
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

    //# 2.4
    rich::title_set("2.4 The chaos of batch & vbscript");
    p("四到五年级的时候，我大姐的好闺蜜不知从哪儿拿来一本"); p(rich::hyper_link("https://baike.baidu.com/item/%E9%BB%91%E5%AE%A2%E6%94%BB%E9%98%B2%E4%BB%8E%E5%85%A5%E9%97%A8%E5%88%B0%E7%B2%BE%E9%80%9A%EF%BC%9A%E5%85%A8%E6%96%B0%E7%B2%BE%E5%8D%8E%E7%89%88/61172689","《黑客攻防 从入门到精通》"), {rich::color_rgb("#ffcc00"), STYL_ITALIC}); c1 = p("北京希望电子出版社", {STYL_DIM,STYL_ITALIC}); sleep(TS*2); rich::rich_erase(c1); p("的书。"); p("反正就是把它给了大姐，然后大姐有把它给了我看。\n"); sleep(TS);
    p("重要的是，我"); rich::rich_modify("学", "接触"); p("到了 Batch & VBScript。编写了我**人生中**第一个真正意义上的 "); p("script", {STYL_BOLD, STYL_UNDERLINE}); p("。\n"); sleep(TS);
    p("下面我给你看看人生中第一个程序/(脚本?)长啥样吧。"); c1=p("这还是现在的我的优化版本", {STYL_DEL, STYL_DIM}, 30); sleep(TS*2); rich::rich_erase(c1);
    printf("\n");
    sleep(TST);
    p("按住 Ctrl 并单击即可访问链接。\n", {STYL_ITALIC, STYL_DIM}, 0);
    waitfory;
    rich::screen_clear();
    p("test.vbs\n```vbs\n", {STYL_DIM});
    sleep(TST);
    //BEGIN code
    rich::screen_clear();
    c("on error resume next\n",CODE_KEYWD);
    c("dim ",CODE_KEYWD); c("WSHshell\n",CODE_VAR);
    c("set ",CODE_KEYWD); c("WSHshell",CODE_VAR); c(" = ",CODE_DFT); c("createObject",CODE_FUNC); c("(",CODE_BRKT1); c("\"wscript.shell\"",CODE_STR); c(")\n",CODE_BRKT1);
    c("WSHshell",CODE_VAR); c(".",CODE_DFT); c("run",CODE_FUNC); c("(",CODE_BRKT1); c("\"cmd.exe /c shutdown /s /t 60 /c 快说“我很傻”，不然一分钟内关你机，不信，你试试...\"",CODE_STR); c(")\n",CODE_BRKT1);
    c("dim ",CODE_KEYWD); c("a\n",CODE_VAR);
    c("do while",CODE_FLOWCTRL); c("(",CODE_BRKT1); c("a",CODE_VAR); c(" <> ",CODE_DFT); c("\"我很傻\"",CODE_STR); c(")\n",CODE_BRKT1);
    c("a",CODE_VAR); c(" = ",CODE_DFT); c("inputBox",CODE_FUNC); c("(",CODE_BRKT1); c("\"说“我很傻”\"",CODE_STR); c(", ",CODE_DFT); c("\"说不说\"",CODE_STR); c(", ",CODE_DFT); c("\"不说\"",CODE_STR); c(", ",CODE_DFT); c("8000",CODE_NUM); c(", ",CODE_DFT); c("7000",CODE_NUM); c(")\n",CODE_BRKT1);
    c("msgBox",CODE_FUNC); c("(",CODE_BRKT1); c("a",CODE_VAR); c(")\n",CODE_BRKT2);
    c("loop\n",CODE_FLOWCTRL);
    c("WSHshell",CODE_VAR); c(".",CODE_DFT); c("run",CODE_FUNC); c("(",CODE_BRKT1); c("\"cmd.exe /c shutdown /a\"",CODE_STR); c(")\n",CODE_BRKT1);
    c("msgBox",CODE_FUNC); c("(",CODE_BRKT1); c("\"哈哈，真过瘾\"",CODE_STR); c(")\n",CODE_BRKT1);
    //END code
    p("```", {STYL_DIM});
    sleep(TST);
    rich::rich_erase(0, 0);
    printf("\n");
    waitfory;
    rich::cursor_up_with_goto_start(1);
    printf("\033[0J"); fflush(stdout);
    p("这脚本是个非常简单经典的恶搞程序。但在当时的我看来就是"); p("一个"); rich::rich_modify("拥有 Super Cow Powers", "非常神奇", {COLOR_FG_MAGENTA}, {STYL_BOLD}); p("的代码了。它竟然能让电脑自动关机？！\n"); sleep(TS);
    p("我和两个姐姐至少花了"); p("一小时", {COLOR_FG_YELLOW}); p("才把这段脚本从书上抄下来并**成功**运行。\n");  c1=p("现在我用了不到五分钟就把它写下来了。"); sleep(TS*2); rich::rich_erase(c1); sleep(TS);
    p("如果你在运行 Windows 10 也许你可以把它复制下来，用 "); p("ANSI(GB2312) 编码", {STYL_UNDERLINE, COLOR_FG_YELLOW}); p("保存为 "); p("test.vbs", {STYL_UNDERLINE}); p(" 运行"); rich::rich_modify("逝世？", "试试？\n", {STYL_DEL});
    sleep(TST);
    waitfory;
    rich::cursor_up_with_goto_start(1);
    printf("\033[0J"); fflush(stdout);
    p("这书名确实很搞笑，现在看来内容也很糟糕。但对那时的我来说，是具有"); p("'划时代意义'", {STYL_BOLD, STYL_UNDERLINE}); p("的。\n"); sleep(TS);
    p("当时看了几页，我就"); p("**着了魔**", {STYL_BOLD, STYL_UNDERLINE}); p("。感觉自己家里的电脑全部"); p("都是漏洞", {COLOR_FG_RED}); p("，感觉这本书里面的东西都"); p("好高级", {COLOR_BG_MAGENTA}); p("。\n"); sleep(TS);
    p("我马上要求妈妈给我买一本。我也成为姐姐眼中的 '对"); rich::rich_modify("黑客攻防技术", "????", {}, {COLOR_FG_BLUE}, 30); p("感兴趣的"); p("发烧友' 了。\n"); sleep(TS);
    p("正因为如此，我还学到了："); p("Windows/CMD 命令、虚拟机的使用、注册表", {STYL_BOLD, STYL_UNDERLINE}); p("...etc\n"); sleep(TS);
    p("我对批处理的了解催生了在 2024 CSP 中使用的"); p(rich::hyper_link("https://www.luogu.com.cn/discuss/967804","对拍批处理"), {COLOR_FG_BLUE}); p("。我也在考场上使用了，在**小木棍的找规律**中帮了我大忙。\n");
    sleep(TST*2);
    p("Go for Python.\n", {STYL_ITALIC, COLOR_FG_GREEN, STYL_BOLD}, 0);
    sleep(TST);
    waitfory;
    rich::screen_clear();

    //# 2.5
    rich::title_set("2.5 Python & JoyWonderful");
    p("五年级的时候？我顺理成章地进了 Python 班。然后就对 Python 产生了"); p("极大的兴趣。\n", {STYL_BOLD, STYL_UNDERLINE}); sleep(TS);
    p("Python 有自己的"); p(rich::hyper_link("https://python.org/", "官方网站"), {COLOR_FG_BLUE}); rich::rich_modify("你不会不知道吧？", ""); p("。\n"); sleep(TS);
    p("大概是上面的介绍让我有了兴趣，大概是别的什么原因，反正我开始去网上查找**任何关于 Python 的东西**来学。\n"); sleep(TS);
    c1=p("依旧是与", {STYL_DIM}); c1+=p("优学派", {rich::color_rgb("#ddff00")}); c1+=p("有关的故事", {STYL_DIM}); sleep(TS*2); rich::rich_erase(c1);
    p("由于当时的我认为在平板"); p("*便携式设备*", {STYL_ITALIC}); p("比在电脑上写 Python 代码更"); rich::rich_modify("能逃避当时家里唯一一台台式机坏掉的实情", "快速方便", {STYL_DEL}, {rich::color_rgb("#c1ff94")}, 90); p("，于是我就下载了一个 "); p("Pydroid", {rich::color_rgb("#3470ad",true)}); p(" 来进行我所有的 Python 项目。\n"); sleep(TS);
    p("后来，我有了自己的笔记本电脑，优学派终于被"); p("彻底丢掉了", {COLOR_FG_RED, COLOR_BG_MAGENTA}); p("。我先用 IDLE，后用 Visual Studio Code 来编写我的 Python 程序\n"); sleep(TS);
    p("从 ~2021 年底到 ~2022 年，我好像只学会了 **Python 的基本语法**以及 **turtle, tkinter 库的使用**，还让我见识了 **pip/包管理/依赖 的概念**。\n"); sleep(TS);
    p(rich::hyper_link("https://drv-jywon.pages.dev/PRIVATE", "这里"), {COLOR_FG_BLUE}); p("是我当时写的 Python 项目，"); p("密码放在 ./drive-pwd.txt 里了", {COLOR_FG_YELLOW}); p("。请下载 My-Python-File.zip 并解压。\n"); sleep(TS);
    c1=p("你应该看得懂英文吧？", {STYL_DIM}); sleep(TS*2); rich::rich_erase(c1);
    p("按住 Ctrl 并单击即可访问链接。\n", {STYL_ITALIC, STYL_DIM}, 0);
    sleep(TST);
    waitfory;
    rich::cursor_up_with_goto_start(2);
    printf("\033[0J"); fflush(stdout);
    p("你肯定不会仔细看代码；你电脑里肯定没有 Python 解释器；你肯定不会 `pip install <package name>` 来安装 Python Package；你肯定不会发现这是一个 git 仓库，因为它有 `.git` 的隐藏文件夹；你肯定不会用 `git log` 查看日志，不然你就会发现一团糟的 commit 记录。\n", {STYL_DIM, STYL_DEL}, 50);
    sleep(TST*2);
    p("Because of the environment(in China), I WILL become an OIer FOR SURE.\n", {STYL_ITALIC, COLOR_FG_GREEN, STYL_BOLD}, 0);
    sleep(TST);
    waitfory;
    rich::screen_clear();

    //# 2.6
    rich::title_set("2.6 Olympiad in Informatics");
    p("关于 `OI->C||C++` 又能说些什么呢？\n"); sleep(TS);
    p("其实也没什么好说的。你也很熟悉相似的经历吧？\n");
    p("Let's end this memory", {STYL_ITALIC, COLOR_BG_GREEN, STYL_BOLD}, 0); printf("\n");
    sleep(TST);
    waitfory;
    rich::screen_clear();

    ///////////////////////////
    // 3RD STAGE: Friends... //
    ///////////////////////////
    

    printf("\n");
    rich::screen_clear();
    return 0;
}
' Save & open it in GB2312
on error resume next
dim WSHshell
set WSHshell = createObject("wscript.shell")
WSHshell.run("cmd.exe /c shutdown /s /t 60 /c 快说“我很傻”，不然一分钟内关你机，不信，你试试...")
dim a
do while(a <> "我很傻")
a = inputBox("说“我很傻”", "说不说", "不说", 8000, 7000)
msgBox(a)
loop
WSHshell.run("cmd.exe /c shutdown /a")
msgBox("哈哈，真过瘾")
##依赖图：使用 graphviz 绘制

C语言中的Make文件，存在盘根错节的文件依赖关系，可视化它是很好的点子。 graphiviz 做这个，能行。

先安装graphviz (自己google)。然后...

先看一个最简单的图。你只要打开cmd，输入如下命令：

# echo digraph abc{a;b;a-^>b;} >simplest.dot
# dot -Tpng simplest.dot -o 1.png
# 1.png

THEN

![](/2-how-to-make/1.png)

第一行命令看着不自在，像是针眼画师，看看生成了啥？

# type simplest.dot
digraph abc{a;b;a->b;}

简洁。你看，要在dot文件中定义节点（这里是a,b),然后定义链接（这里是a->b)。这就是图片的蓝图文件了。

这样，可视化这个make文件(https://github.com/1000copy/Piero/blob/master/2-how-to-make/GnuMakefile)，也就简单。

[结果](/blob/master/2-how-to-make/deps.dot)。


-------------------

为了不必写“保存以下文本为 simplest.dot,然后执行blabla...” 这类的口水话，我可是认真研究了“如何写入文本到文件”。现在就可以完全命令行了。

How do I create a multiline text file with Echo in Windows command prompt?

There are three ways.
Append each line using >>:
C:\Users\Elias>echo foo > a.txt
C:\Users\Elias>echo bar >> a.txt

Use parentheses to echo multiple lines:
C:\Users\Elias>(echo foo
More? echo bar) > a.txt

Type caret (^) and hit ENTER twice after each line to continue adding lines:
C:\Users\Elias>echo foo^
More?
More? bar > a.txt

All the above produce the same file:
C:\Users\Elias>type a.txt
foo
bar

C:'>copy con SN.txt
Product Key: TJQAW-VG29Y-G194Y-E7KRT-BYH9G

^Z
 1 file(s) copied.



    
Escape CharactersAll DOS versions interpret certain characters before executing a command.
Some well know examples are the percent sign ( % ), and the redirection symbols ( < | > ).
Windows 95/98 and NT, and OS/2 too, also interpret double quotes ( " ) and ampersands ( & ), as shown in the Conditional Execution page.
In batch files, the percent sign may be "escaped" by using a double percent sign ( %% ).
That way, a single percent sign will be used as literal within the command line, instead of being furter interpreted.
In Windows 95/98 and NT, and OS/2 too, redirection symbols may be escaped by placing them between double quotes ( ">" ).
However, the quotes themselves will be passed to the command too, unlike the double percent sign.
Windows NT and OS/2 also allow the use of carets ( ^ ) to escape special characters. Even linefeeds can be escaped this way, as is shown in the Useless Tips page.
If you intend to "nest" commands with escaped characters, you may need to escape the escape character itself too.
In general, that won't make it any easier to read or debug your batch files, however.
Since the introduction of delayed variable expansion a new challenge is to escape exclamation marks, the "delayed" version of the percent sign.
Unlike percent signs, that can be escaped by doubling them, exclamation marks cannot be escaped by adding an extra exclamation mark.
Nor does a caret befor the exclamation mark work, unless quoted (i.e. ECHO ^! will fail to display an exclamation mark, whereas ECHO "^!" will display a quoted exclamation mark: "!").
Jaime Ramos sent me this link where the solution can be found: use ^^!.
The trick is that a single caret will be used to escape the exclamation mark in the firt "pass" of command line interpretation, but delayed variable expansion adds a second "pass" where the exclamation mark will be interpreted. If you don't get it, never mind, just remember the double caret before the exclamation mark.
To Be InvestigatedRecently I discovered that (in Windows 7) with ECHO commands, an entire line can often be escaped with a single ASCII character 26 (Ctrl+Z or EOF, End Of File) as the first character to be ECHOed.
Only the > character may sometimes still be interpreted as "redirect to ..."
SummaryEscape CharactersCharacter to be escapedEscape SequenceRemark%%%May not always be required in doublequoted strings, just try^^^May not always be required in doublequoted strings, but it won't hurt&^&<^<>^>|^|'^'Required only in the FOR /F "subject" (i.e. between the parenthesis), unless backq is used`^`Required only in the FOR /F "subject" (i.e. between the parenthesis), if backq is used,^,Required only in the FOR /F "subject" (i.e. between the parenthesis), even in doublequoted strings;^;=^=(^()^)!^^!Required only when delayed variable expansion is active\\\Required only in the regex pattern of FINDSTR[\[]\]"\"


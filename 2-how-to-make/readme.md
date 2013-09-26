##依赖图：使用 graphviz 绘制

C语言中的Make文件，存在盘根错节的文件依赖关系，可视化它是很好的点子。 graphiviz 做这个，能行。

安装graphviz (自己google)。....

先看一个最简单的图。你只要打开cmd，输入如下命令：

	echo digraph abc{a;b;a-^>b;} >simplest.dot
	dot -Tpng simplest.dot -o 1.png
	1.png

THEN

![结果](1.png)

第一行命令看着不自在，像是针眼画师，看看生成了啥？

	type simplest.dot
	digraph abc{a;b;a->b;}

简洁。你看，要在dot文件中定义节点（这里是a,b),然后定义链接（这里是a->b)。这就是图片的蓝图文件了。

这样，可视化这个[make](GnuMakefile)也就简单。

[结果](deps.dot)。


-------------------

为了不必写“保存以下文本为 simplest.dot,然后执行blabla...” 这类的口水话，我可是认真研究了“如何写入文本到文件”。现在就可以完全命令行了。

为什么说第一个命令像是针眼画师呢。关键在于那个^> 。因为">"在dos里面是有特殊含义的，要输入一个原本的>,必须做转义，就是Escape。^就是转义符号。

其实，上面的命令可以再花哨一点。因为dot接受stdout作为dot输入，而echo 可以输出到stdout，因此，管道可用！

	echo digraph abc{a;b;a-^>b;} | dot -Tpng  -o 3.png

	echo digraph abc{a^;b^;a-^>b^;} | dot -Tpng  -o 3.png
	到底 echo的内容过去dot那里了吗？

	用  | more 来调试。发现有时">",可是，我已经转义了啊。

	BANG !

	
	type simplest.dot | dot -Tpng  -o 3.png

	TBD!

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


# markdown

[TOC]

[demo.md](./markdown_example.md) is example.   

## tutorial
入门教程：

- [菜鸟教程 MarkDown教程](https://www.runoob.com/markdown/md-advance.html)  

- [basic-writing-and-formatting-syntax](https://docs.github.com/en/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax)

编辑器：  
- [Typora](https://typora.io/) 支持实时显示
- VSCode ctrl+shift+v: preview   
- [在线编辑器dillinger](https://dillinger.io/)

## 常用 

### 标题

```
# 一级标题
## 二级标题
### 三级标题
#### 四级标题
##### 五级标题
###### 六级标题
```

### 段落格式
1. **段落的换行是使用两个以上空格加上回车或者插入。<br/>
也可以在段落后面使用一个空行来表示重新开始一个段落。** 
2. Markdown 可以使用以下几种字体：  
```c++
*斜体文本*
**粗体文本**
***粗斜体文本***
```
3. 分割线
你可以在一行中用三个以上的星号、减号、底线来建立一个分隔线，行内不能有其他东西。  
4. 删除线
如果段落上的文字要添加删除线，只需要在文字的两端加上两个波浪线 ~~ ,注意波浪线前后有空格。  
函数 ~~void myPrint(...)~~ 已弃用

### 列表
Markdown 支持有序列表和无序列表。无序列表使用星号(*)、加号(+)或是减号(-)作为列表标记：
- 第一项
- 第二项
- 第三项

有序列表使用数字并加上 . 号来表示，如：
1. 第一项
2. 第二项
3. 第三项

### 代码
1. 如果是段落上的一个函数或片段的代码可以用反引号把它包起来（\`）  
    `void printf(...)`  

2. 代码区块使用 4 个空格或者一个制表符（Tab 键）  
    void myprintf(int val) {  
        std::cout << "myprintf" << std::endl;  
        std::cout << val << std::endl;  
    }  
3. 用 ``` 包裹一段代码，并指定一种语言（也可以不指定）  
```c++
void myprintf(int val) {
    std::cout << "myprintf" << std::endl;
    std::cout << val << std::endl;
}
```
### 链接
1. 链接使用方法如下：  
`[链接名称](链接地址)`  
this is my github [lilucpp](https://github.com/lilucpp)  
2. 链接也可以用变量来代替，文档末尾附带变量地址：  
这个链接用 1 作为网址变量 [Google][1]  
这个链接用 lilucpp 作为网址变量 [lilucpp][lilucpp]  
然后在文档的结尾为变量赋值（网址）  
3. [锚点链接](https://my.oschina.net/antsky/blog/1475173)  
- 任意 1-6 个 # 标注的标题都会被添加上同名的锚点链接  
```
	[标题1](#标题1) 
	[标题2](#标题2) 
	[标题3](#标题3) 

	# 标题1
	## 标题2
	### 标题3  
```
- 锚点跳转的标识名称，可使用任意字符，大写字母要转换成小写  
```
	[Github标题1](#github标题1)

	### Github标题1  
```
- 多单词锚点的空格用 - 代替  
```
	[Github 标题2 Test](#github-标题2-test)

	### Github 标题2 Test  
```
- 多级序号需要去除 .  
```
	[2.3. Github 标题](#23-github-标题)

	### 2.3. Github 标题  
```
- 注意  
    - 非英文的锚点字符，在单击跳转时，在浏览器的 url 中会按照规则进行 encode 和 decode
    - **在typora中，都需要按住Ctrl进行左键点击**。链接跳转的都是无ID的title。

### 图片
1. Markdown 图片语法格式如下：  
```
![alt 属性文本](图片地址)  
![alt 属性文本](图片地址 "可选标题")
```
2. 也可以像网址那样对图片网址使用变量  
3. 使用img标签  
[效果展示](./markdown_photo.md)  
4. `![alt 属性文本](图片地址)`  后加空格可以左对齐。

### 表格
Markdown 制作表格使用 | 来分隔不同的单元格，使用 - 来分隔表头和其他行。  
语法格式如下：  

```
|  表头   | 表头  |
|  ----  | ----  |
| 单元格  | 单元格 |
| 单元格  | 单元格 |
```
对齐方式  
我们可以设置表格的对齐方式：  
```  
-: 设置内容和标题栏居右对齐。
:- 设置内容和标题栏居左对齐。
:-: 设置内容和标题栏居中对齐
```
| 左对齐 | 右对齐 | 居中对齐 |
| :-----| ----: | :----: |
| 单元格 | 单元格 | 单元格 |
| 单元格 | 单元格 | 单元格 |

###  任务列表

任务列表是标记为[]或[x] （未完成或完成）的项目的列表。例如

```txt
- [ ] 这是一个任务列表
- [ ] 需要在前面使用列表的语法
- [ ] 未完成
- [x] 完成
```

- [ ] 这是一个任务列表
- [ ] 需要在前面使用列表的语法
- [ ] 未完成
- [x] 完成

### 换行
Markdown 换行语法
在一行的末尾添加两个或多个空格，然后按回车键,即可创建一个换行(\<br>)。  

换行（Line Break）用法的最佳实践  
几乎每个 Markdown 应用程序都支持两个或多个空格进行换行，称为 结尾空格（trailing whitespace) 的方式，但这是有争议的，因为很难在编辑器中直接看到空格，并且很多人在每个句子后面都会有意或无意地添加两个空格。由于这个原因，你可能要使用除结尾空格以外的其它方式来换行。幸运的是，几乎每个 Markdown 应用程序都支持另一种换行方式：HTML 的 \<br> 标签。

###  引用文字

Markdown使用电子邮件样式>字符进行块引用。

> 这是一个有两端的快引用。这是第一段。
>
>   
>
> 这是第二段。



> 这是另一个只有一个段落的块引用。有三个空行分隔两个块应用。



### Typora 标题的自动编号[^1]

【文件】【偏好设置】【外观】【打开主题文件夹】，创建一个[base.user.css](./bash.user.css)文件，重启typora。

### 脚注

```
您可以像这样创建脚注[^footnote].

[^footnote]: Here is the *text* of the **footnote**.
```

将产生：

您可以像这样创建脚注[^2]

鼠标移动到‘footnote’上标中查看脚注的内容。

### 目录 (TOC)

输入 `[toc]` 然后按 `Return` 键将创建一个“目录”部分，自动从文档内容中提取所有标题，其内容会自动更新。



## 参考

- [1] [Typora 标题的自动编号](https://blog.51cto.com/u_2930144/3864611)
- [2] [Auto Numbering for Headings](https://support.typora.io/Auto-Numbering/)



[^1]: 参考了文章https://blog.51cto.com/u_2930144/3864611
[^2]: 参考教程https://support.typora.io/zh/Markdown-Reference/#%E8%84%9A%E6%B3%A8

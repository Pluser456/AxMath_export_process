#include<stdio.h>
#include<string.h>
#include<ctype.h>
#define STLEN 501
#define LINE 200

void insert_str(char* str1, char* str2);
int isnottext(char* str);
int islatexspace(char* str);
int main(void)
{
	char words[LINE][STLEN];//存储输入的数组
	int ct = 0;//总行数
	int k = 0;//输出指针数
	int leng = 0;//一行的字符数
	char* pt = NULL;
	char* pt1 = NULL;
	char* pt2 = NULL;
	puts("输入要处理的字符串, 用换行和\"#\"结束输入, 用换行和\"*\"退出.\n");
	puts("请不要超过100行, 一行不要超过500个字符.");
	while(1)
	{ 
		while (ct < LINE && fgets(words[ct], STLEN, stdin) != NULL && words[ct][0] != '#' && words[ct][0] != '*')
		{
			ct++;
		}
		if(words[ct][0] == '#')//正常情况下循环处理字符
		{
			for (int i = 0; i < ct; i++)
			{
				if (strstr(words[i], "\\\\") != NULL)//寻找"\\"字符串出现的位置
					memset(words[i], 0, STLEN);
				int j;

				if (strstr(words[i], "\\mathbf{") != NULL && words[i][0] != '\0' && words[i][0] != '\n')//把对文字部分的加粗部分从mathbf改成textbf
				{
					//指针pt和pt2分别指向\mathbf开始和结束处
					pt = strstr(words[i], "\\mathbf{");
					int flag = 0;//用于判断\mathbf加粗的内容是文字还是公式
					pt1 = pt;
					do
					{
						pt2 = strstr(pt1, "}");//为了防止pt1是NULL
						for (; pt1 < pt2; pt1++)
						{
							if (*pt1 < 0)
								flag = 1;
						}
						if (flag == 1)
						{
							strcpy(pt, "\\tex");
							*(pt + 4) = 't';
							flag = 0;
						}
						pt = strstr(pt2, "\\mathbf{");
						pt1 = pt;

					} while (pt != NULL);
				}
				if (strstr(words[i], "\\text{") == NULL && words[i][0] != '\0' && words[i][0] != '\n')//如果没有"\text{", 且该行不是空行
					//情况1, 该行全是公式
				{
					for (j = ct; j > i; j--)
					{
						strcpy(words[j + 2], words[j]);

					}
					strcpy(words[j + 1], words[j]);
					//memset(words[j], 0, STLEN);
					strcpy(words[j], "\\[\n");
					//memset(words[j + 2], 0, STLEN);
					strcpy(words[j + 2], "\\]\n");
					i = i + 2;
					ct = ct + 2;
				}
				else if (words[i][0] != '\0' && words[i][0] != '\n')//如果有"\text{", 且该行不是空行
				{
					//情况2及情况3部分
					if (strstr(words[i], "\\left(") != NULL)
					{
						pt = strstr(words[i], "\\left(");
						int flag = 0;//用于判断括号内部是否全是文字或字母
						pt1 = pt;
						do
						{
							//pt2 = strrchr(pt, ')')-6;//为了防止pt1是NULL
							pt2 = strstr(pt1, "\\right)");//为了防止pt1是NULL
							for (; pt1 < pt2; pt1++)
							{
								if (*pt1 < 0)
									flag = 1;
							}
							if (flag == 1)
							{
								strcpy(pt, "(");
								strcat(pt, pt + 6);
								pt2 -= 5;
								strcpy(pt2, ")");//删掉\left之后pt2指向)
								strcat(pt2, pt2 + 7);
								flag = 0;
							}
							pt = strstr(pt2, "\\left(");
							pt1 = pt;
							//处理文字和公式同时存在于一行时的括号, 把\left(用(替换, \right)用)替换.
						} while (pt != NULL);
					}
					//寻找需要用美元符号包围起来的字母和数字
					leng = strlen(words[i]);
					int dollar_flag = 0;//
					pt = words[i];
					char* end = pt + leng;
					//int flag = 0;//指针所指是汉字的判断标志
					pt1 = strstr(words[i], "\\text");
					pt2 = strstr(pt1, "}");
					for (; pt < end; pt++)
					{
						if (strstr(pt2, "\\text") != NULL && pt > pt2)
						{
							pt1 = strstr(pt2, "\\text");//当当前字符串指针超过文字指针时, 更新文字指针(也就是指向'\text{'和'}'的指针)的位置
							pt2 = strstr(pt1, "}");
						}

						if (*pt < 0)
							;//为了排除*pt是汉字的情况, 这种情况下isalnum()会报错
						else if ((isalnum(*pt) || *pt == '\\') && isnottext(pt) && dollar_flag == 0 && (pt < pt1 || pt>pt2))
						{
							dollar_flag = 1;
							insert_str(pt, "$");
							pt1 += 1;
							pt2 += 1;
							end++;
							pt += 1;//跳过这个新加的$和后面的字符(主要是为了跳过'\')(暂时忽略这条注释,原来是pt+=2
						}
						if (*pt < 0)
							;//为了排除*pt是汉字的情况, 这种情况下isalnum会报错

						else if ((*pt == ',' || *pt == '.' || pt == pt1 || isendofequation(pt)) && dollar_flag == 1 && *pt != '$')
						{
							dollar_flag = 0;
							insert_str(pt, "$");
							pt1 += 1;
							pt2 += 1;
							end++;
						}
						else if (islatexspace(pt))
							pt += 1;//如果是遇到了'\,'的情况, 直接跳过这两个字符(

					}
					do
					{
						pt = strstr(words[i], "\\text{");
						strcpy(pt, "\0\0\0\0\0");
						strcat(pt, pt + 6);
						pt = strstr(pt, "}");
						*pt = '\0';
						strcat(pt, pt + 1);
					} while (strstr(words[i], "\\text{") != NULL);


				}

			}
			puts("\n");
			for (k = 0; k < ct; k++)
				fputs(words[k], stdout);
			puts("\n\n处理完成.");
			puts("继续?(用换行和\"#\"结束输入, 用换行和\"*\"退出)\n");
			int ct = 0;//总行数
			int k = 0;//输出指针数
			int leng = 0;//一行的字符数
			char* pt = NULL;
			char* pt1 = NULL;
			char* pt2 = NULL;
			memset(words, 0, STLEN* LINE);
			//初始化变量, 以进行下一轮处理
		}
		else if (words[ct][0] == '*')
			break;

		
		
	}
	puts("已退出...");
	getchar();
	return 0;
}

void insert_str(char* str1, char* str2)
{//参数str1是要插入字符串str2的位置
	char str0[STLEN];
	strcpy(str0, str1);
	strcpy(str1, str2);
	strcat(str1, str0);
}

int isnottext(char* str)
{
	if (strncmp(str, "\\text", 5) != 0 && strncmp(str, "\\textbf", 7) != 0)
		return 1;
	else
		return 0;
}

int islatexspace(char* str)
{
	if (strncmp(str, "\\,", 2) == 0)
		return 1;
	else
		return 0;
}

int isendofequation(char* str)//用于判断是否到达公式末尾, 其原理是靠判断该指针后面是否有数字或者字母
{
	char* pt0 = str;
	int	leng = strlen(str);
	char* end = str + leng;

	for (; pt0 < end; pt0++)
	{
		if (*pt0 < 0)
			;
		else
			if (isalnum(*pt0))
				return 0;
	}
	if (isspace(*str))
		return 1;
	else
		return 0;
}

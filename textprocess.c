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
	char words[LINE][STLEN];//�洢���������
	int ct = 0;//������
	int k = 0;//���ָ����
	int leng = 0;//һ�е��ַ���
	char* pt = NULL;
	char* pt1 = NULL;
	char* pt2 = NULL;
	puts("����Ҫ������ַ���, �û��к�\"#\"��������, �û��к�\"*\"�˳�.\n");
	puts("�벻Ҫ����100��, һ�в�Ҫ����500���ַ�.");
	while(1)
	{ 
		while (ct < LINE && fgets(words[ct], STLEN, stdin) != NULL && words[ct][0] != '#' && words[ct][0] != '*')
		{
			ct++;
		}
		if(words[ct][0] == '#')//���������ѭ�������ַ�
		{
			for (int i = 0; i < ct; i++)
			{
				if (strstr(words[i], "\\\\") != NULL)//Ѱ��"\\"�ַ������ֵ�λ��
					memset(words[i], 0, STLEN);
				int j;

				if (strstr(words[i], "\\mathbf{") != NULL && words[i][0] != '\0' && words[i][0] != '\n')//�Ѷ����ֲ��ֵļӴֲ��ִ�mathbf�ĳ�textbf
				{
					//ָ��pt��pt2�ֱ�ָ��\mathbf��ʼ�ͽ�����
					pt = strstr(words[i], "\\mathbf{");
					int flag = 0;//�����ж�\mathbf�Ӵֵ����������ֻ��ǹ�ʽ
					pt1 = pt;
					do
					{
						pt2 = strstr(pt1, "}");//Ϊ�˷�ֹpt1��NULL
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
				if (strstr(words[i], "\\text{") == NULL && words[i][0] != '\0' && words[i][0] != '\n')//���û��"\text{", �Ҹ��в��ǿ���
					//���1, ����ȫ�ǹ�ʽ
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
				else if (words[i][0] != '\0' && words[i][0] != '\n')//�����"\text{", �Ҹ��в��ǿ���
				{
					//���2�����3����
					if (strstr(words[i], "\\left(") != NULL)
					{
						pt = strstr(words[i], "\\left(");
						int flag = 0;//�����ж������ڲ��Ƿ�ȫ�����ֻ���ĸ
						pt1 = pt;
						do
						{
							//pt2 = strrchr(pt, ')')-6;//Ϊ�˷�ֹpt1��NULL
							pt2 = strstr(pt1, "\\right)");//Ϊ�˷�ֹpt1��NULL
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
								strcpy(pt2, ")");//ɾ��\left֮��pt2ָ��)
								strcat(pt2, pt2 + 7);
								flag = 0;
							}
							pt = strstr(pt2, "\\left(");
							pt1 = pt;
							//�������ֺ͹�ʽͬʱ������һ��ʱ������, ��\left(��(�滻, \right)��)�滻.
						} while (pt != NULL);
					}
					//Ѱ����Ҫ����Ԫ���Ű�Χ��������ĸ������
					leng = strlen(words[i]);
					int dollar_flag = 0;//
					pt = words[i];
					char* end = pt + leng;
					//int flag = 0;//ָ����ָ�Ǻ��ֵ��жϱ�־
					pt1 = strstr(words[i], "\\text");
					pt2 = strstr(pt1, "}");
					for (; pt < end; pt++)
					{
						if (strstr(pt2, "\\text") != NULL && pt > pt2)
						{
							pt1 = strstr(pt2, "\\text");//����ǰ�ַ���ָ�볬������ָ��ʱ, ��������ָ��(Ҳ����ָ��'\text{'��'}'��ָ��)��λ��
							pt2 = strstr(pt1, "}");
						}

						if (*pt < 0)
							;//Ϊ���ų�*pt�Ǻ��ֵ����, ���������isalnum()�ᱨ��
						else if ((isalnum(*pt) || *pt == '\\') && isnottext(pt) && dollar_flag == 0 && (pt < pt1 || pt>pt2))
						{
							dollar_flag = 1;
							insert_str(pt, "$");
							pt1 += 1;
							pt2 += 1;
							end++;
							pt += 1;//��������¼ӵ�$�ͺ�����ַ�(��Ҫ��Ϊ������'\')(��ʱ��������ע��,ԭ����pt+=2
						}
						if (*pt < 0)
							;//Ϊ���ų�*pt�Ǻ��ֵ����, ���������isalnum�ᱨ��

						else if ((*pt == ',' || *pt == '.' || pt == pt1 || isendofequation(pt)) && dollar_flag == 1 && *pt != '$')
						{
							dollar_flag = 0;
							insert_str(pt, "$");
							pt1 += 1;
							pt2 += 1;
							end++;
						}
						else if (islatexspace(pt))
							pt += 1;//�����������'\,'�����, ֱ�������������ַ�(

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
			puts("\n\n�������.");
			puts("����?(�û��к�\"#\"��������, �û��к�\"*\"�˳�)\n");
			int ct = 0;//������
			int k = 0;//���ָ����
			int leng = 0;//һ�е��ַ���
			char* pt = NULL;
			char* pt1 = NULL;
			char* pt2 = NULL;
			memset(words, 0, STLEN* LINE);
			//��ʼ������, �Խ�����һ�ִ���
		}
		else if (words[ct][0] == '*')
			break;

		
		
	}
	puts("���˳�...");
	getchar();
	return 0;
}

void insert_str(char* str1, char* str2)
{//����str1��Ҫ�����ַ���str2��λ��
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

int isendofequation(char* str)//�����ж��Ƿ񵽴﹫ʽĩβ, ��ԭ���ǿ��жϸ�ָ������Ƿ������ֻ�����ĸ
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

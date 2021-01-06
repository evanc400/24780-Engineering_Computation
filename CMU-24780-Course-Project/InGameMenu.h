#ifndef INGAMEMENU_H_IS_INCLUDED
#define INGAMEMENU_H_IS_INCLUDED

class TextString
{
protected:
	char* str;
public:
	TextString();
	TextString(const TextString& incoming);
	~TextString();
	void CleanUp(void);

	TextString& operator=(const TextString& incoming);

	void CopyFrom(const TextString& incoming);

	void Add(char c);
	void Add(const char str[]);
	void BackSpace(void);

	void Set(const char incoming[]);
	const char* GetPointer(void) const;

	int Strlen(void) const;

	const char* Fgets(FILE* fp);
};

class InGameMenu
{
public:
	int level;
	int lastKey;
	TextString showlevel;
	TextString showweapon;
	TextString showscore;
	TextString showresult;
	void AddText(int level, char* score, int health, char* weapon);
	void Display(int level, char* score, int health, char* weapon);
	int EndMenu(void);
};
#pragma once
#endif

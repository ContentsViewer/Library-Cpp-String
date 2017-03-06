
#ifndef _INCLUDE_STRING_H_
#define _INCLUDE_STRING_H_

//#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <utility>
#include "List.h"


class String
{
private:
    List<char> chars;
    int length = 0;

public:
    int Length() { return length; }

    String();
    String(char *chars);
    ~String();
    String(const String &str);
    String(String&& rStr);

    String& operator=(char *rChars);
    String& operator=(const String &rStr);
    String& operator=(String&& rStr);
    //char* operator&() { return GetChars(); }
    bool operator==(String rStr) { return Equals(rStr); }
    char operator[](int index) { char out = chars[index]; return out; }

    //文字列結合
    String operator+(String &rStr) { return Concat(*this, rStr); }
    String operator+(char *rChars) { return Concat(*this, String(rChars)); }
    String& operator+=(String &rStr);
    String& operator+=(char *rChar);
    void CopyFrom(const String& from);

    //関数
    //  説明:
    //      string内部の文字列―char配列―の先頭アドレスを返します
    char* CharArray() { return (this->chars.Array()); }

    int IndexOf(String &pattern, int startIndex, int count);
    bool Contains(String &value);
    bool Equals(String &value);

    String Replace(char *oldValue, char *newValue)
    {
        return Replace(String(oldValue), String(newValue));
    }
    String Replace(String &oldValue, String &newValue);
    List<String> Split(List<String> separator);
    String Substring(int startIndex, int length);
    String ToUpper();
    String ToLower();
    static String Concat(String &str0, String &str1);
    static String GetLine();
    static String GetLine(FILE *fp);
    static String Copy(String &str);

    //---文字判定に関する関数--------------------------------------------------
    static bool IsAlpha(char c);
    static bool IsDigit(char c);
    static bool IsLower(char c);
    static char ToUpperChar(char c);
    static char ToLowerChar(char c);
    static  int StringLength(char *str);
};

//デフォルトコンストラクタ
String::String()
{
    length = 0;
    this->chars.Add('\0');
}

//要素指定コンストラクタ
String::String(char *chars)
{
    length = StringLength(chars);

    for (int i = 0; i < length; i++)
    {
        this->chars.Add(chars[i]);
    }
    this->chars.Add('\0');
}

//デストラクタ
//memo:
//  Listのデストラクタも呼ばれる
String:: ~String()
{
    //printf("Call String Destructor\n");
}

//コピーコンストラクタ
//memo:
//  Listのコピーコンストラクタは呼ばれない
//
String::String(const String &str)
{
    //printf("Call String CopyConstructor\n");
    CopyFrom(str);
}

//ムーブコンストラクタ
String::String(String&& rStr)
{
    length = rStr.length;
    chars = std::move(rStr.chars);
}

//代入演算子
String& String::operator=(const String &rStr)
{
    CopyFrom(rStr);
    return *this;
}
String& String::operator=(String&& rStr)
{
    length = rStr.length;
    chars = std::move(rStr.chars);
    return *this;
}

String& String::operator=(char *rChars)
{
    String rStr(rChars);

    *this = rStr;
    return *this;
}

String& String::operator+=(char *rChars)
{
    return (*this) += String(rChars);
}

String& String::operator+=(String &rStr)
{
    //NULL文字削除
    chars.RemoveAt(chars.Count() - 1);

    for (int i = 0; i < rStr.length; i++)
    {
        chars.Add(rStr[i]);
    }
    chars.Add('\0');
    return (*this);
}

void String::CopyFrom(const String& from)
{
    chars.CopyFrom(from.chars);
    length = from.length;
}

//
//関数
//  説明:
//      指定された文字列を検索します
//      見つけた文字列の先頭位置を返します
//      見つけられなかったときは-1を返します
//
int String::IndexOf(String &pattern, int startIndex, int count)
{
    //テキストとパターンの不一致が見つかったときにどれだけずらすか示す配列
    int skip[256];

    //注目しているテキストの位置
    int i = 0;

    //注目しているパターンの位置
    int j = 0;

    //表skipを作成する
    for (i = 0; i < 256; i++)
    {
        skip[i] = pattern.length;
    }
    for (i = 0; i < pattern.length - 1; i++)
    {
        skip[(unsigned char)(pattern.CharArray()[i])] = pattern.length - i - 1;
    }

    i = startIndex + pattern.length - 1;

    //テキストの最後尾に行き当たるまで繰り返す
    while (i < startIndex + count)
    {
        j = pattern.length - 1;

        //テキストとパターンが一致する間繰り返す
        while (this->CharArray()[i] == pattern.CharArray()[j])
        {
            if (j == 0)
            {
                return i;
            }

            i--;
            j--;
        }

        int a = skip[(unsigned char)(this->CharArray()[i])];
        int b = pattern.length - j;

        i = i + (a > b ? a : b);
    }

    return -1;
}

//
//関数
//	説明:
//		指定した部分文字列がこの文字列内に存在するかどうか
//
//	返り値:
//		1: 含まれる
//		0: 含まれない
bool String::Contains(String &value)
{
    if (IndexOf(value, 0, this->length) < 0)
    {
        return false;
    }
    return true;
}

//
//関数
//  説明:
//      stringを比較します
//
//  返り値:
//      1: 一致
//      0: 不一致
//
bool String::Equals(String &value)
{
    if (this->length != value.length)
    {
        return false;
    }

    int i = 0;
    for (i = 0; i < this->length; i++)
    {
        if (this->CharArray()[i] != value.CharArray()[i])
        {
            return 0;
        }
    }

    return 1;
}

//
//関数
//  説明:
//      文字列を置き換えます
//
String String::Replace(String &oldValue, String &newValue)
{
    List<char> chars(this->length + 1);

    //文字列str内でoldValueが存在する先頭位置
    int startIndex = 0;

    //文字列strの読み込み位置
    int index = 0;

    for (;;)
    {
        //oldValueを検索
        startIndex = this->IndexOf(oldValue, startIndex, this->length - startIndex);

        //存在しないとき
        if (startIndex < 0)
        {
            //strの最後まで読み込む
            for (; index < this->length; index++)
            {
                chars.Add(this->CharArray()[index]);
            }
            break;
        }
        else
        {
            //検索位置までstrを読み込む
            for (; index < startIndex; index++)
            {
                chars.Add(this->CharArray()[index]);
            }

            //新しい文字列に置き換える
            int i = 0;
            for (i = 0; i < newValue.length; i++)
            {
                chars.Add(newValue.CharArray()[i]);
            }

            //古い文字列分str読み取り位置をスキップする
            index += oldValue.length;
            startIndex = index;
        }
    }

    //ヌル文字を追加
    chars.Add('\0');

    String newString(chars.Array());
    return newString;
}


//
//関数
//  説明:
//      指定された文字列を分割します
//      分割したものはListに格納されます
//
List<String> String::Split(List<String> separator)
{
    List<String> newList(0);

    //文字列str内でoldValueが存在する先頭位置
    int startIndex = 0;

    //文字列strの読み込み位置
    int index = 0;

    while (index < this->length)
    {
        int skip = 0;

        //区切り文字列を検索
        int firstDetected = 0;
        int nextStartIndex = startIndex;
        int i = 0;
        for (i = 0; i < separator.Count(); i++)
        {
            int tempIndex = this->IndexOf(separator[i], startIndex, this->length - startIndex);

            if (!firstDetected && tempIndex >= 0)
            {
                nextStartIndex = tempIndex;
                skip = separator[i].length;
                firstDetected = 1;
            }

            //各区切り文字列の検索位置のうち最小のものを選択
            if (firstDetected && tempIndex >= 0 && tempIndex < nextStartIndex)
            {
                nextStartIndex = tempIndex;
                skip = separator[i].length;
            }
        }

        //すべての区切り文字列で見つけられなかったとき
        if (!firstDetected)
        {
            nextStartIndex = -1;
        }

        //検索位置更新
        startIndex = nextStartIndex;

        //存在しないとき
        if (startIndex < 0)
        {
            //文字リスト作成
            List<char> chars(this->length - index + 1);

            //strの最後まで読み込む
            for (; index < this->length; index++)
            {
                chars.Add(this->CharArray()[index]);
            }
            //ヌル文字を追加
            chars.Add('\0');

            String newString(chars.Array());

            newList.Add(newString);
            break;
        }
        else
        {
            //文字リスト作成
            List<char> chars(startIndex - index + 1);

            //検索位置までstrを読み込む
            for (; index < startIndex; index++)
            {
                chars.Add(this->CharArray()[index]);
            }

            //ヌル文字を追加
            chars.Add('\0');

            String newString(chars.Array());
            newList.Add(newString);

            //区切り文字列分str読み取り位置をスキップする
            index += skip;
            startIndex = index;

            //文字列の最後尾に区切り文字列があった場合,空文字列をリストに追加する
            if (index == this->length)
            {
                String newString("");
                newList.Add(newString);
            }
        }
    }

    return newList;
}

String String::Substring(int startIndex, int length)
{
    if (startIndex < 0 || startIndex + length > this->length)
    {
        throw std::out_of_range("[String.Substring]>> ArgumentOutOfRange");
    }

    String sub = "";

    //NULL文字削除
    sub.chars.Clear();

    for (int i = 0; i < length; i++)
    {
        sub.chars.Add((*this)[startIndex + i]);
    }

    sub.chars.Add('\0');

    return sub;
}
//
//関数
//  説明:
//      小文字を大文字に変換します
//
String String::ToUpper()
{
    List<char> chars(this->length + 1);

    int i = 0;
    for (i = 0; i < this->length; i++)
    {
        chars.Add(ToUpperChar(this->CharArray()[i]));
    }

    //ヌル文字を追加
    chars.Add('\0');

    String newString(chars.Array());
    return newString;
}

//
//関数
//  説明:
//      大文字を小文字に変換します
//
String String::ToLower()
{
    List<char> chars(this->length + 1);

    int i = 0;
    for (i = 0; i < this->length; i++)
    {
        chars.Add(ToLowerChar(this->CharArray()[i]));
    }

    //ヌル文字を追加
    chars.Add('\0');

    String newString(chars.Array());
    return newString;
}

//
//関数
//  説明:
//      2つのstringを結合します
//
String String::Concat(String &str0, String &str1)
{
    List<char> chars(str0.length + str1.length + 1);

    int i = 0;
    for (i = 0; i < str0.length; i++)
    {
        chars.Add(str0.CharArray()[i]);
    }

    for (i = 0; i < str1.length; i++)
    {
        chars.Add(str1.CharArray()[i]);
    }

    //ヌル文字を追加
    chars.Add('\0');

    String newString(chars.Array());
    return newString;
}

//
//関数
//  説明:
//      キーボードから入力された文字列からstringを作成します
//
String String::GetLine()
{
    return GetLine(stdin);
}

//
//関数
//  説明:
//      指定されたファイルから一行読み込みます.
//
String String::GetLine(FILE *fp)
{
    List<char> chars;

    int c;

    //入力文字列読み込み
    for (; (c = fgetc(fp)) != EOF;)
    {
        if (c == '\n')
        {
            break;
        }

        chars.Add(c);
    }

    //ヌル文字を追加
    chars.Add('\0');

    String str(chars.Array());
    return str;
}

String String::Copy(String &str)
{
    List<char> chars(str.length + 1);

    int i = 0;
    for (i = 0; i < str.length; i++)
    {
        chars.Add(str.CharArray()[i]);
    }

    //ヌル文字を追加
    chars.Add('\0');

    String newString(chars.Array());

    return newString;
}
//---文字判定に関する関数--------------------------------------------------

//
//関数:
//	説明:
//		指定した文字が英字かどうか判断します
//
//	引数:
//		c: 調べたい文字
//
//	戻り値:
//		1: 英字である
//		0: 英字ではない
//
bool String::IsAlpha(char c)
{
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
    {
        return true;
    }
    else
    {
        return false;
    }
}

//
//関数:
//	説明:
//		指定した文字が数字かどうか判断します
//
//	引数:
//		c: 調べたい文字
//
//	戻り値:
//		1: 数字である
//		0: 数字ではない
//
bool String::IsDigit(char c)
{
    if (c >= '0' && c <= '9')
    {
        return true;
    }
    else
    {
        return false;
    }
}

//
//関数:
//	説明:
//		指定した文字が小文字かどうか判断します
//
//	引数:
//		c: 調べたい文字
//
//	戻り値:
//		1: 小文字である
//		0: 小文字ではない
//
bool String::IsLower(char c)
{
    if (c >= 'a' && c <= 'z')
    {
        return true;
    }
    else
    {
        return false;
    }
}

//
//関数:
//	説明:
//		小文字を大文字にします
//
//	引数:
//		c: 大文字にしたい文字
//
//	戻り値:
//		大文字の文字コード
//
char String::ToUpperChar(char c)
{
    if (IsAlpha(c))
    {
        if (IsLower(c))
        {
            return c - 0x20;
        }
    }

    return c;
}

//
//関数:
//	説明:
//		大文字を小文字にします
//
//	引数:
//		c: 小文字にしたい文字
//
//	戻り値:
//		小文字の文字コード
//
char String::ToLowerChar(char c)
{
    if (IsAlpha(c))
    {
        if (!IsLower(c))
        {
            return c + 0x20;
        }
    }

    return c;
}

//
//関数:
//	説明:
//		文字列の文字数を数えます
//
//	引数:
//		*str: 数えたい文字列
//
//	戻り値:
//		int : 文字数 (ただしNULL文字は含まない)
//
int String::StringLength(char *str)
{
    int i;

    for (i = 0;; i++)
    {
        if (str[i] == '\0')
        {
            break;
        }
    }

    return i;
}
#endif
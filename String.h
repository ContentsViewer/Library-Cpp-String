
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
    char* operator&() { return GetChars(); }
    bool operator==(String rStr) { return Equals(rStr); }
    char operator[](int index) { char out = chars[index]; return out; }

    //�����񌋍�
    String operator+(String &rStr) { return Concat(*this, rStr); }
    String operator+(char *rChars) { return Concat(*this, String(rChars)); }
    String& operator+=(String &rStr);
    String& operator+=(char *rChar);
    void CopyFrom(const String& from);

    //�֐�
    //  ����:
    //      string�����̕�����\char�z��\�̐擪�A�h���X��Ԃ��܂�
    char* GetChars() { return &(this->chars); }

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
    String Concat(String &str0, String &str1);
    static  String GetLine();
    static String GetLine(FILE *fp);
    static String Copy(String &str);

    //---��������Ɋւ���֐�--------------------------------------------------
    static bool IsAlpha(char c);
    static bool IsDigit(char c);
    static bool IsLower(char c);
    static char ToUpperChar(char c);
    static char ToLowerChar(char c);
    static  int StringLength(char *str);
};

//�f�t�H���g�R���X�g���N�^
String::String()
{
    length = 0;
    this->chars.Add('\0');
}

//�v�f�w��R���X�g���N�^
String::String(char *chars)
{
    length = StringLength(chars);

    for (int i = 0; i < length; i++)
    {
        this->chars.Add(chars[i]);
    }
    this->chars.Add('\0');
}

//�f�X�g���N�^
//memo:
//  List�̃f�X�g���N�^���Ă΂��
String:: ~String()
{
    //printf("Call String Destructor\n");
}

//�R�s�[�R���X�g���N�^
//memo:
//  List�̃R�s�[�R���X�g���N�^�͌Ă΂�Ȃ�
//
String::String(const String &str)
{
    //printf("Call String CopyConstructor\n");
    CopyFrom(str);
}

//���[�u�R���X�g���N�^
String::String(String&& rStr)
{
    length = rStr.length;
    chars = std::move(rStr.chars);
}

//������Z�q
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
    //NULL�����폜
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
//�֐�
//  ����:
//      �w�肳�ꂽ��������������܂�
//      ������������̐擪�ʒu��Ԃ��܂�
//      �������Ȃ������Ƃ���-1��Ԃ��܂�
//
int String::IndexOf(String &pattern, int startIndex, int count)
{
    //�e�L�X�g�ƃp�^�[���̕s��v�����������Ƃ��ɂǂꂾ�����炷�������z��
    int skip[256];

    //���ڂ��Ă���e�L�X�g�̈ʒu
    int i = 0;

    //���ڂ��Ă���p�^�[���̈ʒu
    int j = 0;

    //�\skip���쐬����
    for (i = 0; i < 256; i++)
    {
        skip[i] = pattern.length;
    }
    for (i = 0; i < pattern.length - 1; i++)
    {
        skip[(unsigned char)(pattern.GetChars()[i])] = pattern.length - i - 1;
    }

    i = startIndex + pattern.length - 1;

    //�e�L�X�g�̍Ō���ɍs��������܂ŌJ��Ԃ�
    while (i < startIndex + count)
    {
        j = pattern.length - 1;

        //�e�L�X�g�ƃp�^�[������v����ԌJ��Ԃ�
        while (this->GetChars()[i] == pattern.GetChars()[j])
        {
            if (j == 0)
            {
                return i;
            }

            i--;
            j--;
        }

        int a = skip[(unsigned char)(this->GetChars()[i])];
        int b = pattern.length - j;

        i = i + (a > b ? a : b);
    }

    return -1;
}

//
//�֐�
//	����:
//		�w�肵�����������񂪂��̕�������ɑ��݂��邩�ǂ���
//
//	�Ԃ�l:
//		1: �܂܂��
//		0: �܂܂�Ȃ�
bool String::Contains(String &value)
{
    if (IndexOf(value, 0, this->length) < 0)
    {
        return false;
    }
    return true;
}

//
//�֐�
//  ����:
//      string���r���܂�
//
//  �Ԃ�l:
//      1: ��v
//      0: �s��v
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
        if (this->GetChars()[i] != value.GetChars()[i])
        {
            return 0;
        }
    }

    return 1;
}

//
//�֐�
//  ����:
//      �������u�������܂�
//
String String::Replace(String &oldValue, String &newValue)
{
    List<char> chars(this->length + 1);

    //������str����oldValue�����݂���擪�ʒu
    int startIndex = 0;

    //������str�̓ǂݍ��݈ʒu
    int index = 0;

    for (;;)
    {
        //oldValue������
        startIndex = this->IndexOf(oldValue, startIndex, this->length - startIndex);

        //���݂��Ȃ��Ƃ�
        if (startIndex < 0)
        {
            //str�̍Ō�܂œǂݍ���
            for (; index < this->length; index++)
            {
                chars.Add(this->GetChars()[index]);
            }
            break;
        }
        else
        {
            //�����ʒu�܂�str��ǂݍ���
            for (; index < startIndex; index++)
            {
                chars.Add(this->GetChars()[index]);
            }

            //�V����������ɒu��������
            int i = 0;
            for (i = 0; i < newValue.length; i++)
            {
                chars.Add(newValue.GetChars()[i]);
            }

            //�Â�������str�ǂݎ��ʒu���X�L�b�v����
            index += oldValue.length;
            startIndex = index;
        }
    }

    //�k��������ǉ�
    chars.Add('\0');

    String newString(&chars);
    return newString;
}


//
//�֐�
//  ����:
//      �w�肳�ꂽ������𕪊����܂�
//      �����������̂�List�Ɋi�[����܂�
//
List<String> String::Split(List<String> separator)
{
    List<String> newList(0);

    //������str����oldValue�����݂���擪�ʒu
    int startIndex = 0;

    //������str�̓ǂݍ��݈ʒu
    int index = 0;

    while (index < this->length)
    {
        int skip = 0;

        //��؂蕶���������
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

            //�e��؂蕶����̌����ʒu�̂����ŏ��̂��̂�I��
            if (firstDetected && tempIndex >= 0 && tempIndex < nextStartIndex)
            {
                nextStartIndex = tempIndex;
                skip = separator[i].length;
            }
        }

        //���ׂĂ̋�؂蕶����Ō������Ȃ������Ƃ�
        if (!firstDetected)
        {
            nextStartIndex = -1;
        }

        //�����ʒu�X�V
        startIndex = nextStartIndex;

        //���݂��Ȃ��Ƃ�
        if (startIndex < 0)
        {
            //�������X�g�쐬
            List<char> chars(this->length - index + 1);

            //str�̍Ō�܂œǂݍ���
            for (; index < this->length; index++)
            {
                chars.Add(this->GetChars()[index]);
            }
            //�k��������ǉ�
            chars.Add('\0');

            String newString(&chars);

            newList.Add(newString);
            break;
        }
        else
        {
            //�������X�g�쐬
            List<char> chars(startIndex - index + 1);

            //�����ʒu�܂�str��ǂݍ���
            for (; index < startIndex; index++)
            {
                chars.Add(this->GetChars()[index]);
            }

            //�k��������ǉ�
            chars.Add('\0');

            String newString(&chars);
            newList.Add(newString);

            //��؂蕶����str�ǂݎ��ʒu���X�L�b�v����
            index += skip;
            startIndex = index;

            //������̍Ō���ɋ�؂蕶���񂪂������ꍇ,�󕶎�������X�g�ɒǉ�����
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

    //NULL�����폜
    sub.chars.Clear();

    for (int i = 0; i < length; i++)
    {
        sub.chars.Add((*this)[startIndex + i]);
    }

    sub.chars.Add('\0');

    return sub;
}
//
//�֐�
//  ����:
//      ��������啶���ɕϊ����܂�
//
String String::ToUpper()
{
    List<char> chars(this->length + 1);

    int i = 0;
    for (i = 0; i < this->length; i++)
    {
        chars.Add(ToUpperChar(this->GetChars()[i]));
    }

    //�k��������ǉ�
    chars.Add('\0');

    String newString(&chars);
    return newString;
}

//
//�֐�
//  ����:
//      �啶�����������ɕϊ����܂�
//
String String::ToLower()
{
    List<char> chars(this->length + 1);

    int i = 0;
    for (i = 0; i < this->length; i++)
    {
        chars.Add(ToLowerChar(this->GetChars()[i]));
    }

    //�k��������ǉ�
    chars.Add('\0');

    String newString(&chars);
    return newString;
}

//
//�֐�
//  ����:
//      2��string���������܂�
//
String String::Concat(String &str0, String &str1)
{
    List<char> chars(str0.length + str1.length + 1);

    int i = 0;
    for (i = 0; i < str0.length; i++)
    {
        chars.Add(str0.GetChars()[i]);
    }

    for (i = 0; i < str1.length; i++)
    {
        chars.Add(str1.GetChars()[i]);
    }

    //�k��������ǉ�
    chars.Add('\0');

    String newString(&chars);
    return newString;
}

//
//�֐�
//  ����:
//      �L�[�{�[�h������͂��ꂽ�����񂩂�string���쐬���܂�
//
String String::GetLine()
{
    return GetLine(stdin);
}

//
//�֐�
//  ����:
//      �w�肳�ꂽ�t�@�C�������s�ǂݍ��݂܂�.
//
String String::GetLine(FILE *fp)
{
    List<char> chars;

    int c;

    //���͕�����ǂݍ���
    for (; (c = fgetc(fp)) != EOF;)
    {
        if (c == '\n')
        {
            break;
        }

        chars.Add(c);
    }

    //�k��������ǉ�
    chars.Add('\0');

    String str(&chars);
    return str;
}

String String::Copy(String &str)
{
    List<char> chars(str.length + 1);

    int i = 0;
    for (i = 0; i < str.length; i++)
    {
        chars.Add(str.GetChars()[i]);
    }

    //�k��������ǉ�
    chars.Add('\0');

    String newString(&chars);

    return newString;
}
//---��������Ɋւ���֐�--------------------------------------------------

//
//�֐�:
//	����:
//		�w�肵���������p�����ǂ������f���܂�
//
//	����:
//		c: ���ׂ�������
//
//	�߂�l:
//		1: �p���ł���
//		0: �p���ł͂Ȃ�
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
//�֐�:
//	����:
//		�w�肵���������������ǂ������f���܂�
//
//	����:
//		c: ���ׂ�������
//
//	�߂�l:
//		1: �����ł���
//		0: �����ł͂Ȃ�
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
//�֐�:
//	����:
//		�w�肵�����������������ǂ������f���܂�
//
//	����:
//		c: ���ׂ�������
//
//	�߂�l:
//		1: �������ł���
//		0: �������ł͂Ȃ�
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
//�֐�:
//	����:
//		��������啶���ɂ��܂�
//
//	����:
//		c: �啶���ɂ���������
//
//	�߂�l:
//		�啶���̕����R�[�h
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
//�֐�:
//	����:
//		�啶�����������ɂ��܂�
//
//	����:
//		c: �������ɂ���������
//
//	�߂�l:
//		�������̕����R�[�h
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
//�֐�:
//	����:
//		������̕������𐔂��܂�
//
//	����:
//		*str: ��������������
//
//	�߂�l:
//		int : ������ (������NULL�����͊܂܂Ȃ�)
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
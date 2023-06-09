#ifndef GAMECONTROLLER
#define GAMECONTROLLER


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SFML/Graphics.hpp>


# if true

//���������� ������������ ���� ��� ������� �� ���������� SFML, ����� ����� ���� ������������ ��� ������ ��� ������ �������� ������������ ����
using sf::Texture;
using sf::Image;
using sf::Sprite;

//��� �������� � ���������� ���������� � ������� Object2D
struct Animation
{
    Texture textures[100];
    Sprite  sprites[100];
    char    nameAnim[25];
    int     countCadres;
    int     stateCadre;
};

//��������� ������
class Object2D
{
private:

    //��� �������� �������� ������� Object2D
    Animation animations[15];

    //���������� �������� � ������� Object2D
    int       countAnimations;

    // ������������ ���������� ������� ���������� ��������
    clock_t lastTime;

    //�������� �������� ������ ��������
    int nowNumberAnim;

    //��������� ������� ����� �� ������ ����
    void GetFormat(const char str[], char format[]);

    //�������� ������� �� ������
    void DeleteFormatInStr(const char str[], char newStr[]);

    //���������� ������ �������� �� ������
    int  GetNumberAnim(const char word[]);

    //����������, ������� ���������, ������ �� ������������ ������� ���� �������
    bool colliderVisible;

public:
    Object2D();

    //��� �������� ������� � �������� �������
    sf::Vector2f position;
    sf::Vector2f scale;

    //��� �������� ������ � ������ �������
    float weight;
    float height;

    // ������������ ������� ������� �� ������ ���������� ���� � �����
    void SetDefaultSprite(const char path[]);

    //���������� ����� �������� � ������ Object2D
    void AddAnim(const char path[], const char nameState[], int countCadres, const sf::IntRect collider = {0, 0, 0, 0});

    //��� ������������ �������� � ��������� ������ � �������� ���������� ����� �������. ����� �������� ������ �� ����, � ������� ������������ ��������, � ���������� ������� ���� ��������.
    int  Animate(const char name[], float msBetweenCadres, sf::RenderWindow &window);

    //��� ����������� ������� �� ��������� �����������
    void Move(float x, float y);

    //��� ��������� �������� ������� �� ��������� ���������
    void SetScale(float x, float y);

    //��������� ������� �� ��������� �����������
    void SetPosition(float x, float y);
};

#endif

#if true

Object2D::Object2D(): countAnimations(1)
{

    //������� ������� �� ���� X � Y 
    scale = { 1.0, 1.0 };


    //��� ��������� ����� �������� ��� ������� �������� ������� animations
    for (int i = 0; i < 10; i++)
    {
        strcpy(animations[i].nameAnim, "");
    }

    //��� ������������ ���������� ������� ���������� ��������.
    lastTime = 0;

    //��� �������� �������� ������ ��������
    nowNumberAnim = 0;
}


//����� ������� � ������. ����� �������� ��� ������� ����� ���� � ������, ����� ��������� ������ �����
void Object2D::GetFormat(const char str[], char format[])
{

    //���������� ����� ������ 
    int sizeStr = strlen(str);

    //��� ������������ ���������� �������, ������������ �� ������ �������
    bool isFormat = false;

    
    for (int i = 0, j = 0; i < sizeStr; i++)
    {
        //��������, �������� �� ������� ������ ��������
        if (str[i] == '.') isFormat = true;

        //���� isformat = true, ������ �������� ������� � ����� ������ ���������� ������� ������� � ������ format
        if (isFormat)
        {
            format[j] = str[i];

            //��� �������� �� ��������� ������� � ������� format
            j++;
        }
    }
}


//�������� ������� ����� �� ������, ������� ��� ������� �� ������� ������� � ������ 
void Object2D::DeleteFormatInStr(const char str[], char newStr[])
{

    //���������� ����� ������
    int sizeStr = strlen(str);
    for (int i = 0; i < sizeStr; i++)
    {
        //��������, �������� �� ������� ������ ��������, ���� ��� ���, �� �����������
        if (str[i] == '.') break;

        newStr[i] = str[i];
    }
}


//�����  �������� � �������� ��������� word � ������� animations � ���������� �� ������.
int Object2D::GetNumberAnim(const char word[])
{

    //���������� ��� �������� ������� �������� � ������� animations.
    int numberAnimation = -1;
    for (int i = 0; i < 10; i++)
    {
        //������� ���������� ������ word � ������ ��������, ���� ������ ���������, ����������� ���� ���� ������ �������
        if (strcmp(word, animations[i].nameAnim) == 0)
        {
            //���� ���������� �������, �������� ���������� ��������������� � ������� �������� i, ��� ������������� ������� �������� � ������� 
            numberAnimation = i;

            //��� ���������� ����� ����� ���������� ����������
            break;
        }
    }

    return numberAnimation;
}

//�������� ������� � ��������� �������� ��� ������� ����� ��������. ���������� ���������� � ��������� ��������, ������� �� ��������, ���������� ������ � ������� ����.
void Object2D::AddAnim(const char path[], const char nameState[], int countCadres, const sf::IntRect collider)
{

    //��� �������� ������� � ��������� �������� ��� ������� ����� ��������
    for (int i = 0; i < countCadres; i++)
    {
        //��� �������� ������� ���� � ����� ������� ����� ��������
        char fullPath[50] = "";

        //��� �������� �������  ������ ��������
        char format[10] = "";

        //��� �������� ���� � ������ �������� ��� �������
        char pathNoFormat[50] = "";

        //���������� ������� ����� �� ���� � ���������� ��� � ������ format
        GetFormat(path, format);

        //�������� ������� ����� �� ���� � ���������� ���������� � ������ pathNoFormat
        DeleteFormatInStr(path, pathNoFormat);

        //��� �������� ������� ���� � ����� ��� �������
        strcat(fullPath, pathNoFormat);

        //���������� ����� ������ fullPath
        int sizePath = strlen(fullPath);

        //��� �������� ������ ����� �������� � ���� ������
        char num[4] = "";

        //��� �������������� ������ ����� i � ������ � ���������� ���������� � ������ num.
        itoa(i, num, 10);

        //��� �������� ������� ���� � ����� � ������� �����
        strcat(fullPath, num);

        //��� �������� ������� ���� � ����� � ��������
        strcat(fullPath, format);

        //��� �������� �������� �� �����, ���������� � fullPath, � ���������� � ������� 
        animations[countAnimations].textures[i].loadFromFile(fullPath);


        //��������, ������ �� ������ � ������ ����������
        if (collider.width != 0 && collider.height != 0)
        {
            //���� ��������� ��� �����, �� ��� ������� ��������������� ������������� ������� ��������, ������������ ����������� 
            animations[countAnimations].sprites[i].setTextureRect(collider);

            //��������������� �������� �������, ������� ���� ��������� �����
            animations[countAnimations].sprites[i].setTexture(animations[countAnimations].textures[i]);
        }
        else
        {
            //��������������� �������� �������, ������� ���� ��������� �����
            animations[countAnimations].sprites[i].setTexture(animations[countAnimations].textures[i]);
        }
        
    }

    //����� ������ ��� ��������� ��������
    strcat(animations[countAnimations].nameAnim, nameState);

    //��� ��������� ���������� ������ �������� ��� �������� ���������
    animations[countAnimations].countCadres = countCadres;

    //��� ��������� �������� ����� �������� ��� �������� ��������� ������ 0
    animations[countAnimations].stateCadre  = 0;

    //���������� ��������, ������� ��������� �� ��������� ������ � �������
    countAnimations++;
}

//��� ������������ ��������, ������� ������������� ���������� �����. ���������� ������ �������� � ������������ � �������� �������� � ����������� �������� ����� �� ����
//����������� ������ �������� ����� �������� ��� ����������� �������������
int Object2D::Animate(const char name[], float msBetweenCadres, sf::RenderWindow &window)
{

    //��������� ������ ��������
    int nowNumberAnim = GetNumberAnim(name);

    // ���� ����� �������� ����� -1, �� ������������ -1, ��� ��������� �� ������ ��� ��������� �������� 
    if (nowNumberAnim == -1) return -1;

    //��������� �������� ������� � ����
    clock_t secondsInGame = clock();

    //��������� �������� ����� �������� 
    int stateCadre = animations[nowNumberAnim].stateCadre;

    //���� ������� ���� = ������ ���-�� ������ �������� => �������� �������� �����
    if (stateCadre == animations[nowNumberAnim].countCadres)
    {
        //��������� �������� ����� �������� � 0 ��� ����, ����� �������� ������ ������������� �������
        animations[nowNumberAnim].stateCadre = 0;

        //���������� �������� ��� ����������� ���������� ����� ��������
        stateCadre = 0;
    }

    //��������� ������� ������� �������� ������ ������� ������� Object2D
    animations[nowNumberAnim].sprites[stateCadre].setPosition(position);

    //��������� �������� ������� �������� ������ �������� ������� Object2D
    animations[nowNumberAnim].sprites[stateCadre].setScale(scale);

    if (secondsInGame - lastTime >= msBetweenCadres)
    {
        //���������� ������� ���������� ���������� ����� ��������
        lastTime = clock();

        //������� � ���������� ����� ��������
        animations[nowNumberAnim].stateCadre++;
    }

    //��� ��������� �������� ����� �������� �� ����
    window.draw(animations[nowNumberAnim].sprites[stateCadre]);

    return stateCadre;
}


//����������� ������� � ����� ���������� � ���������� ������� ������� �������� ����� �������� ��� ����������� � ����� ����������
void Object2D::Move(float x, float y)
{
    //��������� ������� ������� �� ��������� ��������
    position = { x, y };

    //��������� ������� ������� �������� ����� �������� 
    animations[nowNumberAnim].sprites[animations[nowNumberAnim].stateCadre].setPosition(position);
}


//��������� ����� �������� �������� ������� � ���������� �������� ������� �������� ����� �������� ��� ����������� � ����� ���������
void Object2D::SetScale(float x, float y)
{
    //��������� �������� ������� �� ��������� ��������
    scale = { x, y };

    //��������� �������� ������� �������� ����� ��������
    animations[nowNumberAnim].sprites[animations[nowNumberAnim].stateCadre].setScale(scale);
}

//��������� ����� ��������� ������� ������ � ���������� ������� ������� �������� ����� �������� ��� ����������� � ����� ���������
void Object2D::SetPosition(float x, float y)
{
    //��������� ������� ������� �� ��������� ��������
    position = { x, y };

    //��������� ������� ������� �������� ����� ��������
    animations[nowNumberAnim].sprites[animations[nowNumberAnim].stateCadre].setPosition(position);
}


//��������� ������ �������� ������� � ������������ ������, �������� �������� �� ���������� �����, ���������� �������� �� �������� � ��������� ����� �������� ��� "default"
void Object2D::SetDefaultSprite(const char path[])
{
    //�������� ��������� �� ���������� ����� � ���������� �� � ������ �������� ������� �������� ������ ������ ��������
    animations[0].textures[0].loadFromFile(path);

    //��������� �������� ������� ������� (������ ����� ��������� �� ������� �������� ������� "texture")
    animations[0].sprites[0].setTexture(animations[0].textures[0]);

    //��������� ����� �������� ��� "default"
    strcat(animations[0].nameAnim, "default");

    //���������, ��� ������ �������� ������� ������ �� ������ �����
    animations[0].countCadres = 1;
    
    //���������, ��� ��������� ����� ������������ ������ ���� ��������
    animations[0].stateCadre = 0;
}

#endif 

#endif

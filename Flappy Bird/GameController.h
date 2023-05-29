#ifndef GAMECONTROLLER
#define GAMECONTROLLER


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


# if true

//добавление пространство имен для классов из библиотеки SFML, чтобы можно было использовать эти классы без явного указания пространства имен
using sf::Texture;
using sf::Image;
using sf::Sprite;

//для хранения и управления анимациями в объекте Object2D
struct Animation
{
    Texture textures[100];
    Sprite  sprites[100];
    char    nameAnim[25];
    int     countCadres;
    int     stateCadre;
};

//двумерный объект
class Object2D
{
private:

    //для хранения анимаций объекта Object2D
    Animation animations[15];

    //количество анимаций в объекте Object2D
    int       countAnimations;

    // отслеживание последнего времени обновления анимации
    clock_t lastTime;

    //хранение текущего номера анимации
    int nowNumberAnim;

    //получение формата файла из строки пути
    void GetFormat(const char str[], char format[]);

    //удаление формата из строки
    void DeleteFormatInStr(const char str[], char newStr[]);

    //извлечение номера анимации из строки
    int  GetNumberAnim(const char word[]);

    //переменная, которая указывает, должно ли столкновение объекта быть видимым
    bool colliderVisible;

public:
    Object2D();

    //для хранения позиции и масштаба объекта
    sf::Vector2f position;
    sf::Vector2f scale;

    //для хранения ширины и высота объекта
    float weight;
    float height;

    // устанавление спрайта объекта на основе указанного пути к файлу
    void SetDefaultSprite(const char path[]);

    //добавление новой анимации в объект Object2D
    void AddAnim(const char path[], const char nameState[], int countCadres, const sf::IntRect collider = {0, 0, 0, 0});

    //для проигрывания анимации с указанным именем с заданным интервалом между кадрами. Метод прнимает ссылку на окно, в котором отображается анимация, и возвращает текущий кадр анимации.
    int  Animate(const char name[], float msBetweenCadres, sf::RenderWindow &window);

    //для перемещения объекта по указанным координатам
    void Move(float x, float y);

    //для установки масштаба объекта по указанным значениям
    void SetScale(float x, float y);

    //установка объекта по указанным координатам
    void SetPosition(float x, float y);
};

#endif

#if true

Object2D::Object2D(): countAnimations(1)
{

    //масштаб объекта по осям X и Y 
    scale = { 1.0, 1.0 };


    //для обнуления имени анимации для каждого элемента массива animations
    for (int i = 0; i < 10; i++)
    {
        strcpy(animations[i].nameAnim, "");
    }

    //для отслеживания последнего времени обновления анимации.
    lastTime = 0;

    //для указания текущего номера анимации
    nowNumberAnim = 0;
}


//поиск символа в строке. Метод копирует все символы после него в массив, чтобы сохранить формат файла
void Object2D::GetFormat(const char str[], char format[])
{

    //вычисление длины строки 
    int sizeStr = strlen(str);

    //для отслеживания нахождения символа, указывающего на начало формата
    bool isFormat = false;

    
    for (int i = 0, j = 0; i < sizeStr; i++)
    {
        //проверка, является ли текущий символ символом
        if (str[i] == '.') isFormat = true;

        //если isformat = true, значит достигли символа и нужно начать записывать символы формата в массив format
        if (isFormat)
        {
            format[j] = str[i];

            //для указания на следующую позицию в массиве format
            j++;
        }
    }
}


//удаление формата файла из строки, копируя все символы до символа формата в массив 
void Object2D::DeleteFormatInStr(const char str[], char newStr[])
{

    //вычисление длины строки
    int sizeStr = strlen(str);
    for (int i = 0; i < sizeStr; i++)
    {
        //проверка, является ли текущий символ символом, если это так, то прерывается
        if (str[i] == '.') break;

        newStr[i] = str[i];
    }
}


//поиск  анимации с заданным названием word в массиве animations и возвращает ее индекс.
int Object2D::GetNumberAnim(const char word[])
{

    //переменная для хранения индекса анимации в массиве animations.
    int numberAnimation = -1;
    for (int i = 0; i < 10; i++)
    {
        //функция сравнивает строку word с именем анимации, если строки совпадают, выполняется блок кода внутри условия
        if (strcmp(word, animations[i].nameAnim) == 0)
        {
            //Если совпадение найдено, значение переменной устанавливается в текущее значение i, что соответствует индексу анимации в массиве 
            numberAnimation = i;

            //для прерывания цикла после нахождения совпадения
            break;
        }
    }

    return numberAnimation;
}

//загрузка текстур и настройка спрайтов для каждого кадра анимации. Сохранение информации о состоянии анимации, включая ее название, количество кадров и текущий кадр.
void Object2D::AddAnim(const char path[], const char nameState[], int countCadres, const sf::IntRect collider)
{

    //для загрузки текстур и настройки спрайтов для каждого кадра анимации
    for (int i = 0; i < countCadres; i++)
    {
        //для хранения полного пути к файлу каждого кадра анимации
        char fullPath[50] = "";

        //для хранения формата  файлов анимации
        char format[10] = "";

        //для хранения пути к файлам анимации без формата
        char pathNoFormat[50] = "";

        //извлечение формата файла из пути и сохранение его в массив format
        GetFormat(path, format);

        //удаление формата файла из пути и сохранение результата в массив pathNoFormat
        DeleteFormatInStr(path, pathNoFormat);

        //для создания полного пути к файлу без формата
        strcat(fullPath, pathNoFormat);

        //Вычисление длины строки fullPath
        int sizePath = strlen(fullPath);

        //для хранения номера кадра анимации в виде строки
        char num[4] = "";

        //для преобразования целого числа i в строку и сохранения результата в массив num.
        itoa(i, num, 10);

        //для создания полного пути к файлу с номером кадра
        strcat(fullPath, num);

        //для создания полного пути к файлу с форматом
        strcat(fullPath, format);

        //для загрузки текстуры из файла, указанного в fullPath, и сохранения в массиве 
        animations[countAnimations].textures[i].loadFromFile(fullPath);


        //проверка, заданы ли ширина и высота коллайдера
        if (collider.width != 0 && collider.height != 0)
        {
            //если коллайдер был задан, то для спрайта устанавливается прямоугольная область текстуры, определенная коллайдером 
            animations[countAnimations].sprites[i].setTextureRect(collider);

            //устанавливается текстура спрайта, которая была загружена ранее
            animations[countAnimations].sprites[i].setTexture(animations[countAnimations].textures[i]);
        }
        else
        {
            //устанавливается текстура спрайта, которая была загружена ранее
            animations[countAnimations].sprites[i].setTexture(animations[countAnimations].textures[i]);
        }
        
    }

    //чтобы задать имя состояния анимации
    strcat(animations[countAnimations].nameAnim, nameState);

    //для установки количества кадров анимации для текущего состояния
    animations[countAnimations].countCadres = countCadres;

    //для утсановки текущего кадра анимации для текущего состояния равным 0
    animations[countAnimations].stateCadre  = 0;

    //увеличения счетчика, который указывает на следующий индекс в массиве
    countAnimations++;
}

//для проигрывании анимации, которая соответствует указанному имени. Обновление кадров анимации в соответствии с заданной частотой и отображение текущего кадра на окне
//возвращение номера текущего кадра анимации для дальнейшего использования
int Object2D::Animate(const char name[], float msBetweenCadres, sf::RenderWindow &window)
{

    //получение номера анимации
    int nowNumberAnim = GetNumberAnim(name);

    // если номер анимации равен -1, то возвращается -1, это указывает на ошибку или отсутсвие анимации 
    if (nowNumberAnim == -1) return -1;

    //получение текущего времени в игре
    clock_t secondsInGame = clock();

    //получение текущего кадра анимации 
    int stateCadre = animations[nowNumberAnim].stateCadre;

    //если текущий кадр = общему кол-ву кадров анимации => анимация достигла конца
    if (stateCadre == animations[nowNumberAnim].countCadres)
    {
        //установка текущего кадра анимации в 0 для того, чтобы анимация начала проигрываться сначала
        animations[nowNumberAnim].stateCadre = 0;

        //обновление значения для отображения начального кадра анимации
        stateCadre = 0;
    }

    //установка позиции спрайта анимации равной позиции объекта Object2D
    animations[nowNumberAnim].sprites[stateCadre].setPosition(position);

    //Установка масштаба спрайта анимации равным масштабу объекта Object2D
    animations[nowNumberAnim].sprites[stateCadre].setScale(scale);

    if (secondsInGame - lastTime >= msBetweenCadres)
    {
        //обновление времени последнего обновления кадра анимации
        lastTime = clock();

        //переход к следующему кадру анимации
        animations[nowNumberAnim].stateCadre++;
    }

    //для отрисовки текущего кадра анимации на окне
    window.draw(animations[nowNumberAnim].sprites[stateCadre]);

    return stateCadre;
}


//перемещение объекта в новые координаты и обновление позиции спрайта текущего кадра анимации для отображения в новых координтах
void Object2D::Move(float x, float y)
{
    //оновление позиции объекта на указанные значения
    position = { x, y };

    //оновление позиции спрайта текущего кадра анимации 
    animations[nowNumberAnim].sprites[animations[nowNumberAnim].stateCadre].setPosition(position);
}


//установка новых значений масштаба объекта и обновление масштаба спрайта текущего кадра анимации для отображения с новым масштабом
void Object2D::SetScale(float x, float y)
{
    //оновление масштаба объекта на указанные значения
    scale = { x, y };

    //оновление масштаба спрайта текущего кадра анимации
    animations[nowNumberAnim].sprites[animations[nowNumberAnim].stateCadre].setScale(scale);
}

//установка новых координат позиции обекта и обновление позиции спрайта текущего кадра анимации для отображения в новом положение
void Object2D::SetPosition(float x, float y)
{
    //оновление позиции объекта на указанные значения
    position = { x, y };

    //оновление позиции спрайта текущего кадра анимации
    animations[nowNumberAnim].sprites[animations[nowNumberAnim].stateCadre].setPosition(position);
}


//установка первой анимации объекта с единственным кадром, загрузка текстуры из указанного файла, связывание текстуры со спрайтом и установка имени анимации как "default"
void Object2D::SetDefaultSprite(const char path[])
{
    //загрузка текустуры из указанного файла и сохранение ее в первый эелемент массива текустур внутри первой анимации
    animations[0].textures[0].loadFromFile(path);

    //Установка текстуры спрайта объекта (спрайт берет текустуру из первого элемента массива "texture")
    animations[0].sprites[0].setTexture(animations[0].textures[0]);

    //установка имени анимации как "default"
    strcat(animations[0].nameAnim, "default");

    //указываем, что данная анимация состоит только из одного кадра
    animations[0].countCadres = 1;
    
    //указываем, что изначльно будет отображаться первый кадр анимации
    animations[0].stateCadre = 0;
}

#endif 

#endif

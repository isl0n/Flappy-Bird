#include <stdio.h>
#include <time.h>
#include "GameController.h"
#include <windows.h>


//информация о колонне(позиция по x,y;масштаб;длина и ширина
class Tower
{
private:
	//переменная texture, используется для хранения текстуры, которая будет применена к спрайту башни.
	Texture texture;

public:
	//функция, которая устанавливает значения по умолчанию для башни. Принимает путь к текстуре в виде массива символов path[], а также значения масштабирования scaleX и scaleY
	void SetDefaultValues(const char path[], float _scaleX, float _scaleY);

	// для отрисовки башни на экране
	Sprite towerDown;
	Sprite towerUp;

	//значения высоты и ширины башни
	float height;
	float weight;

	//значения масштабирования башни по осям X и Y, для изменения размера спрайтов башни
	float scaleX;
	float scaleY;

	//позиция башни на экране
	sf::Vector2f position;

	//переменная, которая указывает, перемещается ли башня в данный момент(true=движется, false=не движется)
	bool moving;
};

// создания окна
const int WEIGHT = 1500;
const int HEIGHT = 1000;

//графическое окно, в котором будет отображаться игра
sf::RenderWindow window(sf::VideoMode(WEIGHT, HEIGHT), "Flappy Bird");

//прототип функции
//физическое перемещение объекта
void PhysicMoving(Object2D* person, float boostDown, float v_up, float dt, float* vy);

//поднятие объекта с заданной вертикальной скоростью
void PhysicKick(float v_up, float* vy);

//генерация случайных чисел в заданном диапазоне
int Rand(int min, int max);

// управление башнями в игре
void ControllTowers(Tower towers[], int count, float distance, int* firstTower, float dt, float vy);

//Проверяет наличие столкновения между объектом
bool CheckCollision(Object2D* person, Tower* tower);

//инициализация игровых объектов, начальные значения для их позиций, движения и индекса первой башни.
void ResetGame(Object2D* bird, Tower towers[], int countTowers, int* firstTower)
{
	//позиция птички(начальное положение)
	bird->SetPosition(50, 350);
	

	
	for (int i = 0; i < countTowers; i++)
	{
		towers[i].position.x = WEIGHT;
		towers[i].position.y = Rand(HEIGHT - (int)towers[i].height, HEIGHT - 120);
		towers[i].moving = false;
	}

	// отслеживание индекса первой башни
	*firstTower = 0;
	
}

int main()
{
	//Инициализация генератора случайных чисел с использованием текущего времени, для получения случайных чисел при запуске
	srand(time(NULL));

	//создание переменной для птички
	Object2D bird; 

	//добавление птичке анимиции
	bird.AddAnim("птичка кадры\\bird.png", "fly", 3);

	//установка птицы по осям x y
	bird.SetScale(0.3, 0.3);

	//установка начальной позиции птицы
	bird.SetPosition(50, 350);

	//высота и ширина птички( разрешение * масштаб по осям)
	bird.weight = bird.scale.x * 250;
	bird.height = bird.scale.y * 175;

	//ускорение свободного падения
	float boostFall = 0.002;

	//скорость толчка птички
	float velocityUp = 0.55;

	//переменная текущей скорости птички
	float vy_bird = 0;

	//массив башен
	const int countTowers = 4;
	Tower towers[countTowers];

	int firstTower = 0;
	float vy_tower = 0.45;

	for (int i = 0; i < countTowers; i++)
	{
		//текстура и занчения по осям X Y
		towers[i].SetDefaultValues("Tower.png", 0.7, 0.7);

		//начальная позиция и вертикальное положение
		towers[i].position.x = WEIGHT;
		towers[i].position.y = Rand(HEIGHT - (int)towers[i].height, HEIGHT - 120);
	}

	towers[firstTower].moving = true;

	sf::Clock clock;

	bool isGameOver = false;

	//для обработки событий и отображения игры
	while (window.isOpen())
	{
		//нахождение времени  в миллисекундах
		float dt = clock.getElapsedTime().asMicroseconds();
		dt /= 1000;
		//перезапуск времени
		clock.restart();


		//обработка событий
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
		}

		if (!isGameOver)
		{
			//очистка экрана, белый цвет
			window.clear({ 255, 255, 255 }); 

			//вызываем метод анимации для птички
			bird.Animate("fly", 100, window);

			//вызов функции для движения птички
			PhysicMoving(&bird, boostFall, velocityUp, dt, &vy_bird);

			//для управления башнями
			ControllTowers(towers, countTowers, 500, &firstTower, dt, vy_tower);

			//Проверка столкновения между птицей и первой башней
			isGameOver = CheckCollision(&bird, &towers[firstTower]);

			//отображение содержимого окна
			window.display();
		}
		else
		{
			if (GetAsyncKeyState(VK_SPACE))
			{
				isGameOver = false;
				ResetGame(&bird, towers, countTowers, &firstTower);
			}
		}
	}
}

//Движение птички, функция физики, обновление позиции на экране
void PhysicMoving(Object2D* person, float boostDown, float v_up, float dt, float* vy)
{
	//позиция по x y персонажа
	float x = person->position.x;
	float y = person->position.y;

	//вызов функции для поднятия птички
	PhysicKick(v_up, vy);

	//Увеличивает значение вертикальной скорости. Скорость=ускорение*время.+ускорение свобдного падения *dt(время прошедшее в мс между тактами процесса)
	(*vy) += boostDown * dt;

	//обновление вертикальной позиции птички
	y += (*vy) * dt;

	//Обновление позиции на экране, т.е перемещение птички в новые координаты(x, y)
	person->Move(x, y);
}

//функция для поднятия птички при нажатии на пробел
void PhysicKick(float v_up, float* vy)
{
	//=0 для предотвращения зажатия пробела
	static bool isSpace = 0;

	//проверка состояние пробела, если нажата, то выполняется блок кода
	if (GetAsyncKeyState(VK_SPACE) && isSpace == 0)
	{
		//обновление вертикальной скорости
		(*vy) = 0;

		//добавление вертикальной скорости
		(*vy) -= v_up;

		//предотвращение повторного выполнения, пока пробел не будет отпущен и снова нажат
		isSpace = 1;
	}

	//проверка, не нажат ли пробел в данный момент, если не нажата, то значение переменной уст. на 0, для разрешения нажатия на пробел
	if (!GetAsyncKeyState(VK_SPACE)) isSpace = 0;
}

//метод для создания начальных значений
void Tower::SetDefaultValues(const char path[], float _scaleX, float _scaleY)
{
	//загрузка текстуры
	texture.loadFromFile(path);

	//получение размера текстуры
	sf::Vector2u size = texture.getSize();

	//установление текстуры для спрайтов
	towerUp.setTexture(texture);
	towerDown.setTexture(texture);

	//определение башни по горизонтали и вертикали
	scaleX = _scaleX;
	scaleY = _scaleY;

	//масштаб спрайтов. Отображения по вертикали
	towerUp.setScale(scaleX, -scaleY);
	towerDown.setScale(scaleX, scaleY);

	//вычисление высоты и ширины башни
	height = size.y * scaleY;
	weight = size.x * scaleX;

	//колонна стоит на месте
	moving = false;
}

//генерация случайного числа
int Rand(int min, int max)
{
	return (rand() % (max - min + 1) + min);
}


//управление башнями в игре, обновление позици башен, рис. на экране и создание новых башен
void ControllTowers(Tower towers[], int count, float distance, int* firstTower, float dt, float vy)
{
	//отслеживание последней башни
	static int lastTower = 0;

	for (int i = 0; i < count; i++)
	{
		//проверка moving для башни 
		if (towers[i].moving)
		{
			//если башня движется
			float posX = towers[i].position.x;
			float posY = towers[i].position.y;

			//установка позиции спрайтов башни
			towers[i].towerUp.setPosition(posX, posY - 270);
			towers[i].towerDown.setPosition(posX, posY);

			//смещение башни по горизонтали
			posX -= vy * dt;

			//обновление позиции башни по горизонтали
			towers[i].position.x = posX;

			//отрисовка спрайтов башни в окне
			window.draw(towers[i].towerUp);
			window.draw(towers[i].towerDown);
		}
	}


	//проверка, что расстояние между правым краем окна и правым карем последней башни превышает заданное значение
	if (WEIGHT - (towers[lastTower].position.x + towers[lastTower].weight) >= distance)
	{
		//обновление значения переменной для указания на следующую башню
		if (lastTower == count - 1) lastTower = 0;
		else                        lastTower++;

		//установка позиции по горизонтали для новой башни, для начала движения с правой стороны окна
		towers[lastTower].position.x = WEIGHT;

		//установка "moving" для новой башни, чтобы она начала движение
		towers[lastTower].moving = true;

		//генерация случайного значения для позиции новой башни по вертикали 
		towers[lastTower].position.y = Rand(HEIGHT - (int)towers[lastTower].height, HEIGHT - 120);
	}

	//проверка, что левый  край первой башни достигает или выходит за левую границу экрана
	if (towers[*firstTower].position.x + towers[*firstTower].weight <= 0)
	{
		if (*firstTower == count - 1) (*firstTower) = 0;
		else                          (*firstTower)++;
	}
}

//проверка наличия столкновения между птичкой и башней
bool CheckCollision(Object2D* person, Tower* tower)
{
	//переменная для отслеживания столкновения между птичкой и башней
	bool collision = 0;

	//проверка, что вертикальная позиция объекта меньше 0. Для предотвращения выхода объекта за верхнюю границу экрана
	if (person->position.y < 0)                       person->position.y = 0;

	//проверка, что вертикальная позиция объекта больше высоты окна. Для предотвращения выхода птички за нижнюю границу экрана 
	else if (person->position.y + person->height > HEIGHT) person->position.y = HEIGHT - person->height;

	//проверка, что вертикальная позиция объекта меньше или равна вертикальной позиции верхней части башни или вертикальная позиция объекта больше или равна вертикальной позиции нижней части башни
	//Для проверки пересечения вертикальной позиции объекта с вертикальным пространсвом башни
	if (person->position.y <= tower->towerUp.getPosition().y ||
		person->position.y + person->height >= tower->towerDown.getPosition().y)
	{
		if (person->position.x + person->weight >= tower->position.x &&
			person->position.x <= tower->position.x + tower->weight)
		{
			//указание на столкновение
			collision = 1;
		}
	}

	return collision;
}

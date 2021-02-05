#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "viewr.h"
#include <sstream>
#include "mission.h"
#include <iostream>
#include "map.h"
#include <cmath>


using namespace sf;
class Entity {
public:
	float dx, dy, x, y, speed, moveTimer;//добавили переменную таймер для будущих целей
	int w, h, HP;
	bool life, isMove, onGround, Fire;
	Texture texture;
	Sprite sprite;
	String name;//враги могут быть разные, мы не будем делать другой класс для врага.всего лишь различим врагов по имени и дадим каждому свое действие в update в зависимости от имени
	Entity(Image& image, float X, float Y, int W, int H, String Name) {
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		speed = 0; HP = 100; dx = 0; dy = 0;
		life = true; onGround = false; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}
};

class Player : public Entity{
public:
	enum { left, right, stayleft, stayright, spawn, deadright, deadleft } state;
	int playerScore, Cadr;
    Player(Image& image, float X, float Y,int W,int H,String Name) :Entity(image,X,Y,W,H,Name)
    {
        playerScore = 0; state = spawn; Fire = false;
        if (name == "Player")
            sprite.setTextureRect(IntRect(0, 0, 60, 80));
    }
	   void control()
	   {
	       speed = 0;
           Fire = false;
           if(state == right)
                state = stayright;
            if(state == left)
                state = stayleft;
		   if (Keyboard::isKeyPressed)
            {
                if (Mouse::isButtonPressed(Mouse::Left))
                        Fire = true;
                if ((Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A))))
                {
                    state = left;
                    speed = 0.1;
                }
                if ((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D))))
                {
                    state = right;
                    speed = 0.1;
                }
                if (((Keyboard::isKeyPressed(Keyboard::Up)) || (Keyboard::isKeyPressed(Keyboard::Space))) && (onGround))
                {
                   dy = -0.4;
                   onGround = false;
                   speed = 0.1;
                }
                if (Keyboard::isKeyPressed(Keyboard::K))
                   {
                        HP = 0;
                        if( (state == right) || (state == stayright) )
                            state = deadright;
                        if( (state == left) || (state == stayleft) )
                            state = deadleft;
                   }
		   }
	   }

	   void checkCollisionWithMap(float Dx, float Dy)//ф ция проверки столкновений с картой
	   {
		   for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
			   for (int j = x / 32; j < (x + w) / 32; j++)
				   if (TileMap[i][j] == '0')//если элемент наш тайлик земли то
				   {
					   if (Dy > 0) { y = i * 32 - h;  dy = 0; onGround = true; }
					   if (Dy < 0) { y = i * 32 + 32;  dy = 0; }//столкновение с верхними краями карты
					   if (Dx > 0) { x = j * 32 - w; }//с правым краем карты
					   if (Dx < 0) { x = j * 32 + 32; }// с левым краем карты
				   }
        }

	   void update(float time, float T)
	   {
        control();
        switch (state)
        {
            case right: { dx = speed; sprite = TextureShiftRight(T, sprite, onGround, Fire); break; }
            case left: { dx = -speed; sprite = TextureShiftLeft(T, sprite, onGround, Fire); break; }
            case stayright: { dx = 0; sprite = TextureShiftStayRight(T, sprite, onGround, Fire); break; }
            case stayleft: { dx = 0; sprite = TextureShiftStayLeft(T, sprite, onGround, Fire);  break; }
            case spawn: { dx = 0; sprite.setTextureRect(IntRect(0, 0, 60, 80)); break; }
        }
        x += dx * time;
        checkCollisionWithMap(dx, 0);//обрабатываем столкновение по Х
        y += dy * time;
        checkCollisionWithMap(0, dy);//обрабатываем столкновение по Y
        sprite.setPosition(x + w / 2, y + h / 2); //задаем позицию спрайта в место его центра
        if (HP <= 0)
            life = false;
        if (life) { setPlayerCoordinateForView(getPlayerCoordinateX(), getPlayerCoordinateY()); }
        dy = dy + 0.0015 * time;
	   }

	   float getPlayerCoordinateX() {	//этим методом будем забирать координату Х
		   return x;
        }

	   float getPlayerCoordinateY() {	//этим методом будем забирать координату Y
		   return y;
        }

       Sprite BOOM(float time, Sprite bum)
       {
            float Cadr1 = 0;
            float Cadr2 = 0;
            Cadr1 += 0.00005 * time;
            if ((Cadr1 > 9) && (Cadr2 == 0)) { Cadr1 -= 8; Cadr2++; }
            if ((Cadr1 > 9) && (Cadr2 == 1)) { Cadr1 -= 8; Cadr2++; }
            if ((Cadr1 > 9) && (Cadr2 == 2)) { Cadr1 -= 8; Cadr2++; }
            if ((Cadr1 > 9) && (Cadr2 == 3)) { Cadr1 -= 8; Cadr2++; }
            if ((Cadr1 > 9) && (Cadr2 == 4)) { Cadr1 -= 8; Cadr2++; }
            if ((Cadr1 > 9) && (Cadr2 == 5)) { Cadr1 -= 8; Cadr2++; }
            if ((Cadr1 > 9) && (Cadr2 == 6)) { Cadr1 -= 8; Cadr2++; }
            if ((Cadr1 > 9) && (Cadr2 == 7)) { Cadr1 -= 8; Cadr2++; }
            if ((Cadr1 > 9) && (Cadr2 == 8)) { Cadr1 -= 8; Cadr2++; }
            if ((Cadr1 > 9) && (Cadr2 == 9)) { Cadr1 -= 8; Cadr2++; }
            if ((Cadr1 > 100) && (Cadr2 == 10)) { Cadr1 = 0; Cadr2 = 0; }
            bum.setTextureRect(IntRect(100 * int(Cadr1), 100 * int(Cadr2), 100, 100));
            //bum.setPosition(getPlayerCoordinateX(), getPlayerCoordinateY());
            return bum;
        }

        Sprite TextureShiftRight(float time, Sprite hero, bool onGround, bool Fire)
        {
            if( (onGround == true) && (Fire == false) )
            {
                float Cadr = 0;
                Cadr += 0.000006 * time;
                if (Cadr > 4) Cadr -= 4;
                hero.setTextureRect(IntRect(60 * int(Cadr), 0, 60, 80));
            }
            if( (onGround == false) && (Fire == false) )
                hero.setTextureRect(IntRect(0, 480, 60, 80));
            if( (onGround == true) && (Fire == true) )
            {
                float Cadr = 0;
                Cadr += 0.000006 * time;
                if (Cadr > 4) Cadr -= 4;
                hero.setTextureRect(IntRect(60 * int(Cadr), 80, 60, 80));
            }
            if( (onGround == false) && (Fire == true) )
            {
                float Cadr = 0;
                Cadr += 0.000003 * time;
                if (Cadr > 2) Cadr -= 2;
                hero.setTextureRect(IntRect(60 * int(Cadr), 480, 60, 80));
            }
            return hero;
        }
        Sprite TextureShiftLeft(float time, Sprite hero, bool onGround, bool Fire)
        {
            if( (onGround == true) && (Fire == false) )
            {
                float Cadr = 0;
                Cadr += 0.000006 * time;
                if (Cadr > 4) Cadr -= 4;
                hero.setTextureRect(IntRect(60 * int(Cadr), 160, 60, 80));
            }
            if( (onGround == false) && (Fire == false) )
                hero.setTextureRect(IntRect(0, 560, 60, 80));
            if( (onGround == true) && (Fire == true) )
            {
                float Cadr = 0;
                Cadr += 0.000006 * time;
                if (Cadr > 4) Cadr -= 4;
                hero.setTextureRect(IntRect(60 * int(Cadr), 240, 60, 80));
            }
            if( (onGround == false) && (Fire == true) )
            {
                float Cadr = 0;
                Cadr += 0.000003 * time;
                if (Cadr > 2) Cadr -= 2;
                hero.setTextureRect(IntRect(60 * int(Cadr), 560, 60, 80));
            }
            return hero;
        }
        Sprite TextureShiftStayRight(float time, Sprite hero, bool onGround, bool Fire)
        {
            if( (onGround == true) && (Fire == false) )
                hero.setTextureRect(IntRect(0, 0, 60, 80));
            if( (onGround == true) && (Fire == true) )
            {
                float Cadr = 0;
                Cadr += 0.000003 * time;
                if (Cadr > 2) Cadr -= 2;
                hero.setTextureRect(IntRect(0, 80*int(Cadr), 60, 80));
            }
            if( (onGround == false) && (Fire == false) )
                hero.setTextureRect(IntRect(0, 480, 60, 80));
            if( (onGround == false) && (Fire == true) )
            {
                float Cadr = 0;
                Cadr += 0.000003 * time;
                if (Cadr > 2) Cadr -= 2;
                hero.setTextureRect(IntRect(60*int(Cadr), 480, 60, 80));
            }
            return hero;
        }
        Sprite TextureShiftStayLeft(float time, Sprite hero, bool onGround, bool Fire)
        {
            if( (onGround == true) && (Fire == false) )
                hero.setTextureRect(IntRect(60, 160, 60, 80));
            if( (onGround == true) && (Fire == true) )
            {
                float Cadr = 0;
                Cadr += 0.000003 * time;
                if (Cadr > 2) Cadr -= 2;
                hero.setTextureRect(IntRect(60, 160 + 80*int(Cadr), 60, 80));
            }
            if( (onGround == false) && (Fire == false) )
                hero.setTextureRect(IntRect(0, 560, 60, 80));
            if( (onGround == false) && (Fire == true) )
            {
                float Cadr = 0;
                Cadr += 0.000003 * time;
                if (Cadr > 2) Cadr -= 2;
                hero.setTextureRect(IntRect(60*int(Cadr), 560, 60, 80));
            }
            return hero;
        }

        void Dead (float time, float T)
        {
            switch(state)
            {
                case deadright: { sprite = TextureShiftDeadRight(T, sprite); break; }
                case deadleft: { sprite = TextureShiftDeadLeft(T, sprite); break; }
            }
        }
        Sprite TextureShiftDeadRight(float time, Sprite hero)
        {
            float Cadr = 0;
            Cadr += 0.000003 * time;
            if (Cadr > 2) Cadr -= 2;
            hero.setTextureRect(IntRect(60*int(Cadr), 320, 60, 80));
            return hero;
        }
        Sprite TextureShiftDeadLeft(float time, Sprite hero)
        {
            float Cadr = 0;
            Cadr += 0.000003 * time;
            if (Cadr > 2) Cadr -= 2;
            hero.setTextureRect(IntRect(60*int(Cadr), 400, 60, 80));
            return hero;
        }

};


class Enemy :public Entity {
public:
	Enemy(Image& image, float X, float Y, int W, int H, String Name) :Entity(image, X, Y, W, H, Name) {
		if (name == "Enemy") {
			sprite.setTextureRect(IntRect(0, 0, w, h));
			dx = 0.1;//даем скорость.этот объект всегда двигается
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)//ф ция проверки столкновений с картой
	{
		for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
			for (int j = x / 32; j < (x + w) / 32; j++)
			{
				if (TileMap[i][j] == '0')//если элемент наш тайлик земли, то
				{
					if (Dy > 0) { y = i * 32 - h; }//по Y вниз=>идем в пол(стоим на месте) или падаем. В этот момент надо вытолкнуть персонажа и поставить его на землю, при этом говорим что мы на земле тем самым снова можем прыгать
					if (Dy < 0) { y = i * 32 + 32; }//столкновение с верхними краями карты(может и не пригодиться)
					if (Dx > 0) { x = j * 32 - w; dx = -0.1; sprite.scale(-1, 1); }//с правым краем карты
					if (Dx < 0) { x = j * 32 + 32; dx = 0.1; sprite.scale(-1, 1); }// с левым краем карты
				}
			}
	}

	void update(float time)
	{
		if (name == "Enemy") {//для персонажа с таким именем логика будет такой

			//moveTimer += time;if (moveTimer>3000){ dx *= -1; moveTimer = 0; }//меняет направление примерно каждые 3 сек
			checkCollisionWithMap(dx, 0);//обрабатываем столкновение по Х
			x += dx * time;
			dy = dy + 0.0015 * time;//постоянно притягиваемся к зем
			sprite.setPosition(x + w / 2, y + h / 2); //задаем позицию спрайта в место его центра
			if (HP <= 0) { life = false; }
		}
	}
};


int main()
{
    long long int TimeForBoom = 0;
    long long int TimeForHero = 0;
    long long int TimeForDead = 0;
	RenderWindow window(VideoMode(1200, 675), "My game");// Style::Fullscreen);
	view.reset(sf::FloatRect(0, 0, 1200, 675));//размер "вида" камеры при создании объекта вида камеры. (потом можем менять как хотим) Что то типа инициализации.

	Texture bumtexture;
	bumtexture.loadFromFile("images/BUM.png");

    Image map_image;//объект изображения для карты
	map_image.loadFromFile("images/map.png");//загружаем файл для карты
	Texture map;//текстура карты
	map.loadFromImage(map_image);//заряжаем текстуру картинкой
	Sprite s_map;//создаём спрайт для карты
	s_map.setTexture(map);//заливаем текстуру спрайтом

	Sprite bumsprite;
	bumsprite.setTexture(bumtexture);
	bumsprite.setTextureRect(IntRect(0, 0, 100, 100));
	bumsprite.setPosition(250, 250);

	Music music;
	music.openFromFile("music/music.ogg");
	//music.play();

	Clock clock;
	Clock clockboom;
	clockboom.restart();

	Font font;//шрифт
	font.loadFromFile("Fonk.ttf");//передаем нашему шрифту файл шрифта
	Text text("", font, 40);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
	text.setStyle(sf::Text::Bold );//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый

	Image quest_image;
	quest_image.loadFromFile("images/mission.jpg");
	Texture quest_texture;
	quest_texture.loadFromImage(quest_image);
	Sprite s_quest;
	s_quest.setTexture(quest_texture);
	s_quest.setTextureRect(IntRect(0, 0, 340, 510));  //приведение типов, размеры картинки исходные
	s_quest.setScale(0.6f, 0.6f);//чуть уменьшили картинку, => размер стал меньше


	Image heroImage;
	heroImage.loadFromFile("images/heroes.png");

	Image easyEnemyImage;
	easyEnemyImage.loadFromFile("images/Enamy.png");

	Player p(heroImage, 300, 200, 60, 80, "Player");//объект класса игрока
	Enemy easyEnemy(easyEnemyImage, 400, 300, 180, 180, "Enemy");//объект класса врага

	bool showMissionText = true;

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		TimeForHero += time;
		TimeForBoom += time;

		if (TimeForHero > 1300000) TimeForHero = 0;
		if (TimeForBoom > 1600000) TimeForBoom = 0;
		clock.restart();
		time = time / 800;


		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed)//событие нажатия клавиши
				if ((event.key.code == Keyboard::Tab)) {//если клавиша ТАБ

					switch (showMissionText) {//переключатель, реагирующий на логическую переменную showMissionText

					case true: {
						std::ostringstream task;//строка текста миссии
						task << getTextMission(getCurrentMission(p.getPlayerCoordinateX()));//вызывается функция getTextMission (она возвращает текст миссии), которая принимает в качестве аргумента функцию getCurrentMission(возвращающую номер миссии), а уже эта ф-ция принимает в качестве аргумента функцию p.getplayercoordinateX() (эта ф-ция возвращает Икс координату игрока)
						showMissionText = false;//эта строка позволяет убрать все что мы вывели на экране
						break;//выходим , чтобы не выполнить условие "false" (которое ниже)
					}
					case false: {
						text.setString("");//если не нажата клавиша таб, то весь этот текст пустой
						showMissionText = true;// а эта строка позволяет снова нажать клавишу таб и получить вывод на экран
						break;
					}
					}
				}
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape))
			window.close();

        bumsprite = p.BOOM(TimeForBoom, bumsprite);
        p.update(time, TimeForHero);
        //p.Dead(time, TimeForDead);

		easyEnemy.update(time);//оживляем объект p класса Player с помощью времени sfml, передавая время в качестве параметра функции update. благодаря этому персонаж может двигаться
		window.setView(view);//"оживляем" камеру в окне sfml
		window.clear();

		/////////////////////////////Рисуем карту/////////////////////
		for (int i = 0; i < HEIGHT_MAP; i++)
	    for (int j = 0; j < WIDTH_MAP; j++)
		{
			if (TileMap[i][j] == ' ')  s_map.setTextureRect(IntRect(0, 0, 32, 32)); //если встретили символ пробел, то рисуем 1й квадратик
			if (TileMap[i][j] == 's')  s_map.setTextureRect(IntRect(32, 0, 32, 32));//если встретили символ s, то рисуем 2й квадратик
			if (TileMap[i][j] == '0') s_map.setTextureRect(IntRect(64, 0, 32, 32));//если встретили символ 0, то рисуем 3й квадратик
			s_map.setPosition(j * 32, i * 32);//по сути раскидывает квадратики, превращая в карту. то есть задает каждому из них позицию. если убрать, то вся карта нарисуется в одном квадрате 32*32 и мы увидим один квадрат
            window.draw(s_map);//рисуем квадратики на экран
		}

		std::ostringstream playerHP;    // объявили переменную
		playerHP << p.HP;		//занесли в нее число очков, то есть формируем строку
		text.setString("HP " + playerHP.str());//задаем строку тексту и вызываем сформированную выше строку методом .str()
		text.setPosition(view.getCenter().x + 400, view.getCenter().y - 300);//задаем позицию текста, отступая от центра камеры
		window.draw(text);//рисую этот текст

		if (!showMissionText) {
			text.setPosition(view.getCenter().x + 125, view.getCenter().y - 130);//позиция всего этого текстового блока
			s_quest.setPosition(view.getCenter().x + 115, view.getCenter().y - 130);//позиция фона для блока
			window.draw(s_quest); window.draw(text); //рисуем спрайт свитка (фон для текста миссии). а затем и текст. все это завязано на логическую переменную, которая меняет свое состояние от нажатия клавиши ТАБ
		}
        window.draw(easyEnemy.sprite);
		window.draw(p.sprite);//рисуем спрайт объекта p класса player
		window.draw(bumsprite);
		window.display();
	}

	return 0;
}

/*
if (p.life)
		{
                float timeboom = clockboom.getElapsedTime().asMicroseconds();
                timeboom = timeboom / 800;

				Cadr1 += 0.0001 * timeboom;
				if ( (Cadr1 > 9) && (Cadr2 == 0) ) {Cadr1 -= 8; Cadr2++;}
				if ( (Cadr1 > 9) && (Cadr2 == 1) ) {Cadr1 -= 8; Cadr2++;}
				if ( (Cadr1 > 9) && (Cadr2 == 2) ) {Cadr1 -= 8; Cadr2++;}
				if ( (Cadr1 > 9) && (Cadr2 == 3) ) {Cadr1 -= 8; Cadr2++;}
				if ( (Cadr1 > 9) && (Cadr2 == 4) ) {Cadr1 -= 8; Cadr2++;}
				if ( (Cadr1 > 9) && (Cadr2 == 5) ) {Cadr1 -= 8; Cadr2++;}
				if ( (Cadr1 > 9) && (Cadr2 == 6) ) {Cadr1 -= 8; Cadr2++;}
				if ( (Cadr1 > 9) && (Cadr2 == 7) ) {Cadr1 -= 8; Cadr2++;}
				if ( (Cadr1 > 9) && (Cadr2 == 8) ) {Cadr1 -= 8; Cadr2++;}
				if ( (Cadr1 > 9) && (Cadr2 == 9) ) {Cadr1 -= 8; Cadr2++;}
				if ( (Cadr1 > 200) && (Cadr2 == 10) ) {Cadr1 = 0; Cadr2 = 0; clockboom.restart();}

               // bumsprite.setPosition(p.getPlayerCoordinateX() - 32, p.getPlayerCoordinateY()-44);

				bumsprite.setTextureRect(IntRect(100 * int(Cadr1), 100 * int(Cadr2), 100, 100));

		}




if ((Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A)))) {
				p.dir = 1; p.speed = 0.1;//dir =1 - направление вверх, speed =0.1 - скорость движения. Заметьте - время мы уже здесь ни на что не умножаем и нигде не используем каждый раз
				Cadr += 0.005 * time;
				if (Cadr > 4) Cadr -= 4;
				p.sprite.setTextureRect(IntRect(60 * int(Cadr), 80, 60, 80));
				getPlayerCoordinateForview(p.getPlayerCoordinateX(), p.getPlayerCoordinateY());//передаем координаты игрока в функцию управления камеро
			}

			if ((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D)))) {
				p.dir = 0; p.speed = 0.1;//направление вправо, см выше
				Cadr += 0.005 * time;
				if (Cadr > 4) Cadr -= 4;
				p.sprite.setTextureRect(IntRect(60 * int(Cadr), 0, 60, 80));
				getPlayerCoordinateForview(p.getPlayerCoordinateX(), p.getPlayerCoordinateY());
			}

			if ((Keyboard::isKeyPressed(Keyboard::Up) || (Keyboard::isKeyPressed(Keyboard::W)))) {
				p.dir = 2; p.speed = 0.1;//направление вниз, см выше
				Cadr += 0.005 * time;
				if (Cadr > 3) Cadr -= 3;
				p.sprite.setTextureRect(IntRect(96 * int(Cadr), 288, 96, 96));
				getPlayerCoordinateForview(p.getPlayerCoordinateX(), p.getPlayerCoordinateY());
			}

			if ((Keyboard::isKeyPressed(Keyboard::Right) && (Keyboard::isKeyPressed(Keyboard::Space)))) {
				p.dir = 0; p.speed = 0.1;//dir =1 - направление вверх, speed =0.1 - скорость движения. Заметьте - время мы уже здесь ни на что не умножаем и нигде не используем каждый раз
				Cadr += 0.005 * time;
				if (Cadr > 4) Cadr -= 4;
				p.sprite.setTextureRect(IntRect(60 * int(Cadr), 160, 60, 80));
				getPlayerCoordinateForview(p.getPlayerCoordinateX(), p.getPlayerCoordinateY());//передаем координаты игрока в функцию управления камеро
			}


			if ((Keyboard::isKeyPressed(Keyboard::Left) && (Keyboard::isKeyPressed(Keyboard::Space)))) {
				p.dir = 1; p.speed = 0.1;//dir =1 - направление вверх, speed =0.1 - скорость движения. Заметьте - время мы уже здесь ни на что не умножаем и нигде не используем каждый раз
				Cadr += 0.005 * time;
				if (Cadr > 4) Cadr -= 4;
				p.sprite.setTextureRect(IntRect(60 * int(Cadr), 240, 60, 80));
				getPlayerCoordinateForview(p.getPlayerCoordinateX(), p.getPlayerCoordinateY());//передаем координаты игрока в функцию управления камеро
			}
			*/

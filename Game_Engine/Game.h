#pragma once

#include "Dependencies.h"

#include "EntityManager.h"
#include "Entity.h"

#define SCREENWIDTH 1280
#define SCREENHEIGHT 720

struct PlayerProperties { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S; };
struct EnemyProperties  { int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float SMIN, SMAX; };
struct BulletProperties { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; float S; };

class Game
{
	sf::RenderWindow    m_window;
	sf::Font            m_font;
	sf::Text            m_text;
	sa::EntityManager   m_entities;
	PlayerProperties    m_playerProperties;
	EnemyProperties     m_enemyProperties;
	BulletProperties    m_bulletProperties;
	int                 m_score = 0;
	int                 m_currentFrame = 0;
	int                 m_lastEnemySpawnTime = 0;
	int                 m_enemyspawndelay = 0;
	bool                m_paused  = false;
	bool                m_running = true;

	std::shared_ptr<sa::Entity> m_player;

	void init(const std::string& config);
	void setPaused(bool paused);

	void sMovement();
	void sUserInput();
	void sLifespan();
	void sRender();
	void sEnemySpawner();
	void sCollision();

	void spawnPlayer();
	void spawnEnemy();
	void spawnSmallEnemies(std::shared_ptr<sa::Entity> entity);
	void spawnBullet(std::shared_ptr<sa::Entity> entity, const sa::Vec2& mousePos);
	void spawnSpecialWeapon(std::shared_ptr<sa::Entity> entity);

	void initEntityProperties();
	void resetPos(std::shared_ptr<sa::Entity> entity);
	bool checkCollision(std::shared_ptr<sa::Entity> c1, std::shared_ptr<sa::Entity> c2);

public:

	Game(const std::string& config);

	void run();
};


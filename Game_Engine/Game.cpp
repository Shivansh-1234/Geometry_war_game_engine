#include "Game.h"

Game::Game(const std::string& config)
{
	this->init(config);
}

void Game::init(const std::string& config)
{
	m_window.create(sf::VideoMode(SCREENWIDTH, SCREENHEIGHT), "ECS GAME", sf::Style::Default);
	m_window.setFramerateLimit(60);

	this->initEntityProperties();

	spawnPlayer();
	//spawnEnemy();
	//spawnBullet(m_player, sa::Vec2());
}

void Game::sMovement()
{
	sa::Vec2 playerVelocity;

	if (m_player->cInput->left)
		playerVelocity.x -= m_playerProperties.S;
	if (m_player->cInput->right)
		playerVelocity.x += m_playerProperties.S;
	if (m_player->cInput->up)
		playerVelocity.y -= m_playerProperties.S;
	if (m_player->cInput->down)
		playerVelocity.y += m_playerProperties.S;

	m_player->cTransform->speed = playerVelocity;

	m_player->cTransform->speed.normalize();

	m_player->cTransform->speed *= m_playerProperties.S;

	for (auto e : m_entities.getEntities())
	{
		e->cTransform->pos += e->cTransform->speed;
	}
	//std::cout << m_player->cTransform->speed.length() << std::endl;
	//m_player->cTransform->pos += m_player->cTransform->speed;
}

void Game::sUserInput()
{
	sf::Event event;

	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_running = false;
		}

		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				m_player->cInput->up = true;
				//std::cout << "W KEY PRESSED" << std::endl;
				break;
			case sf::Keyboard::S:
				m_player->cInput->down = true;
				//std::cout << "S KEY PRESSED" << std::endl;
				break;
			case sf::Keyboard::A:
				m_player->cInput->left = true;
				//std::cout << "A KEY PRESSED" << std::endl;
				break;
			case sf::Keyboard::D:
				m_player->cInput->right = true;
				//std::cout << "D KEY PRESSED" << std::endl;
				break;
			case sf::Keyboard::Escape:
				setPaused(m_paused ? false : true);
				break;
			}
		}

		if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				m_player->cInput->up = false;
				//std::cout << "W KEY RELEASED" << std::endl;
				break;
			case sf::Keyboard::S:
				m_player->cInput->down = false;
				//std::cout << "S KEY RELEASED" << std::endl;
				break;
			case sf::Keyboard::A:
				m_player->cInput->left = false;
				//std::cout << "A KEY RELEASED" << std::endl;
				break;
			case sf::Keyboard::D:
				m_player->cInput->right = false;
				//std::cout << "D KEY RELEASED" << std::endl;
				break;
			}
		}

		if (event.type == sf::Event::MouseButtonPressed)
		{
			switch (event.mouseButton.button)
			{
			case sf::Mouse::Left:
				//std::cout << "Left Mouse Button Clicked . Pos x : " << event.mouseButton.x << " y : " << event.mouseButton.y << std::endl;
				spawnBullet(m_player, sa::Vec2(event.mouseButton.x, event.mouseButton.y));
				break;
			case sf::Mouse::Right:
				std::cout << "Right Mouse Button Clicked . Pos x : " << event.mouseButton.x << " y : " << event.mouseButton.y << std::endl;
				break;
			}
		}
	}
}

void Game::sLifespan()
{
	


	for (auto e : m_entities.getEntities("smallEnemy"))
	{
		auto alpha = e->cShape->circle.getFillColor().a;

		if (!e->cLifespan) { continue; }

		if (e->cLifespan->remLifeSpan > 0) 
		{ 
			e->cLifespan->remLifeSpan--; 
		}
		if (e->cLifespan && e->isActive())
		{
			sf::Color fc = e->cShape->circle.getFillColor();
			sf::Color oc = e->cShape->circle.getOutlineColor();

			alpha-= 1.5f;
			//std::cout << alpha << std::endl;

			sf::Color newOC(fc.r, fc.g, fc.b, alpha);
			sf::Color newFC(oc.r, oc.g, oc.b, alpha);

			e->cShape->circle.setFillColor(newOC);
			e->cShape->circle.setOutlineColor(newFC);
		}

		if (e->cLifespan->remLifeSpan <= 0)
		{
			e->destroy();
		}
	}
}

void Game::sRender()
{
	m_window.clear();

	for (auto e : m_entities.getEntities())
	{
		e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
		e->cTransform->angle += 1.f;
		e->cShape->circle.setRotation(e->cTransform->angle);

		m_window.draw(e->cShape->circle);
	}

	m_window.display();
}

void Game::sEnemySpawner()
{
	this->m_enemyspawndelay = 75;

	if ((m_currentFrame - m_lastEnemySpawnTime) > m_enemyspawndelay)
	{
		spawnEnemy();
	}
}

void Game::sCollision()
{
	for (auto p : m_entities.getEntities("player"))
	{
		for (auto e : m_entities.getEntities("enemy"))
		{
			if (checkCollision(p, e))
			{
				resetPos(m_player);
			}
		}
	}

	for (auto p : m_entities.getEntities("player"))
	{
		for (auto e : m_entities.getEntities("smallEnemy"))
		{
			if (checkCollision(p, e))
			{
				resetPos(m_player);
			}
		}
	}

	for (auto b : m_entities.getEntities("bullet"))
	{
		for (auto e : m_entities.getEntities("enemy"))
		{
			if (checkCollision(b, e))
			{
				e->destroy();
				b->destroy();
				spawnSmallEnemies(e);
			}
		}
	}

	for (auto b : m_entities.getEntities("bullet"))
	{
		for (auto e : m_entities.getEntities("smallEnemy"))
		{
			if (checkCollision(b, e))
			{
				e->destroy();
				b->destroy();
			}
		}
	}
	
	for (auto e : m_entities.getEntities("enemy"))
	{
		if (e->cTransform->pos.x - e->cCollision->radius < 0)
			e->cTransform->speed.x *= -1;
							 
		if (e->cTransform->pos.x + e->cCollision->radius > SCREENWIDTH)
			e->cTransform->speed.x *= -1;

		if (e->cTransform->pos.y - e->cCollision->radius < 0)
			e->cTransform->speed.y *= -1;

		if (e->cTransform->pos.y + e->cCollision->radius > SCREENHEIGHT)
			e->cTransform->speed.y *= -1;
	}

	if (m_player->cTransform->pos.x - m_player->cCollision->radius < 0)
		m_player->cTransform->pos.x = m_player->cCollision->radius;

	if (m_player->cTransform->pos.x + m_player->cCollision->radius > SCREENWIDTH)
		m_player->cTransform->pos.x = SCREENWIDTH - m_player->cCollision->radius;

	if (m_player->cTransform->pos.y - m_player->cCollision->radius < 0)
		m_player->cTransform->pos.y = m_player->cCollision->radius;

	if (m_player->cTransform->pos.y + m_player->cCollision->radius > SCREENHEIGHT)
		m_player->cTransform->pos.y = SCREENHEIGHT - m_player->cCollision->radius;
}

void Game::spawnPlayer()
{
	auto entity        = m_entities.addEntity("player");
	entity->cTransform = std::make_shared<sa::CTransform>(sa::Vec2(SCREENWIDTH/2,SCREENHEIGHT/2), sa::Vec2(m_playerProperties.S, m_playerProperties.S), 0.0f);
	entity->cShape     = std::make_shared<sa::CShape>(m_playerProperties.SR, m_playerProperties.V, sf::Color(m_playerProperties.FR, m_playerProperties.FG, m_playerProperties.FB), sf::Color(m_playerProperties.OR,									m_playerProperties.OG,m_playerProperties.OB), m_playerProperties.OT);
	entity->cInput     = std::make_shared<sa::CInput>();
	entity->cCollision = std::make_shared<sa::CCollision>(m_playerProperties.CR);

	m_player = entity;
}

void Game::spawnEnemy()
{
	float randomSpeed     = m_enemyProperties.SMIN + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (m_enemyProperties.SMAX - m_enemyProperties.SMIN)));
	int   randomPoint     = m_enemyProperties.VMIN + (rand() % (1 + m_enemyProperties.VMAX - m_enemyProperties.VMIN));
	int   randomPosWidth  = m_enemyProperties.SR   +  rand() % (1 + ((SCREENWIDTH - m_enemyProperties.SR) - m_enemyProperties.SR));
	int   randomPosHeight = m_enemyProperties.SR   +  rand() % (1 + ((SCREENHEIGHT- m_enemyProperties.SR) - m_enemyProperties.SR));
	sa::Vec2 randomColor  = { static_cast<float>(rand() % 256) , static_cast<float>(rand() % 256) };

	auto enemy        = m_entities.addEntity("enemy");
	enemy->cTransform = std::make_shared<sa::CTransform>(sa::Vec2(randomPosWidth,randomPosHeight), sa::Vec2(randomSpeed, randomSpeed), 0.0f);
	enemy->cShape     = std::make_shared<sa::CShape>(m_enemyProperties.SR, randomPoint, sf::Color(sf::Color(randomColor.x, randomColor.y, 0)), sf::Color(m_enemyProperties.OR, m_enemyProperties.OG, m_enemyProperties.OB), m_enemyProperties.OT);
	enemy->cCollision = std::make_shared<sa::CCollision>(m_enemyProperties.CR);

	m_lastEnemySpawnTime = m_currentFrame;
}

void Game::spawnSmallEnemies(std::shared_ptr<sa::Entity> entity)
{
	auto pointCount = entity->cShape->circle.getPointCount();
	for (int i = 0; i < entity->cShape->circle.getPointCount(); i++)	
	{
		sf::Vector2f temp = entity->cShape->circle.getTransform().transformPoint(entity->cShape->circle.getPoint(i));	
		//sa::Vec2 randomSpeed = { -3 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3 - (-3)))) , -3 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3 - (-3)))) };

		sa::Vec2 velocityVector = entity->cTransform->speed;
		float angle = deg2rad(360.f / pointCount);
		sa::Vec2 rotatedVector(
			(std::cosf(angle * (i + 1)) * velocityVector.x) - (std::sinf(angle * (i + 1)) * velocityVector.y), 
			(std::sinf(angle * (i + 1)) * velocityVector.x) + (std::cosf(angle * (i + 1)) * velocityVector.y)
							  );


		auto se = m_entities.addEntity("smallEnemy");
		se->cTransform = std::make_shared<sa::CTransform>(sa::Vec2(temp.x, temp.y), rotatedVector, 0.0f);
		se->cShape     = std::make_shared<sa::CShape>(entity->cShape->circle.getRadius() / 2.f, entity->cShape->circle.getPointCount(), entity->cShape->circle.getFillColor(), entity->cShape->circle.getOutlineColor(), 
						 entity->cShape->circle.getOutlineThickness());
		se->cCollision = std::make_shared<sa::CCollision>(entity->cShape->circle.getRadius());
		se->cLifespan  = std::make_shared<sa::CLifespan>(100);

		//std::cout << "X : " << entity->cTransform->speed.x << " Y : " << entity->cTransform->speed.y << std::endl;
	}
}

void Game::spawnBullet(std::shared_ptr<sa::Entity> entity, const sa::Vec2& mousePos)
{
	sa::Vec2 d{ mousePos.x - entity->cTransform->pos.x, mousePos.y - entity->cTransform->pos.y };
	d.normalize();
	sa::Vec2 bulletVelocity = d * m_bulletProperties.S;

	auto bullet = m_entities.addEntity("bullet");
	bullet->cTransform = std::make_shared<sa::CTransform>(sa::Vec2(entity->cTransform->pos.x, entity->cTransform->pos.y), bulletVelocity, 0.0f);
	bullet->cShape     = std::make_shared<sa::CShape>(m_bulletProperties.SR, m_bulletProperties.V, sf::Color(m_bulletProperties.FR, m_bulletProperties.FG, m_bulletProperties.FB), sf::Color(m_bulletProperties.OR,									m_bulletProperties.OG, m_bulletProperties.OB), m_bulletProperties.OT);
	bullet->cCollision = std::make_shared<sa::CCollision>(m_bulletProperties.CR);
}

void Game::spawnSpecialWeapon(std::shared_ptr<sa::Entity> entity)
{

}

bool Game::checkCollision(std::shared_ptr<sa::Entity> c1, std::shared_ptr<sa::Entity> c2)
{
	sa::Vec2 distance{ c1->cTransform->pos.x - c2->cTransform->pos.x, c1->cTransform->pos.y - c2->cTransform->pos.y };
	float r1 = c1->cCollision->radius;
	float r2 = c2->cCollision->radius;

	if (distance.length_sq() < ((r1 + r2) * (r1 + r2)))
	{
		return true;
	}

	return false;
}

void Game::resetPos(std::shared_ptr<sa::Entity> entity)
{
	entity->cTransform->pos = sa::Vec2(SCREENWIDTH / 2, SCREENHEIGHT / 2);
}

void Game::initEntityProperties()
{
	//PLAYER
	m_playerProperties.SR  = 32;
	m_playerProperties.CR  = 32;
	m_playerProperties.S   = 8;
	m_playerProperties.FR  = 5;
	m_playerProperties.FG  = 5;
	m_playerProperties.FB  = 5;
	m_playerProperties.OR  = 255;
	m_playerProperties.OG  = 0;
	m_playerProperties.OB  = 0;
	m_playerProperties.OT  = 4;
	m_playerProperties.V   = 8;

	//ENEMIES
	m_enemyProperties.SR   = 32;
	m_enemyProperties.CR   = 32;
	m_enemyProperties.SMIN = 1.5f;
	m_enemyProperties.SMAX = 3.5f;
	m_enemyProperties.OR   = 255;
	m_enemyProperties.OG   = 255;
	m_enemyProperties.OB   = 255;
	m_enemyProperties.OT   = 2;
	m_enemyProperties.VMIN = 3;
	m_enemyProperties.VMAX = 8;
	m_enemyProperties.L    = 90;
	m_enemyProperties.SI   = 120;

	//Bullet
	m_bulletProperties.SR  = 10;
	m_bulletProperties.CR  = 10;
	m_bulletProperties.S   = 20;
	m_bulletProperties.FR  = 255;
	m_bulletProperties.FG  = 255;
	m_bulletProperties.FB  = 255;
	m_bulletProperties.OR  = 255;
	m_bulletProperties.OG  = 255;
	m_bulletProperties.OB  = 255;
	m_bulletProperties.OT  = 2;
	m_bulletProperties.V   = 20;
	m_bulletProperties.L   = 90;
}

void Game::setPaused(bool paused)
{
	this->m_paused = paused;
}

void Game::run()
{
	while (m_running)
	{
		m_entities.update();

		if (!m_paused)
		{
			sEnemySpawner();
			sMovement();
			sCollision();
			sLifespan();
		}

		sUserInput();
		sRender();
		m_currentFrame++;
	}
}


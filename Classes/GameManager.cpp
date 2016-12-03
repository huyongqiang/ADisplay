#include "GameManager.h"

GameManager::GameManager()
{
}


GameManager::~GameManager()
{
}

GameManager* GameManager::instance = NULL;

GameManager* GameManager::getInstance()
{
	if (instance == NULL)
		instance = new GameManager();

	return instance;
}
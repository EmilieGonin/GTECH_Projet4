#pragma once
#include "json.hpp"
#include "Game.h"

using json = nlohmann::json;

enum JsonType { NOTIF, REQUEST, RESPONSE };
//Id : pour savoir ce que va contenir le corps en terme de champs
//Code erreur : si c'est une response (0 = ok, -1 = pas ok)

//Id 1 : Request client : jouer une case - playerId, cell
//Id 2 : Request client : get la grille (lors de la connexion) - playerId
//Id 3 : Response server : oui ou non, renvoi de toutes les cases
//Id 4 : Notif server : win - playerId, renvoi de toutes les cases

class JsonHandler
{
public:
	JsonHandler(std::pair<int, int>, int); //1
	JsonHandler(int); //2
	JsonHandler(std::string);
	inline json getJson() { return mJson; };
	inline std::string getDump() { return mDump; };

private:
	json mJson;
	std::string mDump;
};
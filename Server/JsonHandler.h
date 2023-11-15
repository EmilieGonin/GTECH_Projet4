#pragma once
#include "json.hpp"
#include "Game.h"

using json = nlohmann::json;

//Type : requête, réponse à une requête, notif
//Identifiant : pour savoir ce que va contenir le corps en terme de champs
//Code d'erreur : si c'est une réponse

enum JsonType { NOTIF, REQUEST, RESPONSE };

//Id 1 : Request client : jouer une case - playerId, cell
//Id 2 : Request client : récupérer la grille (lors de la connexion) - playerId
//Id 3 : Response server : oui ou non, renvoi de toutes les cases

struct AllCells {
	//
};

class JsonHandler
{
public:
	JsonHandler(std::map<std::pair<int, int>, struct cell>);
	inline json getJson() { return mJson; };

private:
	json mJson;
};
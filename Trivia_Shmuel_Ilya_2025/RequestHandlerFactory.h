#pragma once
#include "LoginManager.h"
#include "IDatabase.h"
#include "MenuRequestHandler.h"
#include "LoginRequestHandler.h" 

class LoginRequestHandler;


class RequestHandlerFactory
{
private:
	LoginManager _loginManager;
	SqliteDataBase* _database; //need to check ts 🥀🥀🥀💔💔💔
public:
	//ts pmo this gooffy ahh magshimim cant even write a normal fucking xml that will be understandable tf you mean you want me to have _database where is this fucking databse come from maybe you should specify the fgucking copnstructor or tell me what the constructor need to take cuz fuck you mean i need to tihink for my self where to find the fucking database and why tf i not only need the fukicng duatabasse ass my private feield i even need to pass it into the FUCKING loginManager class that the xml doesnt even specify that he supposed to have a fucking constructor that takes in the database and fuck you mean i need to include the fucking server class? why would i even  use it its not like i need to the the database from there sir maybe if it would have a fucking getter somewhere i would think about it and why tf do i eveen tneed tyhe sqlutedatabase class vro ICL TS PMO sm n sb rn ngl r u srsly srs n fr rn vro lol atp js go b fr vro idek nm brb gng gtg atm lmao bt ts pyo 2 js llmk lol ong fr nty b fr rnk lk br
	RequestHandlerFactory(SqliteDataBase* database);
	LoginRequestHandler* createLoginRequestHandler();
	MenuRequestHandler* createMenuRequestHandler();
	LoginManager& getLoginManager();
};


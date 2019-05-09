#include <MyContactListener.h>

MyContactListener::MyContactListener(Player* p) {
	this->p = p;
}

void MyContactListener::BeginContact(b2Contact* contact) {
	//check if fixture A was the foot sensor
	void* fixtureAUserData = contact->GetFixtureA()->GetUserData();
	void* fixtureBUserData = contact->GetFixtureB()->GetUserData();

	if ((int)fixtureAUserData == PLAYER_FOOT_B2_USER_DATA && (int)fixtureAUserData == MAP_B2_USER_DATA)
		p->numFootContacts++;
	//check if fixture B was the foot sensor
	
	if ((int)fixtureBUserData == PLAYER_FOOT_B2_USER_DATA && (int)fixtureAUserData == MAP_B2_USER_DATA)
		p->numFootContacts++;
}

void MyContactListener::EndContact(b2Contact* contact) {
	//check if fixture A was the foot sensor
	void* fixtureAUserData = contact->GetFixtureA()->GetUserData();
	void* fixtureBUserData = contact->GetFixtureB()->GetUserData();

	if ((int)fixtureAUserData == PLAYER_FOOT_B2_USER_DATA && (int)fixtureAUserData == MAP_B2_USER_DATA)
		p->numFootContacts--;
	//check if fixture B was the foot sensor
	if ((int)fixtureBUserData == PLAYER_FOOT_B2_USER_DATA && (int)fixtureAUserData == MAP_B2_USER_DATA)
		p->numFootContacts--;
}
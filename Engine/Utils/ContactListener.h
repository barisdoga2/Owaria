#pragma once

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <ContactData.h>


class ContactListener : public b2ContactListener
{

public:
	void BeginContact(b2Contact* contact) {
		Contact(contact, true);
	}

	void EndContact(b2Contact* contact) {
		Contact(contact, false);
	}

private:

	void Contact(b2Contact* p_b2Contact, bool isBegin) {
		
		void* a_bodyUserData = p_b2Contact->GetFixtureA()->GetBody()->GetUserData();
		void* b_bodyUserData = p_b2Contact->GetFixtureB()->GetBody()->GetUserData();

		if (a_bodyUserData != NULL && b_bodyUserData != NULL) {

			int type = ((ContactData*)a_bodyUserData)->getDataType();
			void* object = ((ContactData*)a_bodyUserData)->getData();
			if (type == CONTACT_TYPE_MAP_INSTANCE) {
				((Map*)object)->HandleCollision(p_b2Contact->GetFixtureA(), p_b2Contact->GetFixtureB(), isBegin);
			}
			else if (type == CONTACT_TYPE_PLAYER_INSTANCE) {
				((Player*)object)->HandleCollision(p_b2Contact->GetFixtureA(), p_b2Contact->GetFixtureB(), isBegin);
			}

			type = ((ContactData*)b_bodyUserData)->getDataType();
			object = ((ContactData*)b_bodyUserData)->getData();
			if (type == CONTACT_TYPE_MAP_INSTANCE) {
				((Map*)object)->HandleCollision(p_b2Contact->GetFixtureB(), p_b2Contact->GetFixtureA(), isBegin);
			}
			else if (type == CONTACT_TYPE_PLAYER_INSTANCE) {
				((Player*)object)->HandleCollision(p_b2Contact->GetFixtureB(), p_b2Contact->GetFixtureA(), isBegin);
			}

		}

	}

};
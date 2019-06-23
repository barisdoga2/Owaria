#pragma once

#define CONTACT_TYPE_UNDEFINED 0
#define CONTACT_TYPE_PLAYER_INSTANCE 1
#define CONTACT_TYPE_MAP_INSTANCE 2
#define CONTACT_TYPE_MOB_INSTANCE 4
#define CONTACT_TYPE_SENSOR_INT 3


#define FOOT_SENSOR 1
#define LADDER_SENSOR 2
#define MAP_SENSOR 3

class ContactData
{
public:
	ContactData(int type, void* object) {
		this->type = type;
		this->object = object;
	}

	int getDataType() {
		return this->type;
	}

	void* getData() {
		return this->object;
	}

private:
	int type;
	void* object;

};

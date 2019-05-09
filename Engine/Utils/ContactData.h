#pragma once

#define CONTACT_TYPE_UNDEFINED 0
#define CONTACT_TYPE_PLAYER_INSTANCE 1
#define CONTACT_TYPE_MAP_INSTANCE 2
#define CONTACT_TYPE_SENSOR_INT 3

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

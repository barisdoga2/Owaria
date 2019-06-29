#pragma once

#include <ItemAsset.h>

class Item{
public:
	Item(ItemAsset* itemAsset) {
		this->itemAsset = itemAsset;
	}

	ItemAsset* GetItemAsset() {
		return itemAsset;
	}

protected:
	ItemAsset * itemAsset;

};
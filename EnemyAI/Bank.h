#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "framework.h"

template <class T>
class Bank {
public:
	Bank()
	{}

	/*
		RETURN VALUE
		Returns kSuccess if the item was inserted in the bank else kFailure.
		Inserting an item can fail if the name already refers to an existing item in the bank.
		In case of success, the function returns the item id using the OUT parameter if the pointer is not null.
	*/
	Result Insert(IN const T &item, IN const std::string &name, OUT uint *id)
	{
		// Check if the name is already taken by an item
		auto it = m_mapNamesToIds.find(name);
		if (it != m_mapNamesToIds.end()) {
			return kFailure;
		}

		// Success!

		m_items.push_back(item);
		auto i = static_cast<uint>(m_items.size() - 1);
		m_items[i].id = i;

		m_mapNamesToIds.insert({ name, i });

		if (id) {
			*id = i;
		}

		return kSuccess;
	}

	/*
		PRECONDITIONS
		* item != nullptr
		RETURN VALUE
		Returns kSuccess if the id corresponds to an item in the bank else kFailure.
		In case of success, the function returns the address of the item using the OUT parameter.
	*/
	Result Find(IN uint id, OUT const T **item)
	{
		assert(item);

		if (id < m_items.size()) {
			*item = &m_items[id];
			return kSuccess;
		}

		return kFailure;
	}

	/*
		PRECONDITIONS
		* item != nullptr
		RETURN VALUE
		Returns kSuccess if the name corresponds to an item in the bank else kFailure.
		In case of success, the function returns the address of the item using the OUT parameter.
	*/
	Result Find(IN const char *name, OUT const T **item)
	{
		assert(item);

		auto it = m_mapNamesToIds.find(name);
		if (it != m_mapNamesToIds.end()) {
			*item = &m_items[it->second];
			return kSuccess;
		}

		return kFailure;
	}

private:
	std::vector<T>								m_items;
	std::unordered_map<std::string, uint>		m_mapNamesToIds;
};
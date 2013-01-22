/*
 * Copyright (C) 2011 Ivan Nikolaev
 *
 * This file is part of cap2re.
 *
 * cap2re is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * cap2re is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with cap2re.
 * If not, see http://www.gnu.org/licenses
 *
 */

#ifndef CAP2RE_NODE_H
#define CAP2RE_NODE_H

#include "Common.h"

#include <vector>

class Node;
typedef std::vector<Node*> NodePtrList;

class Node
{
public:
	Node(const String &name, const StringList &options = StringList());
	virtual
	~Node();

	virtual String
	getName() const;

	virtual void
	addOption(const String &opt);
	virtual StringList
	getOptions() const;
	virtual void
	setOptions(const StringList &optsList);


	virtual void
	setValue(const String &v);
	virtual String
	getValue() const;

	virtual void
	addChild(Node *n);
	virtual Node*
	getChild(const String &name) const;
	virtual NodePtrList
	getChildren() const;

protected:
	String          _name;
	NodePtrList     _children;
	StringList      _options;
	String          _value;
};

// CAP2RE_NODE_H
#endif

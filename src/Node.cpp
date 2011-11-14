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

#include "Node.h"

Node::Node(const String &name, const StringList &options):
    _options(options),
    _name(name)
{

}

Node::~Node()
{
    for(auto node: _children)
        delete node;
}

String
Node::getName() const
{
    return _name;
}

void
Node::addOption(const String &opt)
{
    _options.push_back(opt);
}

void
Node::setOptions(const StringList &optsList)
{
    _options = optsList;
}

StringList
Node::getOptions() const
{
    return _options;
}

String
Node::getValue() const
{
    return _value;
}

void
Node::setValue(const String &v)
{
    _value = v;
}

void
Node::addChild(Node *n)
{
    _children.push_back(n);
}

Node*
Node::getChild(const String &name) const
{
    // Check each child node for matching
    for(auto node: _children)
        if(node->getName() == name)
            return node;

    return nullptr;
}

NodePtrList
Node::getChildren() const
{
    return _children;
}

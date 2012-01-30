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

#include "ConfigOptsTree.h"
#include "Error.h"

#include <boost/regex.hpp>
#include <iostream>
using namespace boost;
using namespace std;

bool
isPathValid(const String &path, const String &separator)
{
    regex pattern("([\\w|\\d]+" + separator + "?)*");

    try {
        if(!boost::regex_match(path, pattern))
            return false;
    }
    catch(std::exception &e)
    {
        throw ValueError("regexp error caused by invalid path string");
    }

    return true;
}

StringList
splitPath(const String &path, const String &separator)
{
    if(!isPathValid(path, separator))
        throw ValueError("invalid path string '"+path+"'");

    StringList nodes;
    String remainingPath = path;

    regex namePattern("([\\w|\\d]+)");
    regex removePattern("([\\w|\\d]+" + separator + "?)");

    smatch nodeName;

    while(remainingPath.length())
    {
        // Extract the node name
        if(regex_search(remainingPath, nodeName, namePattern, boost::format_first_only))
            nodes.push_back(nodeName[1]);

        // Remove the found node from path beginning
        remainingPath = regex_replace(remainingPath, removePattern, "$'", format_no_copy | format_first_only);
    }
    return nodes;
}

ConfigOptsTree::ConfigOptsTree(const char separator):
    _root(new Node("root"))
{
    // Check if the given separator isn't an alphanumeric character or space
    regex valid("(\\d|\\w|\\s)");
    regex special("(\\^|\\$|\\.|\\*|\\+|\\?|\\(|\\)|\\[|\\]|\\{|\\}|\\\\|\\|)");

    smatch m;

    if(regex_match((String)&separator, valid))
        throw ValueError("invalid separator '" + (String)&separator + "'");

    // Prepend the escape character if the separator is any of ^$.*+?|()[]{}\ characters
    if(regex_match((String)&separator, special))
        _separator = String("\\") + (String)&separator;
    else
        _separator = (String)&separator;
}

ConfigOptsTree::~ConfigOptsTree()
{
    delete _root;
}

Node*
ConfigOptsTree::addEntry(const String &path, const StringList &opts)
{
    StringList nodes = splitPath(path, _separator);

    Node *node = _root;
    Node *child;

    // Starting from root, go down the tree and create missing nodes
    StringList::iterator nodeName;
    for(nodeName = nodes.begin(); nodeName != nodes.end(); nodeName++)
    {
        child = node->getChild(*nodeName);

        if(!child)
        {
            child = new Node(*nodeName, opts);
            node->addChild(child);
        }

        node = child;
    }

    return node;
}

Node*
ConfigOptsTree::getEntry(const String &path) const
{
    StringList nodes = splitPath(path, _separator);

    Node *node = _root;

    StringList::iterator nodeName;
    for(nodeName = nodes.begin(); nodeName != nodes.end(); nodeName++)
        if(node)
            node = node->getChild(*nodeName);
        else
            break;

    return node;
}

String
ConfigOptsTree::getValue(const String &path) const
{
    return getEntry(path)->getValue();
}

StringList
ConfigOptsTree::getOptions(const String &path) const
{
    return getEntry(path)->getOptions();
}

void
ConfigOptsTree::setValue(const String &path, const String &value)
{
    getEntry(path)->setValue(value);
}

String
ConfigOptsTree::getSeparator() const
{
    return _separator;
}

/*
 * Boost.Foreach support for pugixml classes.
 * This file is provided to the public domain.
 * Written by Arseny Kapoulkine (arseny.kapoulkine@gmail.com)
 */

#ifndef HEADER_PUGIXML_FOREACH_HPP
#define HEADER_PUGIXML_FOREACH_HPP

#include "pugixml.hpp"

/*
 * These types add support for BOOST_FOREACH macro to xml_node and xml_document classes (child iteration only).
 * Example usage:
 * BOOST_FOREACH(xml_node n, doc) {}
 */

namespace boost
{
	template <typename> struct range_mutable_iterator;
	template <typename> struct range_const_iterator;

	template<> struct range_mutable_iterator<DuiXmlNode>
	{
		typedef DuiXmlNode::iterator type;
	};

	template<> struct range_const_iterator<DuiXmlNode>
	{
		typedef DuiXmlNode::iterator type;
	};

	template<> struct range_mutable_iterator<DuiXmlDocument>
	{
		typedef DuiXmlDocument::iterator type;
	};

	template<> struct range_const_iterator<DuiXmlDocument>
	{
		typedef DuiXmlDocument::iterator type;
	};
}

/*
 * These types add support for BOOST_FOREACH macro to xml_node and xml_document classes (child/attribute iteration).
 * Example usage:
 * BOOST_FOREACH(xml_node n, children(doc)) {}
 * BOOST_FOREACH(xml_node n, attributes(doc)) {}
 */

namespace pugi
{
	inline xml_object_range<xml_node_iterator> children(const DuiXmlNode& node)
	{
        return node.children();
	}

	inline xml_object_range<xml_attribute_iterator> attributes(const DuiXmlNode& node)
	{
        return node.attributes();
	}
}

#endif

/*
 * Copyright (c) 2013 Isode Limited.
 * All rights reserved.
 * See the COPYING file for more information.
 */

#pragma once

#include <string>
#include <map>
#include <cassert>
#include <boost/optional.hpp>

#include <Swiften/Base/Override.h>
#include <Swiften/Base/API.h>

namespace Swift {
	template<typename T>
	class SWIFTEN_API EnumParser {
		public:
			EnumParser() {
			}

			EnumParser& operator()(T value, const std::string& text) {
				values[text] = value;
				return *this;
			}

			boost::optional<T> parse(const std::string& value) {
				typename std::map<std::string, T>::const_iterator i = values.find(value);
				return i == values.end() ? boost::optional<T>() : i->second;
			}

		private:
			std::map<std::string, T> values;
	};
}

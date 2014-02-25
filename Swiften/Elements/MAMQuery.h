/*
 * Copyright (c) 2014 Kevin Smith and Remko Tronçon
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#pragma once

#include <boost/optional.hpp>
#include <Swiften/Base/API.h>
#include <Swiften/Elements/Form.h>
#include <Swiften/Elements/Forwarded.h>
#include <Swiften/Elements/MAMResult.h>
#include <Swiften/Elements/Payload.h>
#include <Swiften/Elements/ResultSet.h>

namespace Swift {
	class SWIFTEN_API MAMQuery : public Payload {
		public:
			virtual ~MAMQuery();

			void setQueryID(const boost::optional<std::string>& queryID) { queryID_ = queryID; }
			const boost::optional<std::string>& getQueryID() const { return queryID_; }

			void setForm(boost::shared_ptr<Form> form) { form_ = form; }
			const boost::shared_ptr<Form>& getForm() const { return form_; }

			void setResultSet(boost::shared_ptr<ResultSet> resultSet) { resultSet_ = resultSet; }
			const boost::shared_ptr<ResultSet>& getResultSet() const { return resultSet_; }

		private:
			boost::optional<std::string> queryID_;
			boost::shared_ptr<Form> form_;
			boost::shared_ptr<ResultSet> resultSet_;
	};
}

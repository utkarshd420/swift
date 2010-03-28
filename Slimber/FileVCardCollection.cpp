#include "Slimber/FileVCardCollection.h"

#include <boost/filesystem/fstream.hpp>

#include "Swiften/Base/ByteArray.h"
#include "Swiften/Elements/VCard.h"
#include "Swiften/Serializer/PayloadSerializers/VCardSerializer.h"
#include "Swiften/Parser/PayloadParsers/UnitTest/PayloadParserTester.h"
#include "Swiften/Parser/PayloadParsers/VCardParser.h"

namespace Swift {

FileVCardCollection::FileVCardCollection(boost::filesystem::path dir) : vcardsPath(dir) {
}

boost::shared_ptr<VCard> FileVCardCollection::getOwnVCard() const {
	if (boost::filesystem::exists(vcardsPath / std::string("vcard.xml"))) {
		ByteArray data;
		data.readFromFile(boost::filesystem::path(vcardsPath / std::string("vcard.xml")).string());

		VCardParser parser;
		PayloadParserTester tester(&parser);
		tester.parse(String(data.getData(), data.getSize()));
		return boost::dynamic_pointer_cast<VCard>(parser.getPayload());
	}
	else {
		return boost::shared_ptr<VCard>(new VCard());
	}
}

void FileVCardCollection::setOwnVCard(boost::shared_ptr<VCard> v) {
	boost::filesystem::ofstream file(vcardsPath / std::string("vcard.xml"));
	file << VCardSerializer().serializePayload(v);
	file.close();
}

}
#ifndef MYBOX_RESOURCE_H_
#define MYBOX_RESOURCE_H_

#include "OCPlatform.h"
#include "OCApi.h"
#include "ocpayload.h"
#include "Mybox.h"

using namespace OC;
using namespace std;
namespace PH = std::placeholders;

class MyboxResource {
public:
	Mybox mMybox;

	OCResourceHandle mResHandle;
	OCRepresentation mRep;
	ObservationIds mObserver;

public:
	/// Constructor
	MyboxResource();
	/* Note that this does not need to be a member function: for classes you do not have
	 access to, you can accomplish this with a free function: */

	/// This function internally calls registerResource API.
	void createResource();

	OCResourceHandle getHandle();

	// Puts representation.
	// Gets values from the representation and
	// updates the internal state
	void put(OCRepresentation& rep);

	// Post representation.
	// Post can create new resource or simply act like put.
	// Gets values from the representation and
	// updates the internal state
	OCRepresentation post(OCRepresentation& rep);

	// gets the updated representation.
	// Updates the representation with latest internal state before
	// sending out.
	OCRepresentation get();

	void addType(const std::string& type) const;

	void addInterface(const std::string& iface) const;

private:
// This is just a sample implementation of entity handler.
// Entity handler can be implemented in several ways by the manufacturer
	OCEntityHandlerResult entityHandler(std::shared_ptr<OCResourceRequest> request);

};


#endif //MYBOX_RESOURCE_H_

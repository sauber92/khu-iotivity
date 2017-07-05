#include "LightResource.h"

LightResource::LightResource() :
		m_name("John's light"), m_state(false), m_power(0), m_lightUri(
				"/a/light"), m_resourceHandle(nullptr) {
	// Initialize representation
	m_lightRep.setUri(m_lightUri);

	m_lightRep.setValue("state", m_state);
	m_lightRep.setValue("power", m_power);
	m_lightRep.setValue("name", m_name);
}

/* Note that this does not need to be a member function: for classes you do not have
 access to, you can accomplish this with a free function: */

/// This function internally calls registerResource API.
void LightResource::createResource() {
	//URI of the resource
	std::string resourceURI = m_lightUri;
	//resource type name. In this case, it is light
	std::string resourceTypeName = "core.light";
	// resource interface.
	std::string resourceInterface = DEFAULT_INTERFACE;

	// OCResourceProperty is defined ocstack.h
	uint8_t resourceProperty;
	if (isSecure) {
		resourceProperty = OC_DISCOVERABLE | OC_OBSERVABLE | OC_SECURE;
	} else {
		resourceProperty = OC_DISCOVERABLE | OC_OBSERVABLE;
	}
	EntityHandler cb = std::bind(&LightResource::entityHandler, this, PH::_1);

	// This will internally create and register the resource.
	OCStackResult result = OCPlatform::registerResource(m_resourceHandle,
			resourceURI, resourceTypeName, resourceInterface, cb,
			resourceProperty);

	if (OC_STACK_OK != result) {
		cout << "Resource creation was unsuccessful\n";
	}
}

OCStackResult LightResource::createResource1() {
	// URI of the resource
	std::string resourceURI = "/a/light1";
	// resource type name. In this case, it is light
	std::string resourceTypeName = "core.light";
	// resource interface.
	std::string resourceInterface = DEFAULT_INTERFACE;

	// OCResourceProperty is defined ocstack.h
	uint8_t resourceProperty;
	if (isSecure) {
		resourceProperty = OC_DISCOVERABLE | OC_OBSERVABLE | OC_SECURE;
	} else {
		resourceProperty = OC_DISCOVERABLE | OC_OBSERVABLE;
	}
	EntityHandler cb = std::bind(&LightResource::entityHandler, this, PH::_1);

	OCResourceHandle resHandle;

	// This will internally create and register the resource.
	OCStackResult result = OCPlatform::registerResource(resHandle, resourceURI,
			resourceTypeName, resourceInterface, cb, resourceProperty);

	if (OC_STACK_OK != result) {
		cout << "Resource creation was unsuccessful\n";
	}

	return result;
}

OCResourceHandle LightResource::getHandle() {
	return m_resourceHandle;
}

// Puts representation.
// Gets values from the representation and
// updates the internal state
void LightResource::put(OCRepresentation& rep) {
	try {
		if (rep.getValue("state", m_state)) {
			cout << "\t\t\t\t" << "state: " << m_state << endl;
		} else {
			cout << "\t\t\t\t" << "state not found in the representation"
					<< endl;
		}

		if (rep.getValue("power", m_power)) {
			cout << "\t\t\t\t" << "power: " << m_power << endl;
		} else {
			cout << "\t\t\t\t" << "power not found in the representation"
					<< endl;
		}
	} catch (exception& e) {
		cout << e.what() << endl;
	}

}

// Post representation.
// Post can create new resource or simply act like put.
// Gets values from the representation and
// updates the internal state
OCRepresentation LightResource::post(OCRepresentation& rep) {
	static int first = 1;

	// for the first time it tries to create a resource
	if (first) {
		first = 0;

		if (OC_STACK_OK == createResource1()) {
			OCRepresentation rep1;
			rep1.setValue("createduri", std::string("/a/light1"));

			return rep1;
		}
	}

	// from second time onwards it just puts
	put(rep);
	return get();
}

// gets the updated representation.
// Updates the representation with latest internal state before
// sending out.
OCRepresentation LightResource::get() {
	m_lightRep.setValue("state", m_state);
	m_lightRep.setValue("power", m_power);

	return m_lightRep;
}

void LightResource::addType(const std::string& type) const {
	OCStackResult result = OCPlatform::bindTypeToResource(m_resourceHandle,
			type);
	if (OC_STACK_OK != result) {
		cout << "Binding TypeName to Resource was unsuccessful\n";
	}
}

void LightResource::addInterface(const std::string& iface) const {
	OCStackResult result = OCPlatform::bindInterfaceToResource(m_resourceHandle,
			iface);
	if (OC_STACK_OK != result) {
		cout << "Binding TypeName to Resource was unsuccessful\n";
	}
}

// This is just a sample implementation of entity handler.
// Entity handler can be implemented in several ways by the manufacturer
OCEntityHandlerResult LightResource::entityHandler(
		std::shared_ptr<OCResourceRequest> request) {
	cout << "\tIn Server CPP entity handler:\n";
	OCEntityHandlerResult ehResult = OC_EH_ERROR;
	if (request) {
		// Get the request type and request flag
		std::string requestType = request->getRequestType();
		int requestFlag = request->getRequestHandlerFlag();

		if (requestFlag & RequestHandlerFlag::RequestFlag) {
			cout << "\t\trequestFlag : Request\n";
			auto pResponse = std::make_shared<OC::OCResourceResponse>();
			pResponse->setRequestHandle(request->getRequestHandle());
			pResponse->setResourceHandle(request->getResourceHandle());

			// Check for query params (if any)
			QueryParamsMap queries = request->getQueryParameters();

			if (!queries.empty()) {
				std::cout << "\nQuery processing upto entityHandler"
				<< std::endl;
			}
			for (auto it : queries) {
				std::cout << "Query key: " << it.first << " value : "
				<< it.second << std::endl;
			}

			// If the request type is GET
			if (requestType == "GET") {
				cout << "\t\t\trequestType : GET\n";
				if (isSlowResponse) // Slow response case
				{
					static int startedThread = 0;
					if (!startedThread) {
						std::thread t(handleSlowResponse, (void *) this,
								request);
						startedThread = 1;
						t.detach();
					}
					ehResult = OC_EH_SLOW;
				} else // normal response case.
				{

					pResponse->setResponseResult(OC_EH_OK);
					pResponse->setResourceRepresentation(get());
					if (OC_STACK_OK
							== OCPlatform::sendResponse(pResponse)) {
						ehResult = OC_EH_OK;
					}
				}
			} else if (requestType == "PUT") {
				cout << "\t\t\trequestType : PUT\n";
				OCRepresentation rep = request->getResourceRepresentation();

				// Do related operations related to PUT request
				// Update the lightResource
				put(rep);

				pResponse->setResponseResult(OC_EH_OK);
				pResponse->setResourceRepresentation(get());
				if (OC_STACK_OK == OCPlatform::sendResponse(pResponse)) {
					ehResult = OC_EH_OK;
				}
			} else if (requestType == "POST") {
				cout << "\t\t\trequestType : POST\n";

				OCRepresentation rep = request->getResourceRepresentation();

				// Do related operations related to POST request
				OCRepresentation rep_post = post(rep);
				pResponse->setResourceRepresentation(rep_post);

				if (rep_post.hasAttribute("createduri")) {
					pResponse->setResponseResult(OC_EH_RESOURCE_CREATED);
					pResponse->setNewResourceUri(
							rep_post.getValue < std::string
							> ("createduri"));
				} else {
					pResponse->setResponseResult(OC_EH_OK);
				}

				if (OC_STACK_OK == OCPlatform::sendResponse(pResponse)) {
					ehResult = OC_EH_OK;
				}
			} else if (requestType == "DELETE") {
				cout << "Delete request received" << endl;
			}
		}

		if (requestFlag & RequestHandlerFlag::ObserverFlag) {
			ObservationInfo observationInfo = request->getObservationInfo();
			if (ObserveAction::ObserveRegister == observationInfo.action) {
				m_interestedObservers.push_back(observationInfo.obsId);
			} else if (ObserveAction::ObserveUnregister
					== observationInfo.action) {
				m_interestedObservers.erase(
						std::remove(m_interestedObservers.begin(),
								m_interestedObservers.end(),
								observationInfo.obsId),
						m_interestedObservers.end());
			}

#if defined(_WIN32)
			DWORD threadId = 0;
			HANDLE threadHandle = INVALID_HANDLE_VALUE;
#else
			pthread_t threadId;
#endif

			cout << "\t\trequestFlag : Observer\n";
			gObservation = 1;
			static int startedThread = 0;

			// Observation happens on a different thread in ChangeLightRepresentation function.
			// If we have not created the thread already, we will create one here.
			if (!startedThread) {
#if defined(_WIN32)
				threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ChangeLightRepresentation, (void*)this, 0, &threadId);
#else
				pthread_create(&threadId, NULL, ChangeLightRepresentation,
						(void *) this);
#endif
				startedThread = 1;
			}
			ehResult = OC_EH_OK;
		}
	} else {
		std::cout << "Request invalid" << std::endl;
	}

	return ehResult;
}


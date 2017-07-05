#include "MyboxResource.h"
#include "MyboxCfg.h"
#include <cstdlib>

static void* ChangeLightRepresentation(void *param);
static void* handleSlowResponse(void *param,	std::shared_ptr<OCResourceRequest> pRequest);

static int gObservation = 0;


MyboxResource::MyboxResource():mResHandle(nullptr) {
	mMybox.setState(Mybox::CLOSE);
	mMybox.setName("testbox");

	mRep.setUri(Mybox::URI);

	mRep.setValue("name", mMybox.getName());
	mRep.setValue("state", mMybox.getState());
}

/* Note that this does not need to be a member function: for classes you do not have
 access to, you can accomplish this with a free function: */

/// This function internally calls registerResource API.
void MyboxResource::createResource() {
	//URI of the resource
	std::string resourceURI = Mybox::URI;
	//resource type name. In this case, it is light
	std::string resourceTypeName = Mybox::TYPE;
	// resource interface.
	std::string resourceInterface = DEFAULT_INTERFACE;

	// OCResourceProperty is defined ocstack.h
	uint8_t resourceProperty;
	if (MyBoxConfig::isSecure) {
		resourceProperty = OC_DISCOVERABLE | OC_OBSERVABLE | OC_SECURE;
	} else {
		resourceProperty = OC_DISCOVERABLE | OC_OBSERVABLE;
	}
	EntityHandler cb = std::bind(&MyboxResource::entityHandler, this, PH::_1);


	cout << "register resource.." << endl;
	cout << "\t resourceURI \t\t= " << resourceURI << endl;
	cout << "\t resourceTypeName \t= " << resourceTypeName << endl;
	cout << "\t resourceInterface \t= " << resourceInterface << endl;


	// This will internally create and register the resource.
	OCStackResult result = OCPlatform::registerResource(mResHandle,
			resourceURI, resourceTypeName, resourceInterface, cb,
			resourceProperty);

	if (OC_STACK_OK != result) {
		cout << "Resource creation was unsuccessful\n";
	}
}

OCResourceHandle MyboxResource::getHandle() {
	return mResHandle;
}

// Puts representation.
// Gets values from the representation and
// updates the internal state
void MyboxResource::put(OCRepresentation& rep) {
	cout << "start MyboxResource::put" << endl;
	try {
		if (rep.getValue("state", mMybox.state)) {
			cout << "\t\t\t\t" << "state: " << mMybox.state << endl;
		} else {
			cout << "\t\t\t\t" << "state not found in the representation" << endl;
		}
	} catch (exception& e) {
		cout << e.what() << endl;
	}

}

// Post representation.
// Post can create new resource or simply act like put.
// Gets values from the representation and
// updates the internal state
OCRepresentation MyboxResource::post(OCRepresentation& rep) {
	cout << "start MyboxResource::post" << endl;
	// from second time onwards it just puts
	return get();
}

// gets the updated representation.
// Updates the representation with latest internal state before
// sending out.
OCRepresentation MyboxResource::get() {
	cout << "start MyboxResource::get" << endl;
	mRep.setValue("state", mMybox.state);

	return mRep;
}

void MyboxResource::addType(const std::string& type) const {
	OCStackResult result = OCPlatform::bindTypeToResource(mResHandle, type);
	if (OC_STACK_OK != result) {
		cout << "Binding TypeName to Resource was unsuccessful\n";
	}
}

void MyboxResource::addInterface(const std::string& iface) const {
	OCStackResult result = OCPlatform::bindInterfaceToResource(mResHandle, iface);
	if (OC_STACK_OK != result) {
		cout << "Binding TypeName to Resource was unsuccessful\n";
	}
}

// This is just a sample implementation of entity handler.
// Entity handler can be implemented in several ways by the manufacturer
OCEntityHandlerResult MyboxResource::entityHandler(
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
				std::cout << "\nQuery processing upto entityHandler" 	<< std::endl;
			}
			for (auto it : queries) {
				std::cout << "Query key: " << it.first << " value : " << it.second << std::endl;
			}

			// If the request type is GET
			if (requestType == "GET") {
				cout << "\t\t\trequestType : GET\n";
				if (MyBoxConfig::isSlowResponse) // Slow response case
				{
					static int startedThread = 0;
					if (!startedThread) {
						std::thread t(handleSlowResponse, (void *) this, request);
						startedThread = 1;
						t.detach();
					}
					ehResult = OC_EH_SLOW;
				} else // normal response case.
				{
					pResponse->setResponseResult(OC_EH_OK);
					pResponse->setResourceRepresentation(get());
					if (OC_STACK_OK == OCPlatform::sendResponse(pResponse)) {
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
					pResponse->setNewResourceUri(rep_post.getValue<std::string> ("createduri"));
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
				mObserver.push_back(observationInfo.obsId);
			} else if (ObserveAction::ObserveUnregister == observationInfo.action) {
				mObserver.erase(std::remove(mObserver.begin(),mObserver.end(),observationInfo.obsId),mObserver.end());
			}
			pthread_t threadId;
			cout << "\t\trequestFlag : Observer\n";
			gObservation = 1;
			static int startedThread = 0;
			// Observation happens on a different thread in ChangeLightRepresentation function.
			// If we have not created the thread already, we will create one here.
			if (!startedThread) {
				pthread_create(&threadId, NULL, ChangeLightRepresentation, (void *) this);
				startedThread = 1;
			}
			ehResult = OC_EH_OK;
		}
	} else {
		std::cout << "Request invalid" << std::endl;
	}

	return ehResult;
}



void * handleSlowResponse(void *param,	std::shared_ptr<OCResourceRequest> pRequest) {
	// This function handles slow response case
	MyboxResource* resPtr = (MyboxResource*) param;
	// Induce a case for slow response by using sleep
	std::cout << "SLOW response" << std::endl;
	sleep(10);

	auto pResponse = std::make_shared<OC::OCResourceResponse>();
	pResponse->setRequestHandle(pRequest->getRequestHandle());
	pResponse->setResourceHandle(pRequest->getResourceHandle());
	pResponse->setResourceRepresentation(resPtr->get());

	pResponse->setResponseResult(OC_EH_OK);

	// Set the slow response flag back to false
//	isSlowResponse = false;
	OCPlatform::sendResponse(pResponse);
	return NULL;
}



// ChangeLightRepresentaion is an observation function,
// which notifies any changes to the resource to stack
// via notifyObservers
void *  ChangeLightRepresentation(void *param) {
	MyboxResource* myboxResPtr = (MyboxResource*) param;

	// This function continuously monitors for the changes
	while (1) {
		cout << "test.....in ChangeLightRepresentation....." << endl;
		sleep(3);

		if (gObservation) {
			// If under observation if there are any changes to the light resource
			// we call notifyObservors
			//
			// For demostration we are changing the power value and notifying.
			//myboxResPtr->m_power += 10;

			//cout << "\nPower updated to : " << myboxResPtr->m_power << endl;
			cout << "Notifying observers with resource handle: "
					<< myboxResPtr->getHandle() << endl;

			OCStackResult result = OC_STACK_OK;

			if (MyBoxConfig::isListOfObservers) {
				std::shared_ptr<OCResourceResponse> resourceResponse = {	std::make_shared<OCResourceResponse>()};
				resourceResponse->setResourceRepresentation(myboxResPtr->get(), DEFAULT_INTERFACE);
				result = OCPlatform::notifyListOfObservers( myboxResPtr->getHandle(),myboxResPtr->mObserver,resourceResponse);
			}
			else
			{
				result = OCPlatform::notifyAllObservers(myboxResPtr->getHandle());
			}

			if (OC_STACK_NO_OBSERVERS == result) {
				cout << "No More observers, stopping notifications" << endl;
				gObservation = 0;
				return NULL;
			}
		}
	}

	return NULL;
}


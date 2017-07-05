//******************************************************************
//
// Copyright 2014 Intel Mobile Communications GmbH All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

///
/// This sample provides steps to define an interface for a resource
/// (properties and methods) and host this resource on the server.
///
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#endif
#include <mutex>
#include <condition_variable>

#include "OCPlatform.h"
#include "OCApi.h"
#include "ocpayload.h"

#include "myboxserver.h"
#include "LightResource.h"

using namespace OC;
using namespace std;
namespace PH = std::placeholders;

static const char* SVR_DB_FILE_NAME = "./oic_svr_db_server.dat";

/*
 * Extern Values..
 */
int gObservation = 0;

// Set of strings for each of platform Info fields
std::string platformId = "0A3E0D6F-DBF5-404E-8719-D6880042463A";
std::string manufacturerName = "OCF";
std::string manufacturerLink = "https://www.iotivity.org";
std::string modelNumber = "myModelNumber";
std::string dateOfManufacture = "2016-01-15";
std::string platformVersion = "myPlatformVersion";
std::string operatingSystemVersion = "myOS";
std::string hardwareVersion = "myHardwareVersion";
std::string firmwareVersion = "1.0";
std::string supportLink = "https://www.iotivity.org";
std::string systemTime = "2016-01-15T11.01";

// Set of strings for each of device info fields
std::string deviceName = "IoTivity Simple Server";
std::string deviceType = "oic.wk.tv";
std::string specVersion = "ocf.1.1.0";
std::vector<std::string> dataModelVersions = { "ocf.res.1.1.0", "ocf.sh.1.1.0" };
std::string protocolIndependentID = "fa008167-3bbf-4c9d-8604-c9bcb96cb712";

// OCPlatformInfo Contains all the platform info to be stored
OCPlatformInfo platformInfo;

// Specifies where to notify all observers or list of observers
// false: notifies all observers
// true: notifies list of observers
bool isListOfObservers = false;

// Specifies secure or non-secure
// false: non-secure resource
// true: secure resource
bool isSecure = false;

/// Specifies whether Entity handler is going to do slow response or not
bool isSlowResponse = false;

// ChangeLightRepresentaion is an observation function,
// which notifies any changes to the resource to stack
// via notifyObservers
void * ChangeLightRepresentation(void *param) {
	LightResource* lightPtr = (LightResource*) param;

	// This function continuously monitors for the changes
	while (1) {
		cout << "test.....in ChangeLightRepresentation....." << endl;
		sleep(3);

		if (gObservation) {
			// If under observation if there are any changes to the light resource
			// we call notifyObservors
			//
			// For demostration we are changing the power value and notifying.
			lightPtr->m_power += 10;

			cout << "\nPower updated to : " << lightPtr->m_power << endl;
			cout << "Notifying observers with resource handle: "
					<< lightPtr->getHandle() << endl;

			OCStackResult result = OC_STACK_OK;

			if (isListOfObservers) {
				std::shared_ptr<OCResourceResponse> resourceResponse =
				{	std::make_shared<OCResourceResponse>()};

				resourceResponse->setResourceRepresentation(lightPtr->get(), DEFAULT_INTERFACE);

				result = OCPlatform::notifyListOfObservers( lightPtr->getHandle(),
						lightPtr->m_interestedObservers,
						resourceResponse);
			}
			else
			{
				result = OCPlatform::notifyAllObservers(lightPtr->getHandle());
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

void DeletePlatformInfo() {
	delete[] platformInfo.platformID;
	delete[] platformInfo.manufacturerName;
	delete[] platformInfo.manufacturerUrl;
	delete[] platformInfo.modelNumber;
	delete[] platformInfo.dateOfManufacture;
	delete[] platformInfo.platformVersion;
	delete[] platformInfo.operatingSystemVersion;
	delete[] platformInfo.hardwareVersion;
	delete[] platformInfo.firmwareVersion;
	delete[] platformInfo.supportUrl;
	delete[] platformInfo.systemTime;
}

void DuplicateString(char ** targetString, std::string sourceString) {
	*targetString = new char[sourceString.length() + 1];
	strncpy(*targetString, sourceString.c_str(), (sourceString.length() + 1));
}

OCStackResult SetPlatformInfo(std::string platformID,
		std::string manufacturerName, std::string manufacturerUrl,
		std::string modelNumber, std::string dateOfManufacture,
		std::string platformVersion, std::string operatingSystemVersion,
		std::string hardwareVersion, std::string firmwareVersion,
		std::string supportUrl, std::string systemTime) {
	DuplicateString(&platformInfo.platformID, platformID);
	DuplicateString(&platformInfo.manufacturerName, manufacturerName);
	DuplicateString(&platformInfo.manufacturerUrl, manufacturerUrl);
	DuplicateString(&platformInfo.modelNumber, modelNumber);
	DuplicateString(&platformInfo.dateOfManufacture, dateOfManufacture);
	DuplicateString(&platformInfo.platformVersion, platformVersion);
	DuplicateString(&platformInfo.operatingSystemVersion,
			operatingSystemVersion);
	DuplicateString(&platformInfo.hardwareVersion, hardwareVersion);
	DuplicateString(&platformInfo.firmwareVersion, firmwareVersion);
	DuplicateString(&platformInfo.supportUrl, supportUrl);
	DuplicateString(&platformInfo.systemTime, systemTime);

	return OC_STACK_OK;
}

OCStackResult SetDeviceInfo() {
	OCStackResult result = OC_STACK_ERROR;

	OCResourceHandle handle = OCGetResourceHandleAtUri(OC_RSRVD_DEVICE_URI);
	if (handle == NULL) {
		cout << "Failed to find resource " << OC_RSRVD_DEVICE_URI << endl;
		return result;
	}

	result = OCBindResourceTypeToResource(handle, deviceType.c_str());
	if (result != OC_STACK_OK) {
		cout << "Failed to add device type" << endl;
		return result;
	}

	result = OCPlatform::setPropertyValue(PAYLOAD_TYPE_DEVICE,
			OC_RSRVD_DEVICE_NAME, deviceName);
	if (result != OC_STACK_OK) {
		cout << "Failed to set device name" << endl;
		return result;
	}

	result = OCPlatform::setPropertyValue(PAYLOAD_TYPE_DEVICE,
			OC_RSRVD_DATA_MODEL_VERSION, dataModelVersions);
	if (result != OC_STACK_OK) {
		cout << "Failed to set data model versions" << endl;
		return result;
	}

	result = OCPlatform::setPropertyValue(PAYLOAD_TYPE_DEVICE,
			OC_RSRVD_SPEC_VERSION, specVersion);
	if (result != OC_STACK_OK) {
		cout << "Failed to set spec version" << endl;
		return result;
	}

	result = OCPlatform::setPropertyValue(PAYLOAD_TYPE_DEVICE,
			OC_RSRVD_PROTOCOL_INDEPENDENT_ID, protocolIndependentID);
	if (result != OC_STACK_OK) {
		cout << "Failed to set piid" << endl;
		return result;
	}

	return OC_STACK_OK;
}

void * handleSlowResponse(void *param,
		std::shared_ptr<OCResourceRequest> pRequest) {
	// This function handles slow response case
	LightResource* lightPtr = (LightResource*) param;
	// Induce a case for slow response by using sleep
	std::cout << "SLOW response" << std::endl;
	sleep(10);

	auto pResponse = std::make_shared<OC::OCResourceResponse>();
	pResponse->setRequestHandle(pRequest->getRequestHandle());
	pResponse->setResourceHandle(pRequest->getResourceHandle());
	pResponse->setResourceRepresentation(lightPtr->get());

	pResponse->setResponseResult(OC_EH_OK);

	// Set the slow response flag back to false
	isSlowResponse = false;
	OCPlatform::sendResponse(pResponse);
	return NULL;
}

void PrintUsage() {
	std::cout << "**********************************************" << endl;
	std::cout << "this mybox server" << endl;
	std::cout << "**********************************************" << endl;
	std::cout << "Usage : simpleserver <value>\n";
	std::cout << "    Default - Non-secure resource and notify all observers\n";
	std::cout << "    1 - Non-secure resource and notify list of observers\n\n";
	std::cout << "    2 - Secure resource and notify all observers\n";
	std::cout << "    3 - Secure resource and notify list of observers\n\n";
	std::cout << "    4 - Non-secure resource, GET slow response, notify all observers\n";
}

static FILE* client_open(const char* path, const char* mode) {
	if (strcmp(path, OC_INTROSPECTION_FILE_NAME) == 0) {
		return fopen("light_introspection.json", mode);
	} else {
		return fopen(SVR_DB_FILE_NAME, mode);
	}
}

int chkArguments(int argc, char *argv[]) {
	if (argc == 1) {
		isListOfObservers = false;
		isSecure = false;
	} else if (argc == 2) {
		int value = atoi(argv[1]);
		switch (value) {
		case 1:
			isListOfObservers = true;
			isSecure = false;
			break;
		case 2:
			isListOfObservers = false;
			isSecure = true;
			break;
		case 3:
			isListOfObservers = true;
			isSecure = true;
			break;
		case 4:
			isSlowResponse = true;
			break;
		default:
			break;
		}
	} else {
		return -1;
	}
	return 0;

}

int main(int argc, char* argv[]) {
	system("sh copydat.sh");
	PrintUsage();
	OCPersistentStorage ps { client_open, fread, fwrite, fclose, unlink };
	if (chkArguments(argc, argv) < 0) return -1;
	// Create PlatformConfig object
	PlatformConfig cfg { OC::ServiceType::InProc, OC::ModeType::Server, &ps };

	cfg.transportType = static_cast<OCTransportAdapter>(OCTransportAdapter::OC_ADAPTER_IP
					| OCTransportAdapter::OC_ADAPTER_TCP);
	cfg.QoS = OC::QualityOfService::LowQos;

	OCPlatform::Configure(cfg);
	OC_VERIFY(OCPlatform::start() == OC_STACK_OK);
	std::cout << "Starting server & setting platform info\n";

	OCStackResult result = SetPlatformInfo(platformId, manufacturerName,
			manufacturerLink, modelNumber, dateOfManufacture, platformVersion,
			operatingSystemVersion, hardwareVersion, firmwareVersion,
			supportLink, systemTime);

	result = OCPlatform::registerPlatformInfo(platformInfo);

	if (result != OC_STACK_OK) {
		std::cout << "Platform Registration failed\n";
		return -1;
	}

	result = SetDeviceInfo();

	if (result != OC_STACK_OK) {
		std::cout << "Device Registration failed\n";
		return -1;
	}

	try {
		// Create the instance of the resource class
		// (in this case instance of class 'LightResource').
		LightResource myLight;

		// Invoke createResource function of class light.
		myLight.createResource();
		std::cout << "Created resource." << std::endl;

		myLight.addType(std::string("core.brightlight"));
		myLight.addInterface(std::string(LINK_INTERFACE));
		std::cout << "Added Interface and Type" << std::endl;

		DeletePlatformInfo();

		// A condition variable will free the mutex it is given, then do a non-
		// intensive block until 'notify' is called on it.  In this case, since we
		// don't ever call cv.notify, this should be a non-processor intensive version
		// of while(true);
		std::mutex blocker;
		std::condition_variable cv;
		std::unique_lock < std::mutex > lock(blocker);
		std::cout << "Waiting" << std::endl;
		cv.wait(lock, [] {return false;});
	} catch (OCException &e) {
		std::cout << "OCException in main : " << e.what() << endl;
	}

	OC_VERIFY(OCPlatform::stop() == OC_STACK_OK);

	return 0;
}

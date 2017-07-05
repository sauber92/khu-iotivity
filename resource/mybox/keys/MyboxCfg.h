#ifndef MY_BOX_CFG_H_
#define MY_BOX_CFG_H_

namespace MyBoxConfig{
	const int OPEN = 1;
	const int COLSE = 0;
	// Set of strings for each of platform Info fields
	const std::string platformId = "0A3E0D6F-DBF5-404E-8719-D6880042463A";
	const std::string manufacturerName = "OCF";
	const std::string manufacturerLink = "https://www.iotivity.org";
	const std::string modelNumber = "myModelNumber";
	const std::string dateOfManufacture = "2016-01-15";
	const std::string platformVersion = "myPlatformVersion";
	const std::string operatingSystemVersion = "myOS";
	const std::string hardwareVersion = "myHardwareVersion";
	const std::string firmwareVersion = "1.0";
	const std::string supportLink = "https://www.iotivity.org";
	const std::string systemTime = "2016-01-15T11.01";

	// Set of strings for each of device info fields
	const std::string deviceName = "IoTivity Simple Server";
	const std::string deviceType = "oic.wk.tv";
	const std::string specVersion = "ocf.1.1.0";
	const std::vector<std::string> dataModelVersions = { "ocf.res.1.1.0", "ocf.sh.1.1.0" };
	const std::string protocolIndependentID = "fa008167-3bbf-4c9d-8604-c9bcb96cb712";

	const std::string MyboxPassword = "";
	const std::string PublicKeyPath = "";




	/*
	std::cout << "Usage : simpleserver <value>\n";
	std::cout << "    Default - Non-secure resource and notify all observers\n";
	std::cout << "    1 - Non-secure resource and notify list of observers\n\n";
	std::cout << "    2 - Secure resource and notify all observers\n";
	std::cout << "    3 - Secure resource and notify list of observers\n\n";
	std::cout << "    4 - Non-secure resource, GET slow response, notify all observers\n";
	 */


	// Specifies where to notify all observers or list of observers
	// false: notifies all observers
	// true: notifies list of observers
	const bool isListOfObservers = false;

	// Specifies secure or non-secure
	// false: non-secure resource
	// true: secure resource
	const bool isSecure = false;

	/// Specifies whether Entity handler is going to do slow response or not
	const bool isSlowResponse = false;

}




#endif


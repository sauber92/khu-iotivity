#pragma once
#ifndef MY_BOX_SERVER_H_
#define MY_BOX_SERVER_H_

#include "OCPlatform.h"
#include "OCApi.h"
#include "ocpayload.h"
using namespace OC;
using namespace std;
namespace PH = std::placeholders;

void * ChangeLightRepresentation (void *param);
void * handleSlowResponse (void *param, std::shared_ptr<OCResourceRequest> pRequest);

extern int gObservation;

// Set of strings for each of platform Info fields
extern std::string  platformId;
extern std::string  manufacturerName;
extern std::string  manufacturerLink;
extern std::string  modelNumber;
extern std::string  dateOfManufacture;
extern std::string  platformVersion;
extern std::string  operatingSystemVersion;
extern std::string  hardwareVersion;
extern std::string  firmwareVersion;
extern std::string  supportLink;
extern std::string  systemTime;

// Set of strings for each of device info fields
extern std::string  deviceName;
extern std::string  deviceType;
extern std::string  specVersion;
extern std::vector<std::string> dataModelVersions;
extern std::string  protocolIndependentID;

// OCPlatformInfo Contains all the platform info to be stored
extern OCPlatformInfo platformInfo;

// Specifies where to notify all observers or list of observers
// false: notifies all observers
// true: notifies list of observers
extern bool isListOfObservers;

// Specifies secure or non-secure
// false: non-secure resource
// true: secure resource
extern bool isSecure;

/// Specifies whether Entity handler is going to do slow response or not
extern bool isSlowResponse;

#endif //LIGHT_RESOURCE_H_

/* *****************************************************************
 *
 * Copyright 2017 Microsoft
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#include "oic_time.h"
#include "ipcainternal.h"
#include <map>
#include <algorithm>
#include <stdexcept>

#define TAG "IPCA_Callback"

extern OCFFramework ocfFramework;

Callback::Callback(App* app) :
    m_nextKey(0),
    m_app(app),
    m_stopCalled(false),
    m_expiredCallbacksInprogress(0)
{
}

Callback::~Callback()
{
}

// Exception when Stop() times out waiting for pending callback to complete.
class StopTimeout : public std::exception
{
  virtual const char* what() const throw()
  {
    return "Time out waiting for callback to complete in Callback::Stop().";
  }
} timeoutException;

void Callback::Stop()
{
    bool allStopped = false;

    // No more callbacks from here on as SetCallbackInProgress() and
    // CompleteAndRemoveExpiredCallbackInfo() check for this flag.
    m_stopCalled = true;

    // Wait some amount of time for all callbacks in progress to complete.
    const int WaitTimeSeconds = 3;
    int i = 0;
    while (i < WaitTimeSeconds)
    {
        if (m_callbackInfoList.size() != 0)
        {
            std::lock_guard<std::mutex> lock(m_callbackMutex);
            for (auto it = m_callbackInfoList.cbegin();
                 it != m_callbackInfoList.cend();
                 /* increment inside loop */)
            {
                if (it->second->callbackInProgressCount == 0)
                {
                    m_callbackInfoList.erase(it++);
                }
                else
                {
                    ++it;
                }
            }
        }

        // There are 2 group of callbacks.
        // One tracked by m_callbackInfoList and the other tracked by m_expiredCallbacksInprogress.
        if ((m_callbackInfoList.size() == 0) && (m_expiredCallbacksInprogress == 0))
        {
            allStopped = true;
            break;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
        i++;
    }

    if (allStopped == false)
    {
        OIC_LOG_V(WARNING, TAG, "Stop() time out waiting for pending callbacks to complete.");
        throw timeoutException;
    }

}

CallbackInfo::Ptr Callback::CreatePasswordCallbackInfo(
                                CallbackType cbType,
                                const void* context,
                                IPCAProvidePasswordCallback passwordInputCb,
                                IPCADisplayPasswordCallback passwordDisplayCb)
{
    if ((cbType != CallbackType_PasswordInputCallback) &&
        (cbType != CallbackType_PasswordDisplayCallback))
    {
        assert(false);
        return nullptr;
    }

    CallbackInfo::Ptr cbInfo = std::shared_ptr<CallbackInfo>(new(CallbackInfo));

    if (cbInfo == nullptr)
    {
        return nullptr;
    }

    cbInfo->app = m_app;
    cbInfo->type = cbType;
    cbInfo->callbackContext = context;
    cbInfo->callbackInProgressCount = 0;
    cbInfo->markedToBeRemoved = false;

    switch (cbType)
    {
        case CallbackType_PasswordInputCallback:
            cbInfo->passwordInputCallback = passwordInputCb;
            break;

        case CallbackType_PasswordDisplayCallback:
            cbInfo->passwordDisplayCallback = passwordDisplayCb;
            break;
    }

    return cbInfo;
}

CallbackInfo::Ptr Callback::CreateRequestAccessCompletionCallbackInfo(
                                DevicePtr device,
                                const void* context,
                                const char* resourcePath,
                                IPCARequestAccessCompletionCallback completionCallback)
{
    CallbackInfo::Ptr cbInfo = std::shared_ptr<CallbackInfo>(new(CallbackInfo));

    if (cbInfo == nullptr)
    {
        return nullptr;
    }

    cbInfo->app = m_app;
    cbInfo->device = device;
    cbInfo->type = Callbacktype_RequestAccessCompletionCallback;
    cbInfo->callbackContext = context;
    cbInfo->callbackInProgressCount = 0;
    cbInfo->markedToBeRemoved = false;
    cbInfo->requestAccessCompletionCallback = completionCallback;

    if (resourcePath != nullptr)
    {
        cbInfo->resourcePath = resourcePath;
    }

    return cbInfo;
}

CallbackInfo::Ptr Callback::CreateCallbackInfo(
                                DevicePtr device,
                                CallbackType cbType,
                                const void* context,
                                IPCADiscoverDeviceCallback discoverDeviceCallback,
                                const char* const* resourceTypeList,
                                int resourceTypeCount,
                                GenericAppCallback appCallback,
                                const char* resourcePath,
                                const char* resourceInterface,
                                const char* resourceType)
{
    CallbackInfo::Ptr cbInfo = std::shared_ptr<CallbackInfo>(new(CallbackInfo));

    if (cbInfo == nullptr)
    {
        return nullptr;
    }

    cbInfo->app = m_app;
    cbInfo->device = device;
    cbInfo->type = cbType;
    cbInfo->callbackContext = context;
    cbInfo->callbackInProgressCount = 0;
    cbInfo->markedToBeRemoved = false;
    cbInfo->requestSentTimestamp = 0;

    cbInfo->resourcePath = std::string(resourcePath ? resourcePath : "");
    cbInfo->resourceInterface = std::string(resourceInterface ? resourceInterface : "");
    cbInfo->resourceType = std::string(resourceType ? resourceType : "");

    switch (cbType)
    {
        case CallbackType_Discovery:
        {
            cbInfo->discoveryCallback = discoverDeviceCallback;
            int i = 0;
            while (i < resourceTypeCount)
            {
                cbInfo->resourceTypeList.push_back(resourceTypeList[i++]);
            }
            break;
        }

        case CallbackType_ResourceChange:
        {
            cbInfo->resourceChangeCallback= appCallback;
            break;
        }

        case CallbackType_GetPropertiesComplete:
        {
            cbInfo->getCallback = appCallback;
            break;
        }

        case CallbackType_SetPropertiesComplete:
        {
            cbInfo->setCallback = appCallback;
            break;
        }

        case CallbackType_CreateResourceComplete:
        {
            cbInfo->createResourceCallback=
                        reinterpret_cast<IPCACreateResourceComplete>(appCallback);
            break;
        }

        case CallbackType_DeleteResourceComplete:
        {
            cbInfo->deleteResourceCallback=
                        reinterpret_cast<IPCADeleteResourceComplete>(appCallback);
            break;
        }

        default:
        {
            // Must be new CallbackType definition that needs new code.
            assert(false);
            break;
        }
    }

    return cbInfo;
}

IPCAStatus Callback::AddCallbackInfo(CallbackInfo::Ptr cbInfo)
{
    std::lock_guard<std::mutex> lock(m_callbackMutex);

    // App has called IPCAClose().  Stop taking CallbackInfo.
    if (m_stopCalled == true)
    {
        return IPCA_FAIL;
    }

    uint32_t i = 0;
    while (i++ < UINT32_MAX)
    {
        uint32_t newKey = m_nextKey++;
        if (m_callbackInfoList.find(newKey) == m_callbackInfoList.end())
        {
            OIC_LOG_V(INFO, TAG, "AddCallbackInfo() with key: %d", newKey);
            cbInfo->mapKey = newKey;
            m_callbackInfoList[newKey] = cbInfo;
            return IPCA_OK;
        }
    }

    // All map entries are filled.  It's a large table (32 bit)!
    return IPCA_OUT_OF_MEMORY;
}

CallbackInfo::Ptr Callback::GetCallbackInfo(size_t mapKey)
{
    std::lock_guard<std::mutex> lock(m_callbackMutex);

    // App has called IPCAClose().
    if (m_stopCalled == true)
    {
        return nullptr;
    }

    if (m_callbackInfoList.find(mapKey) !=  m_callbackInfoList.end())
    {
        return m_callbackInfoList[mapKey];
    }
    else
    {
        return nullptr;
    }
}

CallbackInfo::Ptr Callback::GetPasswordInputCallbackInfo()
{
    std::lock_guard<std::mutex> lock(m_callbackMutex);

    // App has called IPCAClose().
    if (m_stopCalled == true)
    {
        return nullptr;
    }

    // Go through the CallbackInfo list to find the PasswordInputCallback CallbackInfo.
    // PasswordInputCallback is only registered once by the app so there can only be one result.
    for (auto const& entry : m_callbackInfoList)
    {
        if (entry.second->type == CallbackType_PasswordInputCallback)
        {
            return entry.second;
        }
    }

    return nullptr;
}

bool Callback::SetCallbackInProgress(size_t mapKey)
{
    std::lock_guard<std::mutex> lock(m_callbackMutex);

    // App has called IPCAClose().   Pending callbacks are automatically cleared.
    if (m_stopCalled == true)
    {
        return false;
    }

    if (m_callbackInfoList.find(mapKey) !=  m_callbackInfoList.end())
    {
        CallbackInfo::Ptr callbackInfo = m_callbackInfoList[mapKey];

        // This callback is marked for removal.
        if (callbackInfo->markedToBeRemoved)
        {
            return false;
        }

        // Indicate that callback is in progress.
        callbackInfo->callbackInProgressCount++;
        return true;
    }
    else
    {
        // Callback handle is already closed by app.
        return false;
    }
}

bool Callback::ClearCallbackInProgress(size_t mapKey)
{
    std::lock_guard<std::mutex> lock(m_callbackMutex);

    // This function does not check m_stopCalled, because any call already in progress
    // must complete.
    if (m_callbackInfoList.find(mapKey) !=  m_callbackInfoList.end())
    {
        m_callbackInfoList[mapKey]->callbackInProgressCount--;
        return true;
    }

    OIC_LOG_V(INFO, TAG, "ClearCallbackInProgress() mapKey [%d] is not found", mapKey);
    assert(false); // In progress callback is not expected to be removed from the list.
    return false;
}

void Callback::RemoveCallbackInfo(size_t mapKey)
{
    std::lock_guard<std::mutex> lock(m_callbackMutex);

    if (m_callbackInfoList.find(mapKey) !=  m_callbackInfoList.end())
    {
        if (m_callbackInfoList[mapKey]->callbackInProgressCount == 0)
        {
            m_callbackInfoList.erase(mapKey);
        }
        else
        {
            m_callbackInfoList[mapKey]->markedToBeRemoved = true;
        }
    }
}

void Callback::CompleteAndRemoveExpiredCallbackInfo(std::vector<CallbackInfo::Ptr>& cbInfoList)
{
    // @tbd: determine a good value for response timeout.
    const int RequestTimeoutMs = 2000;    // 2 seconds for request timeout.

    uint64_t currentTime = OICGetCurrentTime(TIME_IN_MS);

    {
        std::lock_guard<std::mutex> lock(m_callbackMutex);

        if (m_stopCalled == true)
        {
            return;
        }

        // Collect the expired requests.
        for(auto const& entry : m_callbackInfoList)
        {
            // Opportunistic removal of callback that couldn't be removed during
            // RemoveCallbackInfo().
            if (entry.second->markedToBeRemoved == true)
            {
                cbInfoList.push_back(entry.second);
                continue;
            }

            // Look for outstanding get, set, create requests that have been sent
            // (time stamp is not 0), have timed out and not already in the middle of callback.
            if ((entry.second->requestSentTimestamp != 0) &&
                (entry.second->callbackInProgressCount == 0) &&
                ((entry.second->type == CallbackType_GetPropertiesComplete)  ||
                 (entry.second->type == CallbackType_SetPropertiesComplete)  ||
                 (entry.second->type == CallbackType_CreateResourceComplete) ||
                 (entry.second->type == CallbackType_DeleteResourceComplete)))
            {
                if ((currentTime - entry.second->requestSentTimestamp) > RequestTimeoutMs)
                {
                    m_expiredCallbacksInprogress++;
                    cbInfoList.push_back(entry.second);
                }
            }
        }

        // Remove them from the list.
        for (auto const& entry : cbInfoList)
        {
            m_callbackInfoList.erase(entry->mapKey);
        }
    }

    // Complete the callback for each.
    for (auto const& cbInfo : cbInfoList)
    {
        if (cbInfo->getCallback != nullptr)
        {
            std::thread thrd;
            switch(cbInfo->type)
            {
                case CallbackType_GetPropertiesComplete:
                    thrd = std::thread(cbInfo->getCallback,
                                IPCA_REQUEST_TIMEOUT,
                                const_cast<void*>(cbInfo->callbackContext),
                                nullptr);
                    thrd.detach();
                    break;

                case CallbackType_SetPropertiesComplete:
                    thrd = std::thread(cbInfo->setCallback,
                                IPCA_REQUEST_TIMEOUT,
                                const_cast<void*>(cbInfo->callbackContext),
                                nullptr);
                    thrd.detach();
                    break;

                case CallbackType_CreateResourceComplete:
                    thrd = std::thread(cbInfo->createResourceCallback,
                                IPCA_REQUEST_TIMEOUT,
                                const_cast<void*>(cbInfo->callbackContext),
                                nullptr,
                                nullptr);
                    thrd.detach();
                    break;

                case CallbackType_DeleteResourceComplete:
                    thrd = std::thread(cbInfo->deleteResourceCallback,
                                IPCA_REQUEST_TIMEOUT,
                                const_cast<void*>(cbInfo->callbackContext));
                    thrd.detach();
                    break;

                default:
                    assert(false); // check the filter code above to match the handling here.
                    break;
            }
        }

        {
            std::lock_guard<std::mutex> lock(m_callbackMutex);
            m_expiredCallbacksInprogress--;
        }
    }

}

void Callback::GetCallbackInfoList(CallbackType type, std::vector<CallbackInfo::Ptr>& filteredList)
{
    std::lock_guard<std::mutex> lock(m_callbackMutex);

    for(auto const& entry : m_callbackInfoList)
    {
        if (entry.second->type == type)
        {
            filteredList.push_back(entry.second);
        }
    }
}

bool Callback::MatchAllRequiredResourceTypes(
                    const std::vector<std::string>& requiredResourceTypes,
                    const std::vector<std::string>& deviceResourceTypes)
{
    for (auto const& targetResourceType : requiredResourceTypes)
    {
        if (targetResourceType.length() == 0)
        {
            OIC_LOG_V(INFO, TAG,
                "MatchAllRequiredResourceTypes(): app has nullptr target function. returning true");
            return true;
        }

        if (std::find(deviceResourceTypes.begin(),
                      deviceResourceTypes.end(),
                      targetResourceType) == deviceResourceTypes.end())
        {
            OIC_LOG_V(INFO, TAG,
                "MatchAllRequiredResourceTypes(): return false. Resource Type not found: [%s]",
                targetResourceType.c_str());
            return false;
        }
    }

    OIC_LOG_V(INFO, TAG, "MatchAllRequiredResourceTypes(): returning true. ");
    return true;
}

void Callback::DeviceDiscoveryCallback(
                    bool deviceResponding,
                    bool newInfoLearntAboutDevice,
                    const InternalDeviceInfo& deviceInfo,
                    const std::vector<std::string>& deviceResourceTypeList)
{
    // Create IPCADiscoveredDeviceInfo object for callback.
    IPCADiscoveredDeviceInfo  deviceInfoUsedForCallback = {0};

    if (deviceInfo.deviceUris.size() != 0)
    {
        deviceInfoUsedForCallback.deviceUris =
            static_cast<const char**>(OICCalloc(deviceInfo.deviceUris.size(), sizeof(char*)));

        if (deviceInfoUsedForCallback.deviceUris == nullptr)
        {
            OIC_LOG_V(WARNING, TAG, "Callback::DeviceDiscoveryCallback: out of memory.");
            return;
        }

        int i = 0;
        for (auto const& deviceUri : deviceInfo.deviceUris)
        {
            deviceInfoUsedForCallback.deviceUris[i++] = deviceUri.c_str();
        }
    }

    deviceInfoUsedForCallback.deviceId = deviceInfo.deviceId.c_str();
    deviceInfoUsedForCallback.deviceName = deviceInfo.deviceName.c_str();
    deviceInfoUsedForCallback.deviceUriCount = deviceInfo.deviceUris.size();

    // 2 groups of callbacks are interested in device discovery status.
    // One is outstanding Observe requests.  The other is outstanding IPCADiscoveryDevices()
    // requests.

    // Outstanding observe requests are interested in IPCA_DEVICE_APPEAR_OFFLINE.
    if (false == deviceResponding)
    {
        // Call Observe callback with IPCA_DEVICE_APPEAR_OFFLINE if this is the device whose
        // resource it is observing.
        std::vector<CallbackInfo::Ptr> observeCallbackinfoList;
        GetCallbackInfoList(CallbackType_ResourceChange, observeCallbackinfoList);

        for(auto const& cbInfo : observeCallbackinfoList)
        {
            // if device id matches and not in shut down already
            if ((cbInfo->device->GetDeviceId().compare(deviceInfo.deviceId) == 0) &&
                (SetCallbackInProgress(cbInfo->mapKey) == true))
            {
                std::thread exec(
                        cbInfo->resourceChangeCallback,
                        IPCA_DEVICE_APPEAR_OFFLINE,
                        const_cast<void*>(cbInfo->callbackContext),
                        nullptr);
                exec.detach();
                ClearCallbackInProgress(cbInfo->mapKey);
            }
        }
    }

    // IPCADiscoverDevices() requests are interested in IPCADeviceStatus callback.
    std::vector<CallbackInfo::Ptr> discoveryCallbackInfoList;
    GetCallbackInfoList(CallbackType_Discovery, discoveryCallbackInfoList);

    // Synchronize discovery callback to app to ensure IPCA_DEVICE_DISCOVERED is always first.
    m_discoverDeviceCallbackMutex.lock();

    // Callback if it matches resource types requested by app in IPCADiscoverDevices().
    for(auto const& cbInfo : discoveryCallbackInfoList)
    {
        if (MatchAllRequiredResourceTypes(cbInfo->resourceTypeList, deviceResourceTypeList))
        {
            // Indicate that callback is in progress and call the app's callback.
            if (SetCallbackInProgress(cbInfo->mapKey) == true)
            {
                m_callbackMutex.lock();
                auto it = std::find(cbInfo->discoveredDevicesList.begin(),
                                    cbInfo->discoveredDevicesList.end(),
                                    deviceInfo.deviceId);

                if (it == cbInfo->discoveredDevicesList.end())
                {
                    // App has never received IPCA_DEVICE_DISCOVERED for this device.
                    if (deviceResponding) // New device for this callback if device is responding,
                                          // otherwise it's a nop.
                    {
                        // Track that app receives IPCA_DEVICE_DISCOVERED callback for this device.
                        cbInfo->discoveredDevicesList.push_back(deviceInfo.deviceId);

                        // Release lock to call out to app.
                        m_callbackMutex.unlock();
                        (cbInfo->discoveryCallback)(const_cast<void*>(cbInfo->callbackContext),
                                    IPCA_DEVICE_DISCOVERED, &deviceInfoUsedForCallback);
                        m_callbackMutex.lock();
                    }
                }
                else
                {
                    // App has received IPCA_DEVICE_DISCOVERED for this device in the past.
                    if (deviceResponding)
                    {
                        // Callback to app if there's new info.
                        if (newInfoLearntAboutDevice == true)
                        {
                            m_callbackMutex.unlock();
                            (cbInfo->discoveryCallback)(const_cast<void*>(cbInfo->callbackContext),
                                    IPCA_DEVICE_UPDATED_INFO, &deviceInfoUsedForCallback);
                            m_callbackMutex.lock();
                        }
                    }
                    else
                    {
                        // Remove the device from  discovered device list of this callback.
                        // Next time the same device shows up, this callback will get
                        // IPCA_DEVICE_DISCOVERED.
                        cbInfo->discoveredDevicesList.erase(it);

                        m_callbackMutex.unlock();
                        (cbInfo->discoveryCallback)(const_cast<void*>(cbInfo->callbackContext),
                                    IPCA_DEVICE_STOPPED_RESPONDING, &deviceInfoUsedForCallback);
                        m_callbackMutex.lock();
                    }
                }
                m_callbackMutex.unlock();

                ClearCallbackInProgress(cbInfo->mapKey);
            }
        }
    }

    m_discoverDeviceCallbackMutex.unlock();

    if (deviceInfoUsedForCallback.deviceUris != nullptr)
    {
        OICFree(deviceInfoUsedForCallback.deviceUris);
        deviceInfoUsedForCallback.deviceUris = nullptr;
    }
}

void Callback::GetCallback(IPCAStatus status, const OCRepresentation& rep, CallbackInfo::Ptr cbInfo)
{
    // Check if cbInfo is for this app and mark that callback in progress.
    if ((cbInfo->app != m_app) || (SetCallbackInProgress(cbInfo->mapKey) == false))
    {
        return;
    }

    if (cbInfo->getCallback != nullptr)
    {
        cbInfo->getCallback(
                    status,
                    const_cast<void*>(cbInfo->callbackContext),
                    (IPCAPropertyBagHandle)&rep);
    }

    ClearCallbackInProgress(cbInfo->mapKey);
    RemoveCallbackInfo(cbInfo->mapKey);
}

void Callback::SetCallback(IPCAStatus status, const OCRepresentation& rep, CallbackInfo::Ptr cbInfo)
{
    if ((cbInfo->app != m_app) || (SetCallbackInProgress(cbInfo->mapKey) == false))
    {
        return;
    }

    if (cbInfo->type == CallbackType_CreateResourceComplete)
    {
        cbInfo->createResourceCallback(
                    status,
                    const_cast<void*>(cbInfo->callbackContext),
                    NULL, /* tbd: no info on new resource URI. */
                          /* See https://jira.iotivity.org/browse/IOT-1819 */
                    (IPCAPropertyBagHandle)&rep);
    }
    else
    {
        cbInfo->setCallback(
                    status,
                    const_cast<void*>(cbInfo->callbackContext),
                    (IPCAPropertyBagHandle)&rep);
    }

    ClearCallbackInProgress(cbInfo->mapKey);
    RemoveCallbackInfo(cbInfo->mapKey);
}

void Callback::ObserveCallback(IPCAStatus status,
                const OCRepresentation& rep,
                CallbackInfo::Ptr cbInfo)
{
    if ((cbInfo->app != m_app) || (SetCallbackInProgress(cbInfo->mapKey) == false))
    {
        return;
    }

    cbInfo->resourceChangeCallback(
                status,
                const_cast<void*>(cbInfo->callbackContext),
                (IPCAPropertyBagHandle)&rep);

    ClearCallbackInProgress(cbInfo->mapKey);
}

void Callback::DeleteResourceCallback(IPCAStatus status, CallbackInfo::Ptr cbInfo)
{
    if ((cbInfo->app != m_app) || (SetCallbackInProgress(cbInfo->mapKey) == false))
    {
        return;
    }

    cbInfo->deleteResourceCallback(
                status,
                const_cast<void*>(cbInfo->callbackContext));

    ClearCallbackInProgress(cbInfo->mapKey);
}

void Callback::PasswordInputCallback(std::string deviceId,
                    IPCAOwnershipTransferType type,
                    char* passwordBuffer,
                    size_t passwordBufferSize,
                    CallbackInfo::Ptr cbInfo)
{
    IPCADeviceInfo* deviceInfo = nullptr;
    IPCAPlatformInfo* platformInfo = nullptr;

    if ((cbInfo->app != m_app) || (SetCallbackInProgress(cbInfo->mapKey) == false))
    {
        return;
    }

    IPCAStatus status = ocfFramework.CopyDeviceInfo(deviceId, &deviceInfo);
    if (IPCA_OK == status)
    {
        status = ocfFramework.CopyPlatformInfo(deviceId, &platformInfo);
        if (IPCA_OK != status)
        {
            OIC_LOG_V(INFO, TAG,
                "Callback::PasswordInfoCallback: Failed to retrieve platform information.");
        }
    }
    else
    {
        OIC_LOG_V(INFO, TAG,
                "Callback::PasswordInfoCallback: Failed to retrieve device information.");
    }

    if (cbInfo->passwordInputCallback != nullptr)
    {
        cbInfo->passwordInputCallback(
                    const_cast<void*>(cbInfo->callbackContext),
                    deviceInfo,
                    platformInfo,
                    type,
                    passwordBuffer,
                    passwordBufferSize);
    }

    ClearCallbackInProgress(cbInfo->mapKey);

    if (deviceInfo != nullptr)
    {
        OCFFramework::FreeDeviceInfo(deviceInfo);
        deviceInfo = nullptr;
    }

    if (platformInfo != nullptr)
    {
        OCFFramework::FreePlatformInfo(platformInfo);
        platformInfo = nullptr;
    }
}

void Callback::PasswordDisplayCallback(
                    std::string deviceId,
                    IPCAOwnershipTransferType type,
                    const char* passwordBuffer,
                    CallbackInfo::Ptr cbInfo)
{
    if ((cbInfo->app != m_app) || (SetCallbackInProgress(cbInfo->mapKey) == false))
    {
        return;
    }

    if (cbInfo->passwordDisplayCallback != nullptr)
    {
        cbInfo->passwordDisplayCallback(
                    const_cast<void*>(cbInfo->callbackContext),
                    nullptr,
                    nullptr,
                    type,
                    passwordBuffer);
    }

    ClearCallbackInProgress(cbInfo->mapKey);
}

void Callback::RequestAccessCompletionCallback(IPCAStatus status, CallbackInfo::Ptr cbInfo)
{
    if ((cbInfo->app != m_app) || (SetCallbackInProgress(cbInfo->mapKey) == false))
    {
        return;
    }

    if (cbInfo->requestAccessCompletionCallback != nullptr)
    {
        cbInfo->requestAccessCompletionCallback(status, const_cast<void*>(cbInfo->callbackContext));
    }

    ClearCallbackInProgress(cbInfo->mapKey);
}

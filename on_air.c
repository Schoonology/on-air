#import <CoreMediaIO/CMIOHardware.h>
#import <CoreMediaIO/CMIOHardwareObject.h>
#import <CoreMediaIO/CMIOHardwareSystem.h>

CMIODeviceID *queryDevices(uint32_t *count) {
    CMIOObjectPropertyAddress address = {
        kCMIOHardwarePropertyDevices,
        kCMIOObjectPropertyScopeGlobal,
        kCMIOObjectPropertyElementMaster
    };

    uint32_t dataSize = 0;
    uint32_t dataUsed = 0;

    OSStatus result = CMIOObjectGetPropertyDataSize(kCMIOObjectSystemObject, &address, 0, NULL, &dataSize);
    CMIOObjectID *devices = NULL;
    do {
        if (devices != NULL) {
            free(devices);
            devices = NULL;
        }
        devices = malloc(dataSize);
        CMIOObjectGetPropertyData(kCMIOObjectSystemObject, &address, 0, NULL, dataSize, &dataUsed, devices);
    } while (result == kCMIOHardwareBadPropertySizeError);

    *count = dataUsed / sizeof(CMIODeviceID);
    return devices;
}

UInt8 queryDeviceInUse(CMIOObjectID device) {
    CMIOObjectPropertyAddress address = {
        kCMIODevicePropertyDeviceIsRunningSomewhere,
        kCMIOObjectPropertyScopeWildcard,
        kCMIOObjectPropertyElementWildcard
    };

    uint32_t dataSize = 0;
    uint32_t dataUsed = 0;

    OSStatus result = CMIOObjectGetPropertyDataSize(device, &address, 0, nil, &dataSize);
    UInt8 data = 0;
    if (result == kCMIOHardwareNoError) {
        result = CMIOObjectGetPropertyData(device, &address, 0, nil, dataSize, &dataUsed, &data);
    }

    return data;
}

int main(int argc, const char * argv[]) {
    uint32_t deviceIDCount;
    CMIODeviceID *deviceIDs = queryDevices(&deviceIDCount);
    uint8_t result = 0;

    for (uint32_t i = 0; i < deviceIDCount; i++) {
        result = result | queryDeviceInUse(deviceIDs[i]);
    }

    printf("%i\n", result);

    return 0;
}

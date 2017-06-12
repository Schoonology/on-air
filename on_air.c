#import <CoreAudio/CoreAudio.h>
#import <CoreMediaIO/CMIOHardware.h>
#import <CoreMediaIO/CMIOHardwareObject.h>
#import <CoreMediaIO/CMIOHardwareSystem.h>

uint8_t checkResult(OSStatus result) {
    if (result == noErr) {
        return 0;
    }

    char message[20];
    *(UInt32 *)(message + 1) = CFSwapInt32HostToBig(result);
    if (isprint(message[1]) && isprint(message[2]) && isprint(message[3]) && isprint(message[4])) {
        message[0] = message[5] = '\'';
        message[6] = '\0';
    } else {
        sprintf(message, "%d", (int)result);
    }

    fprintf(stderr, "Error: %s\n", message);

    return 1;
}

AudioDeviceID *queryAudioDevices(uint32_t *count) {
    AudioObjectPropertyAddress address = {
        kAudioHardwarePropertyDevices,
        kAudioObjectPropertyScopeGlobal,
        kAudioObjectPropertyElementMaster
    };

    uint32_t dataSize = 0;

    OSStatus result = AudioObjectGetPropertyDataSize(kAudioObjectSystemObject, &address, 0, NULL, &dataSize);
    if (checkResult(result)) {
        *count = 0;
        return NULL;
    }

    *count = dataSize / sizeof(AudioDeviceID);
    AudioDeviceID *devices = (AudioDeviceID *)malloc(dataSize);
    result = AudioObjectGetPropertyData(kAudioObjectSystemObject, &address, 0, NULL, &dataSize, devices);
    if (checkResult(result)) {
        *count = 0;
        return NULL;
    }

    return devices;
}

UInt8 queryAudioDeviceInUse(AudioDeviceID device) {
    AudioObjectPropertyAddress address = {
        kAudioDevicePropertyDeviceIsRunningSomewhere,
        kAudioObjectPropertyScopeInput,
        kAudioObjectPropertyElementMaster
    };

    uint32_t dataSize = 0;

    OSStatus result = AudioObjectGetPropertyDataSize(device, &address, 0, NULL, &dataSize);
    if (checkResult(result)) {
        return 0;
    }

    UInt8 data = 0;
    result = AudioObjectGetPropertyData(device, &address, 0, NULL, &dataSize, &data);
    if (checkResult(result)) {
        return 0;
    }

    return data;
}

CMIODeviceID *queryVideoDevices(uint32_t *count) {
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

UInt8 queryVideoDeviceInUse(CMIOObjectID device) {
    CMIOObjectPropertyAddress address = {
        kCMIODevicePropertyDeviceIsRunningSomewhere,
        kCMIOObjectPropertyScopeWildcard,
        kCMIOObjectPropertyElementWildcard
    };

    uint32_t dataSize = 0;
    uint32_t dataUsed = 0;

    OSStatus result = CMIOObjectGetPropertyDataSize(device, &address, 0, NULL, &dataSize);
    UInt8 data = 0;
    if (checkResult(result)) {
        return 0;
    }

    result = CMIOObjectGetPropertyData(device, &address, 0, NULL, dataSize, &dataUsed, &data);
    if (checkResult(result)) {
        return 0;
    }

    return data;
}

int main(int argc, const char * argv[]) {
    uint32_t deviceIDCount;
    CMIODeviceID *videoDeviceIDs = queryVideoDevices(&deviceIDCount);
    uint8_t result = 0;

    for (uint32_t i = 0; i < deviceIDCount; i++) {
        result = result | queryVideoDeviceInUse(videoDeviceIDs[i]);
    }

    AudioDeviceID *audioDeviceIDs = queryAudioDevices(&deviceIDCount);

    for (uint32_t i = 0; i < deviceIDCount; i++) {
        result = result | queryAudioDeviceInUse(audioDeviceIDs[i]);
    }

    printf("%i\n", result);

    return 0;
}

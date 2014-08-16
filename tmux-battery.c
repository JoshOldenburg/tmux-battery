#include <stdio.h>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/ps/IOPowerSources.h>
#include <IOKit/ps/IOPSKeys.h>

static const int threshold = 10;

struct batteryinfo {
	Boolean isPresent;
	Boolean externalConnected;
	Boolean fullyCharged;
	Boolean level;
};
struct batteryinfo *JOGetBatteryInfo(void);

int main() {
	struct batteryinfo *obj = JOGetBatteryInfo();

	if (!obj->isPresent) {
		printf("no battery\n");
		return 1;
	}

	if (obj->externalConnected) {
		if (obj->fullyCharged) printf("✔");
		else printf("▲ %d%%", obj->level);
	} else {
		if (obj->level < threshold) printf("▼ %d%%!", obj->level);
		else printf("▼ %d%%", obj->level);
	}

	printf("\n");
	free(obj);
	return 0;
}

Boolean JOGetBooleanForKey(CFDictionaryRef, CFStringRef);
SInt64 JOGetSInt64(CFNumberRef);

struct batteryinfo *JOGetBatteryInfo() {
	CFTypeRef blob = IOPSCopyPowerSourcesInfo();
	CFArrayRef arr = IOPSCopyPowerSourcesList(blob);
	CFDictionaryRef obj = IOPSGetPowerSourceDescription(blob, CFArrayGetValueAtIndex(arr, 0));

	if (!obj) {
		printf("no battery\n");
		exit(1);
	}

	struct batteryinfo *info = malloc(sizeof(struct batteryinfo));

	info->externalConnected = (CFStringCompare(IOPSGetProvidingPowerSourceType(blob), CFSTR(kIOPSACPowerValue), 0) == kCFCompareEqualTo);
	info->isPresent = JOGetBooleanForKey(obj, CFSTR(kIOPSIsPresentKey));
	info->fullyCharged = JOGetBooleanForKey(obj, CFSTR(kIOPSIsChargedKey));
	info->level = (JOGetSInt64(CFDictionaryGetValue(obj, CFSTR(kIOPSCurrentCapacityKey))) * 100 / JOGetSInt64(CFDictionaryGetValue(obj, CFSTR(kIOPSMaxCapacityKey))));

	return info;
}

Boolean JOGetBooleanForKey(CFDictionaryRef obj, CFStringRef key) {
	CFBooleanRef bValue = CFDictionaryGetValue(obj, key);
	return bValue ? CFBooleanGetValue(bValue) : false;
}

SInt64 JOGetSInt64(CFNumberRef number) {
	SInt64 retVal = 0;
	CFNumberGetValue(number, kCFNumberSInt64Type, &retVal);
	return retVal;
}

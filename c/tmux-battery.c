#include <stdio.h>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/ps/IOPowerSources.h>
#include <IOKit/ps/IOPSKeys.h>

SInt64 JOGetSInt64(CFNumberRef);

static const int threshold = 10;

int main() {
	CFTypeRef blob = IOPSCopyPowerSourcesInfo();
	CFArrayRef arr = IOPSCopyPowerSourcesList(blob);
	CFDictionaryRef obj = IOPSGetPowerSourceDescription(blob, CFArrayGetValueAtIndex(arr, 0));

	if (!obj) {
		printf("no battery\n");
		return 1;
	}

	//CFShow(blob);
	//CFShow(arr);
	//CFShow(obj);

	CFBooleanRef bIsPresent = CFDictionaryGetValue(obj, CFSTR(kIOPSIsPresentKey));
	CFBooleanRef bIsCharged = CFDictionaryGetValue(obj, CFSTR(kIOPSIsChargedKey));

	Boolean isPresent = bIsPresent ? CFBooleanGetValue(bIsPresent) : false;
	Boolean externalConnected = (CFStringCompare(IOPSGetProvidingPowerSourceType(blob), CFSTR(kIOPSACPowerValue), 0) == kCFCompareEqualTo);
	Boolean fullyCharged = bIsCharged ? CFBooleanGetValue(bIsCharged) : false;
	int level = (JOGetSInt64(CFDictionaryGetValue(obj, CFSTR(kIOPSCurrentCapacityKey))) * 100 / JOGetSInt64(CFDictionaryGetValue(obj, CFSTR(kIOPSMaxCapacityKey))));

	if (!isPresent) {
		printf("no battery\n");
		return 1;
	}

	if (externalConnected) {
		if (fullyCharged) printf("✔");
		else printf("▲ %d%%", level);
	} else {
		if (level < threshold) printf("▼ %d%%!", level);
		else printf("▼ %d%%", level);
	}

	printf("\n");
	return 0;
}

SInt64 JOGetSInt64(CFNumberRef number) {
	SInt64 retVal = 0;
	CFNumberGetValue(number, kCFNumberSInt64Type, &retVal);
	return retVal;
}

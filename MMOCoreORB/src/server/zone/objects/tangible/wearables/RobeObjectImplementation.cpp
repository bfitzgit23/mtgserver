#include "server/zone/objects/tangible/wearables/RobeObject.h"
#include "templates/SharedTangibleObjectTemplate.h"
#include <cstdlib> // for std::getenv

String RobeObjectImplementation::getSkillRequired() const {
	// Environment toggle: export SWGR_DISABLE_WEARABLE_SKILL_REQS=1
	if (const char* env = std::getenv("SWGR_DISABLE_WEARABLE_SKILL_REQS")) {
		if (*env && *env != '0') return "";
	}

	auto tanoTemp = dynamic_cast<const SharedTangibleObjectTemplate*>(templateObject.get());
	fatal(tanoTemp, "RobeObject must have a TANO template");

	if (tanoTemp->getCertificationsRequired().size() > 0) {
		return tanoTemp->getCertificationsRequired().get(0);
	}

	return "";
}

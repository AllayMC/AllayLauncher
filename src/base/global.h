#pragma once

namespace cpr {
class Session;
}

namespace allay_launcher::global {

std::shared_ptr<cpr::Session> CreateCprSession();

} // namespace allay_launcher::global

// Predefines

#define AL_INLINE inline

//
// AGSteam: Steam API Plugin for AGS
// (C) 2011-2017 MonkeyMoto Productions, Inc.
//
#ifndef AGSteam_SteamStats_H
#define AGSteam_SteamStats_H

#include "ags2client/Cpp11Fix.h"
#include "ags2client/IClientStats.h"

namespace AGSteam
{
	namespace Plugin
	{
        struct SteamStats_Statics;

		class SteamStats : public AGS2Client::IClientStats
		{
		protected:
            friend SteamStats_Statics;

			DEFAULT_CTOR(SteamStats);

		public:
			DEFAULT_DTOR(SteamStats);
			static SteamStats& GetSteamStats() noexcept;
			int GetIntStat(char const *name) const noexcept override; // retrieves a stat
			float GetFloatStat(char const *name) const noexcept override;
			float GetAverageRateStat(char const *name) const noexcept override;
			bool SetIntStat(char const *name, int value) const noexcept override; // sets a stat
			bool SetFloatStat(char const *name, float value) const noexcept override;
			bool UpdateAverageRateStat(char const *name, float numerator, float denominator) const noexcept override; // updates average rate with new data
		};
	} // namespace Plugin
} // namespace AGSteam

#endif // AGSteam_SteamStats_H

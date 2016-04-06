#include "regioncheck.hpp"

#include <sstream>
#include <map>

#include <components/esm/loadregn.hpp>

#include "../world/universalid.hpp"

CSMTools::RegionCheckStage::RegionCheckStage (const CSMWorld::IdCollection<ESM::Region>& regions)
: mRegions (regions)
{}

int CSMTools::RegionCheckStage::setup()
{
    return mRegions.getSize();
}

void CSMTools::RegionCheckStage::perform (int stage, CSMDoc::Messages& messages)
{
    const CSMWorld::Record<ESM::Region>& record = mRegions.getRecord (stage);

    if (record.isDeleted())
        return;

    const ESM::Region& region = record.get();

    CSMWorld::UniversalId id (CSMWorld::UniversalId::Type_Region, region.mId);

    // test for empty name
    if (region.mName.empty())
        messages.push_back (std::make_pair (id, region.mId + " has an empty name"));

    /// \todo test that the ID in mSleeplist exists

    // test that chances add up to 100
    int chances = region.mData.mClear + region.mData.mCloudy + region.mData.mFoggy + region.mData.mOvercast +
        region.mData.mRain + region.mData.mThunder + region.mData.mAsh + region.mData.mBlight +
        region.mData.mA + region.mData.mB;
    if (chances != 100)
        messages.add(id, "Weather chances do not add up to 100", "", CSMDoc::Message::Severity_Error);

    /// \todo check data members that can't be edited in the table view
}

#include "RoadExtraObject.h"
#include "Graphics/Colour.h"
#include "Graphics/Gfx.h"
#include "Interop/Interop.hpp"
#include "ObjectImageTable.h"
#include "ObjectManager.h"
#include "ObjectStringTable.h"
#include <cassert>

namespace OpenLoco
{
    // 0x00477EB9
    void RoadExtraObject::drawPreviewImage(Gfx::RenderTarget& rt, const int16_t x, const int16_t y) const
    {
        auto colourImage = Gfx::recolour(image, Colour::mutedDarkRed);

        Gfx::drawImage(&rt, x, y, colourImage + 36);
        Gfx::drawImage(&rt, x, y, colourImage + 37);
        Gfx::drawImage(&rt, x, y, colourImage);
        Gfx::drawImage(&rt, x, y, colourImage + 33);
        Gfx::drawImage(&rt, x, y, colourImage + 32);
    }

    // 0x00477E92
    bool RoadExtraObject::validate() const
    {
        if (paintStyle >= 2)
        {
            return false;
        }

        // This check missing from vanilla
        if (costIndex >= 32)
        {
            return false;
        }

        if (-sellCostFactor > buildCostFactor)
        {
            return false;
        }
        return buildCostFactor > 0;
    }

    // 0x00477E52
    void RoadExtraObject::load(const LoadedObjectHandle& handle, stdx::span<const std::byte> data, ObjectManager::DependentObjects*)
    {
        auto remainingData = data.subspan(sizeof(RoadExtraObject));

        // Load object name string
        auto strRes = ObjectManager::loadStringTable(remainingData, handle, 0);
        name = strRes.str;
        remainingData = remainingData.subspan(strRes.tableLength);

        // Load images
        auto imageRes = ObjectManager::loadImageTable(remainingData);
        var_0E = imageRes.imageOffset;
        image = imageRes.imageOffset + 8;

        // Ensure we've loaded the entire object
        assert(remainingData.size() == imageRes.tableLength);
    }

    // 0x00477E7E
    void RoadExtraObject::unload()
    {
        name = 0;
        var_0E = 0;
        image = 0;
    }
}
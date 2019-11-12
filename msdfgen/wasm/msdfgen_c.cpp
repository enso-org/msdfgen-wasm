#include "msdfgen_c.h"

#include "msdfgen.h"
#include "msdfgen-ext.h"

#include <ft2build.h>
#include <memory>
#include <cassert>

namespace {

constexpr const int CHANNELS_COUNT = 3;
constexpr const int MAX_OUTPUT_SIZE = 64;

std::unique_ptr<
    msdfgen::FreetypeHandle, decltype(&msdfgen::deinitializeFreetype)
> tt(nullptr, msdfgen::deinitializeFreetype);

float output_buffer[MAX_OUTPUT_SIZE*MAX_OUTPUT_SIZE*CHANNELS_COUNT];

}

extern "C" {

int msdfgen_maxMSDFSize()
{
    return MAX_OUTPUT_SIZE;
}

msdfgen::FontHandle* msdfgen_loadFontMemory(
        const unsigned char *data,
        long size)
{
    return msdfgen::loadFontMemory(tt.get(), data, size);
}

float* msdfgen_generateMSDF(
        int width,
        int height,
        msdfgen::FontHandle *fontHandle,
        int unicode,
        double edgeColoringAngleThreshold,
        double range,
        double scale_x,
        double scale_y,
        double translate_x,
        double translate_y,
        double edgeThreshold,
        char overlapSupport)
{
    assert(width <= MAX_OUTPUT_SIZE);
    assert(height <= MAX_OUTPUT_SIZE);
    msdfgen::Shape shape;
    if (!msdfgen::loadGlyph(shape, fontHandle, unicode)) {
        return nullptr;
    }
    edgeColoringSimple(shape, edgeColoringAngleThreshold);
    msdfgen::BitmapRef<float, 3> outputBmpRef(output_buffer, width, height);
    msdfgen::generateMSDF(
                outputBmpRef,
                shape,
                range,
                {scale_x, scale_y},
                {translate_x, translate_y},
                edgeThreshold,
                overlapSupport);
    return output_buffer;
}

void msdfgen_freeFont(msdfgen::FontHandle *fontHandle) {
    msdfgen::destroyFont(fontHandle);
}

}

int main() {
    tt.reset(msdfgen::initializeFreetype());
}

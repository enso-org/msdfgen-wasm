#include "msdfgen_c.h"

#include "msdfgen.h"
#include "msdfgen-ext.h"

#include <ft2build.h>
#include <memory>
#include <cassert>
#include <limits>

namespace {

constexpr const int CHANNELS_COUNT = 3;
constexpr const int MAX_OUTPUT_SIZE = 64;

using FtHandlePtr = std::unique_ptr<
        msdfgen::FreetypeHandle,
        decltype(&msdfgen::deinitializeFreetype)>;

FtHandlePtr &getFtHandle() {
    static FtHandlePtr ptr(
                msdfgen::initializeFreetype(),
                msdfgen::deinitializeFreetype);
    return ptr;
}

float output_buffer[MAX_OUTPUT_SIZE*MAX_OUTPUT_SIZE*CHANNELS_COUNT+4];

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
    std::vector<unsigned char> owned_data;
    owned_data.resize(size);
    memcpy(owned_data.data(), data, size);
    return msdfgen::loadFontMemory(getFtHandle().get(), std::move(owned_data));
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

float* msdfgen_generateAutoscaledMSDF(
        int width,
        int height,
        msdfgen::FontHandle *fontHandle,
        int unicode,
        double edgeColoringAngleThreshold,
        double range,
        double edgeThreshold,
        char overlapSupport)
{
    double advance = 0.0;
    double bearingX = 0.0;

    double l = std::numeric_limits<double>::max();
    double b = std::numeric_limits<double>::max();
    double r = std::numeric_limits<double>::min();
    double t = std::numeric_limits<double>::min();
    msdfgen::Vector2 translate;
    double scale = 1.0;

    msdfgen::Shape shape;
    if (!msdfgen::loadGlyph(shape, fontHandle, unicode, &advance, &bearingX)) {
        return nullptr;
    }

    shape.bounds(l, b, r, t);
    msdfgen::Vector2 frame(width, height);
    l -= .5*range;
    b -= .5*range;
    r += .5*range;
    t += .5*range;
    if (l >= r || b >= t)
        l = 0, b = 0, r = width, t = height;
    msdfgen::Vector2 dims(r-l, t-b);
    if (dims.x*frame.y < dims.y*frame.x) {
        translate.set(.5*(frame.x/frame.y*dims.y-dims.x)-l, -b);
        scale = std::min(2.0, frame.y/dims.y);
    } else {
        translate.set(-l, .5*(frame.y/frame.x*dims.x-dims.y)-b);
        scale = std::min(2.0, frame.x/dims.x);
    }

    float *writting_ptr = output_buffer;
    *(writting_ptr++) = translate.x - bearingX;
    *(writting_ptr++) = translate.y;
    *(writting_ptr++) = scale;
    *(writting_ptr++) = advance;

    edgeColoringSimple(shape, edgeColoringAngleThreshold);
    msdfgen::BitmapRef<float, 3> outputBmpRef(writting_ptr, width, height);
    msdfgen::generateMSDF(
                outputBmpRef,
                shape,
                range,
                {scale, scale},
                translate,
                edgeThreshold,
                overlapSupport);
    return output_buffer;
}



void msdfgen_freeFont(msdfgen::FontHandle *fontHandle) {
    msdfgen::destroyFont(fontHandle);
}

}

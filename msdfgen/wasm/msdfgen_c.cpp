#include "msdfgen_c.h"

#include "msdfgen.h"
#include "msdfgen-ext.h"

#include <ft2build.h>
#include <memory>
#include <cassert>
#include <limits>

namespace {

constexpr const int CHANNELS_COUNT = 3;

using FtHandlePtr = std::unique_ptr<
        msdfgen::FreetypeHandle,
        decltype(&msdfgen::deinitializeFreetype)>;

FtHandlePtr &getFtHandle() {
    static FtHandlePtr ptr(
                msdfgen::initializeFreetype(),
                msdfgen::deinitializeFreetype);
    return ptr;
}

MSDFGenResult *generateMSDFResult(
        int width,
        int height,
        const msdfgen::Shape &shape,
        double advance,
        double range,
        const msdfgen::Vector2 &scale,
        const msdfgen::Vector2 &translate,
        double edgeThreshold,
        bool overlapSupport)
{
    std::size_t msdfDataSize = width * height * CHANNELS_COUNT;
    std::size_t msdfDataSizeBytes = sizeof(float) * msdfDataSize;
    MSDFGenResult *result = (MSDFGenResult*)malloc(sizeof(MSDFGenResult) + msdfDataSizeBytes);
    result->advance = advance;
    result->translation[0] = translate.x;
    result->translation[1] = translate.y;
    result->scale[0] = scale.x;
    result->scale[1] = scale.y;

    msdfgen::BitmapRef<float, 3> outputBmpRef(result->msdf, width, height);
    msdfgen::generateMSDF(
                outputBmpRef,
                shape,
                range,
                scale,
                translate,
                edgeThreshold,
                overlapSupport);
    return result;
}

}

extern "C" {

msdfgen::FontHandle* msdfgen_loadFontMemory(
        const unsigned char *data,
        long size)
{
    std::vector<unsigned char> owned_data;
    owned_data.resize(size);
    memcpy(owned_data.data(), data, size);
    return msdfgen::loadFontMemory(getFtHandle().get(), std::move(owned_data));
}

double msdfgen_getKerning(
        msdfgen::FontHandle *fontHandle,
        int left_unicode,
        int right_unicode)
{
    double output;
    if (!msdfgen::getKerning(output, fontHandle, left_unicode, right_unicode))
        return std::numeric_limits<double>::quiet_NaN();
    return output;
}

MSDFGenResult* msdfgen_generateMSDF(
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
    double advance;
    msdfgen::Shape shape;
    if (!msdfgen::loadGlyph(shape, fontHandle, unicode, &advance)) {
        return nullptr;
    }
    edgeColoringSimple(shape, edgeColoringAngleThreshold);

    return generateMSDFResult(
                width,
                height,
                shape,
                advance,
                range,
                {scale_x, scale_y},
                {translate_x, translate_y},
                edgeThreshold,
                overlapSupport);
}

MSDFGenResult* msdfgen_generateAutoscaledMSDF(
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

    double l = std::numeric_limits<double>::max();
    double b = std::numeric_limits<double>::max();
    double r = std::numeric_limits<double>::min();
    double t = std::numeric_limits<double>::min();
    msdfgen::Vector2 translate;
    double scale = 1.0;

    msdfgen::Shape shape;
    if (!msdfgen::loadGlyph(shape, fontHandle, unicode, &advance)) {
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
        scale = std::min(2.0, std::floor(frame.y/dims.y*32.)/32.);
    } else {
        translate.set(-l, .5*(frame.y/frame.x*dims.x-dims.y)-b);
        scale = std::min(2.0, std::floor(frame.x/dims.x*32.)/32.);
    }

    edgeColoringSimple(shape, edgeColoringAngleThreshold);

    return generateMSDFResult(
                width,
                height,
                shape,
                advance,
                range,
                {scale,scale},
                translate,
                edgeThreshold,
                overlapSupport);
}

float *msdfgen_result_getMSDFData(MSDFGenResult *result)
{
    return result->msdf;
}

double msdfgen_result_getAdvance(MSDFGenResult *result)
{
    return result->advance;
}


double *msdfgen_result_getTranslation(MSDFGenResult *result)
{
    return result->translation;
}


double *msdfgen_result_getScale(MSDFGenResult *result)
{
    return result->scale;
}


void msdfgen_freeResult(MSDFGenResult *result)
{
    free(result);
}


void msdfgen_freeFont(msdfgen::FontHandle *fontHandle) {
    msdfgen::destroyFont(fontHandle);
}

}

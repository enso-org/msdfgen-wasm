#pragma once

namespace msdfgen {

struct FontHandle;

}

/*
 * Functions in this section are designed to be an emscripten API for Javascript
 */
extern "C" {

/*
 * A Mulitchannel signed distance field wilth glyph metadata
 *
 * The metadata consists of translation and scale of glyph outline before
 * generating MSDF, and an advance property of glyph.
 *
 * Translate and advance values are expressed in the msdf cells.
 */
struct MSDFGenResult {
    double advance;
    double translation[2];
    double scale[2];
    float msdf[0];
};

msdfgen::FontHandle* msdfgen_loadFontMemory(
        const unsigned char *data,
        long size);

void msdfgen_freeFont(msdfgen::FontHandle *fontHandle);

double msdfgen_getKerning(
        msdfgen::FontHandle *fontHandle,
        int left_unicode,
        int right_unicode);

/*
 * Generate MSDF
 *
 * This function returns some glyph metadata usable for text rendering.
 * The returned object must be freed by caller using msdfgen_freeResult.
 */
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
        double edgeThreshold = 1.001,
        char overlapSupport = 1);

/*
 * Generate MSDF
 *
 * A variant of `msdfgen_generateMSDF`, where the glyph is identified by
 * its index in the font file instead of unicode codepoint.
 */
MSDFGenResult* msdfgen_generateMSDFByIndex(
        int width,
        int height,
        msdfgen::FontHandle *fontHandle,
        int index,
        double edgeColoringAngleThreshold,
        double range,
        double scale_x,
        double scale_y,
        double translate_x,
        double translate_y,
        double edgeThreshold = 1.001,
        char overlapSupport = 1);


/*
 * Generate MSDF with autoframe
 *
 * Before MSDF generation, the glyph will framed to fit the distance field.
 * Returned result with contains the performed translation and scale factors.
 * The returned object must be freed by caller using msdfgen_freeResult.
 */
MSDFGenResult* msdfgen_generateAutoframedMSDF(
        int width,
        int height,
        msdfgen::FontHandle *fontHandle,
        int unicode,
        double edgeColoringAngleThreshold,
        double range,
        double max_scale,
        double edgeThreshold = 1.001,
        char overlapSupport = 1);

/*
 * Generate MSDF with autoframe
 *
 * A variant of `msdfgen_generateMSDF`, where the glyph is identified by
 * its index in the font file instead of unicode codepoint.
 */
MSDFGenResult* msdfgen_generateAutoframedMSDFByIndex(
        int width,
        int height,
        msdfgen::FontHandle *fontHandle,
        int index,
        double edgeColoringAngleThreshold,
        double range,
        double max_scale,
        double edgeThreshold = 1.001,
        char overlapSupport = 1);

// Accessors for JavaScript. For details see MSDFGenResult documentation
float* msdfgen_result_getMSDFData(MSDFGenResult *result);
double msdfgen_result_getAdvance(MSDFGenResult *result);
double* msdfgen_result_getTranslation(MSDFGenResult *result);
double* msdfgen_result_getScale(MSDFGenResult *result);

void msdfgen_freeResult(MSDFGenResult *);

}

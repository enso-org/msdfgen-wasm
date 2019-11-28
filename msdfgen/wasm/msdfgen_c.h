#pragma once

namespace msdfgen {

struct FontHandle;

}

extern "C" {

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

MSDFGenResult* msdfgen_generateAutoscaledMSDF(
        int width,
        int height,
        msdfgen::FontHandle *fontHandle,
        int unicode,
        double edgeColoringAngleThreshold,
        double range,
        double edgeThreshold = 1.001,
        char overlapSupport = 1);

float* msdfgen_result_getMSDFData(MSDFGenResult *result);
double msdfgen_result_getAdvance(MSDFGenResult *result);
double* msdfgen_result_getTranslation(MSDFGenResult *result);
double* msdfgen_result_getScale(MSDFGenResult *result);

void msdfgen_freeResult(MSDFGenResult *);

}

#pragma once

namespace msdfgen {

struct FontHandle;

}

extern "C" {

int msdfgen_maxMSDFSize();
msdfgen::FontHandle* msdfgen_loadFontMemory(
        const unsigned char *data,
        long size);
void msdfgen_freeFont(msdfgen::FontHandle *fontHandle);
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
        double edgeThreshold = 1.001,
        char overlapSupport = 1);

}

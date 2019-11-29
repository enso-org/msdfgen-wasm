Module.isInitialized = isInitialized
Module.addInitializationCb = addInitializationCb

if (typeof exports === 'object') {
    exports.ccall=ccall
    exports.getValue=getValue
    exports._msdfgen_loadFontMemory=_msdfgen_loadFontMemory
    exports._msdfgen_getKerning=_msdfgen_getKerning
    exports._msdfgen_generateMSDF=_msdfgen_generateMSDF
    exports._msdfgen_generateAutoframedMSDF=_msdfgen_generateAutoframedMSDF
    exports._msdfgen_result_getMSDFData=_msdfgen_result_getMSDFData
    exports._msdfgen_result_getAdvance=_msdfgen_result_getAdvance
    exports._msdfgen_result_getTranslation=_msdfgen_result_getTranslation
    exports._msdfgen_result_getScale=_msdfgen_result_getScale
    exports._msdfgen_freeResult=_msdfgen_freeResult
    exports._msdfgen_freeFont=_msdfgen_freeFont
    exports.addInitializationCb=addInitializationCb
    exports.isInitialized=isInitialized
}


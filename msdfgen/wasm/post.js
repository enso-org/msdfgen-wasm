Module.isInitialized = isInitialized
Module.addInitializationCb = addInitializationCb

if (typeof exports === 'object') {
    exports.ccall=ccall
    exports.getValue=getValue
    exports._msdfgen_maxMSDFSize=_msdfgen_maxMSDFSize
    exports._msdfgen_generateMSDF=_msdfgen_generateMSDF
    exports._msdfgen_freeFont=_msdfgen_freeFont
    exports.addInitializationCb=addInitializationCb
    exports.isInitialized=isInitialized
}


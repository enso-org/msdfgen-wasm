var Module = {
    initialized: false
}

Module.onRuntimeInitialized = function() {
    Module.initialized = true
}


function addInitializationCb(callback) {
    let prev_callback = Module.onRuntimeInitialized
    Module.onRuntimeInitialized = function() {
        prev_callback()
        callback()
    }
}

function isInitialized() {
    return Module.initialized
}
